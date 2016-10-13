#include <Comms.h>
#include <network/SerialLink.h>
#include <network/UDPLink.h>
#include <network/XbeeLink.h>
#include <tools/data_structures/AutoQueue.h>
#include <ObjectStream.h>
#include <pkg/PacketManager.h>


namespace Comnet {


using namespace Comnet::Tools::DataStructures;


Void Comms::commHelperRecv() {
  while (IsRunning()) {
    ABSPacket^ packet = nullptr;
    uint8_t stream_buffer[MAX_BUFFER_SIZE];
    UInt32 recv_length = 0;
    connLayer->Recv(stream_buffer, recv_length); 
    ObjectStream^ temp = gcnew ObjectStream();
    temp->unmangedObjectStream->Get().SetBuffer((char *)stream_buffer, recv_length);
    if (temp->GetSize() > 0) {
      Header^ header = gcnew Header(&temp->unmangedObjectStream->Get().DeserializeHeader());
      packet = this->packetManager->ProduceFromId(header->GetMessageID());
      if (packet) {
        packet->Unpack(temp);
        CallBack^ callback = nullptr;
        callback = this->packetManager->Get(packet);
        if (callback) {
          error_t error;  
          error = callback->CallFunction(header, packet, static_cast<CommNode^>(this));
          // Do something with the packet.
          switch (error) {
            case CALLBACK_SUCCESS: break;
            case CALLBACK_FAIL: break;
            case CALLBACK_DESTROY_PACKET: break;
            default: break;
          }
        }
      }
    }
  }
}


Void Comms::commHelperSend() {
  while (IsRunning()) {
    if (!sendQueue->IsEmpty()) {
      ObjectStream^ temp = sendQueue->DeQueue();
      connLayer->Send(temp->unmangedObjectStream->Get().header_packet.dest_id, 
                      temp->unmangedObjectStream->Get().GetBuffer(),
                      temp->unmangedObjectStream->Get().GetSize());
    }
  }
}


Comms::Comms(UInt32 id)
: CommNode(id) 
{
  this->recvQueue = gcnew AutoQueue<ABSPacket^>();
  this->sendQueue = gcnew AutoQueue<ObjectStream^>();
  this->sendMut = gcnew Threading::Mutex();
  this->recvMut = gcnew Threading::Mutex();
  recvThr = gcnew Threading::Thread(gcnew Threading::ThreadStart(this, &Comms::commHelperRecv));
  sendThr = gcnew Threading::Thread(gcnew Threading::ThreadStart(this, &Comms::commHelperSend));
  connLayer = nullptr;
  this->packetManager = gcnew PacketManager();
}


Comms::~Comms()
{
  if (connLayer) {
    delete connLayer;
  }
}


Boolean Comms::InitConnection(TransportProtocol connType, String^ port, String^ addr, UInt32 baudrate) {
  UInt16 length = 0;
  switch (connType) {
    case TransportProtocol::UDP_LINK: {
      if (addr->Length < ADDRESS_LENGTH) {
        connLayer = gcnew Network::UDPLink();
        return connLayer->InitConnection(port, addr, baudrate);
      }
      break;
    }
    case TransportProtocol::SERIAL_LINK: {
      if (addr->Length < ADDRESS_LENGTH) {
        connLayer = gcnew Network::SerialLink();
        return connLayer->InitConnection(port, nullptr, baudrate);
      }
      break;
    }
    case TransportProtocol::ZIGBEE_LINK: {
      if (addr->Length < ADDRESS_LENGTH) {
        connLayer = gcnew Network::XBeeLink();
        return connLayer->InitConnection(port, nullptr, baudrate);
      }
      break;
    }
    default:
      return false;
  }

  return false;
}


Boolean Comms::AddAddress(UInt16 destId, String^ addr, UInt16 port) {
  if (connLayer == nullptr) return false;
  return connLayer->AddAddress(destId, addr, port);
}


Boolean Comms::RemoveAddress(UInt16 destId) {
  if (connLayer) {
    return connLayer->RemoveAddress(destId);
  }
  return false;
}


Void Comms::Run() {
  CommNode::Run();
  if (IsRunning()) {
    recvThr->Start();
    sendThr->Start();
  }
}


Void Comms::Pause() {
  CommNode::Pause();
  if (IsPaused()) {
    sendThr->Suspend();
    recvThr->Suspend();
  }
}


Void Comms::Stop() {
  Comms::Stop();
  if (!IsRunning() && !IsPaused()) {
    sendThr->Suspend();
    recvThr->Suspend();
  }
}


Boolean Comms::Send(ABSPacket^ packet, Byte destId) {
  if (!connLayer) {
    return false;
  }
  ObjectStream^ stream = gcnew ObjectStream();
  packet->Pack(stream);
  Header^ header = gcnew Header();
  header->SetDestID(destId);
  header->SetSourceID(this->GetNodeId());
  header->SetMessageID(packet->GetAbstractPacket()->GetId());
  header->SetMessageLength(stream->GetSize());
  stream->SerializeHeader(header);
  sendQueue->EnQueue(stream);
  COMMS_DEBUG("Sending packet...\n");
  return true;
}


ABSPacket^ Comms::Receive(Byte% sourceId) {
  if (connLayer && !recvQueue->IsEmpty()) {
    return recvQueue->DeQueue();
  }
  return nullptr;
}
} // Comnet