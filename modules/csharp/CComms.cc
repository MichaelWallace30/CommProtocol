#include <CComms.h>
#include <network/CSerialLink.h>
#include <network/CUDPLink.h>
#include <network/CXbeeLink.h>
#include <tools/data_structures/CAutoQueue.h>
#include <CObjectStream.h>
#include <pkg/CPacketManager.h>


namespace Comnet {


using namespace Comnet::Tools::DataStructures;


Void CComms::commHelperRecv() {
  while (IsRunning()) {
    ABSPacket^ packet = nullptr;
    uint8_t stream_buffer[MAX_BUFFER_SIZE];
    UInt32 recv_length = 0;
    connLayer->recv(stream_buffer, recv_length); 
    CObjectStream^ temp = gcnew CObjectStream();
    temp->unmangedObjectStream->get().setBuffer((char *)stream_buffer, recv_length);
    if (temp->getSize() > 0) {
      CHeader^ header = gcnew CHeader(&temp->unmangedObjectStream->get().deserializeHeader());
      packet = this->packetManager->ProduceFromId(header->getMessageID());
      if (packet) {
        packet->Unpack(temp);
        CallBack^ callback = nullptr;
        callback = this->packetManager->Get(packet);
        if (callback) {
          error_t error;  
          error = callback->CallFunction(header, packet);
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


Void CComms::commHelperSend() {
  while (IsRunning()) {
    if (!sendQueue->IsEmpty()) {
      CObjectStream^ temp = sendQueue->DeQueue();
      connLayer->send(temp->unmangedObjectStream->get().headerPacket.destID, 
                      temp->unmangedObjectStream->get().getBuffer(),
                      temp->unmangedObjectStream->get().getSize());
    }
  }
}


CComms::CComms(UInt32 id)
: CCommNode(id) 
{
  this->recvQueue = gcnew CAutoQueue<ABSPacket^>();
  this->sendQueue = gcnew CAutoQueue<CObjectStream^>();
  this->sendMut = gcnew Threading::Mutex();
  this->recvMut = gcnew Threading::Mutex();
  recvThr = gcnew Threading::Thread(gcnew Threading::ThreadStart(this, &CComms::commHelperRecv));
  sendThr = gcnew Threading::Thread(gcnew Threading::ThreadStart(this, &CComms::commHelperSend));
  connLayer = nullptr;
  this->packetManager = gcnew CPacketManager();
}


CComms::~CComms()
{
  if (connLayer) {
    delete connLayer;
  }
}


Boolean CComms::InitConnection(TransportProtocol connType, String^ port, String^ addr, UInt32 baudrate) {
  UInt16 length = 0;
  switch (connType) {
    case TransportProtocol::UDP_LINK: {
      if (addr->Length < ADDRESS_LENGTH) {
        connLayer = gcnew Network::CUDPLink();
        return connLayer->initConnection(port, addr, baudrate);
      }
      break;
    }
    case TransportProtocol::SERIAL_LINK: {
      if (addr->Length < ADDRESS_LENGTH) {
        connLayer = gcnew Network::CSerialLink();
        return connLayer->initConnection(port, nullptr, baudrate);
      }
      break;
    }
    case TransportProtocol::ZIGBEE_LINK: {
      if (addr->Length < ADDRESS_LENGTH) {
      }
      break;
    }
    default:
      return false;
  }

  return false;
}


Boolean CComms::AddAddress(UInt16 destId, String^ addr, UInt16 port) {
  if (connLayer == nullptr) return false;
  return connLayer->addAddress(destId, addr, port);
}


Boolean CComms::RemoveAddress(UInt16 destId) {
  if (connLayer) {
    return connLayer->removeAddress(destId);
  }
  return false;
}


Void CComms::Run() {
  CCommNode::Run();
  if (IsRunning()) {
    recvThr->Start();
    sendThr->Start();
  }
}


Void CComms::Pause() {
  CCommNode::Pause();
  if (IsPaused()) {
    sendThr->Suspend();
    recvThr->Suspend();
  }
}


Void CComms::Stop() {
  CComms::Stop();
  if (!IsRunning() && !IsPaused()) {
    sendThr->Suspend();
    recvThr->Suspend();
  }
}


Boolean CComms::Send(ABSPacket^ packet, Byte destId) {
  if (!connLayer) {
    return false;
  }
  CObjectStream^ stream = gcnew CObjectStream();
  packet->Pack(stream);
  CHeader^ header = gcnew CHeader();
  header->setDestID(destId);
  header->setSourceID(this->GetNodeId());
  header->setMessageID(packet->GetAbstractPacket()->getId());
  header->setMessageLength(stream->getSize());
  stream->serializeHeader(header);
  sendQueue->EnQueue(stream);
  COMMS_DEBUG("Sending packet...\n");
  return true;
}


ABSPacket^ CComms::Receive(Byte% sourceId) {
  if (connLayer && !recvQueue->IsEmpty()) {
    return recvQueue->DeQueue();
  }
  return nullptr;
}
} // Comnet