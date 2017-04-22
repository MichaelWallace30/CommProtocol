/*
  Comms implementations.

  Copyright (C) 2016  Mario Garcia, Michael Wallace.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (At your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <CommProto/comms.h>
#include <CommProto/tools/data_structures/auto_queue.h>
#include <CommProto/serialization/objectstream.h>
#include <CommProto/architecture/macros.h>

#include <CommProto/network/udplink.h>
#include <CommProto/network/seriallink.h>
#include <CommProto/network/xbeelink.h>
#include <CommProto/network/tcplink.h>

#include <CommProto/debug/log.h>

#include <CommProto/callback.h>
#include <CommProto/tools/observer.h>

using namespace comnet;

/***********************************************/
/******************* Private *******************/
/***********************************************/

/** function for communication thread */
void Comms::CommunicationHandlerSend()
{
 while (this->IsRunning() && conn_layer)
 {
  send_mutex.Lock();
  bool isEmpty = send_queue->IsEmpty();
  send_mutex.Unlock();
  if (isEmpty)
  {
    comm_cond_var_send.Wait();
  }
  send_mutex.Lock();
  isEmpty = send_queue->IsEmpty();
  send_mutex.Unlock();
  if (!isEmpty)
  {
    send_mutex.Lock();
    ObjectStream *temp = send_queue->Front();
    send_queue->Dequeue();
    send_mutex.Unlock();

    int32_t timeSinceStart = (int32_t)GetTimeSinceStart();
    temp->GetHeaderPacket().SetSourceTime(timeSinceStart);
    temp->SerializeHeader();

    //Send data here
    conn_layer->Send(temp->header_packet.dest_id, temp->GetBuffer(), temp->GetSize());
    if (conStateManager) {
      conStateManager->ResetSendTime(temp->header_packet.dest_id);
    }
    free_pointer(temp);
  }
 }
 debug::Log::Message(debug::LOG_DEBUG, "Send Ends!");
}

/** function for communication thread */
void Comms::CommunicationHandlerRecv() {
  while (this->IsRunning() && conn_layer) {
    AbstractPacket* packet = NULL;
    uint8_t stream_buffer[MAX_BUFFER_SIZE];
    uint32_t recv_len = 0;
    bool received = conn_layer->Recv(stream_buffer, &recv_len);
    ObjectStream temp;
    if ( received ) {
      temp.SetBuffer((char*)stream_buffer, recv_len);
      int32_t err = decrypt.Decrypt(&temp);
      if(err == 1) {
        debug::Log::Message(debug::LOG_NOTE, "Packet was decrypted!");
      }
      else if (err == 0) {
        debug::Log::Message(debug::LOG_WARNING,
          "Packet was not decrypted!\n Either encryption is not set or key was not loaded!");
      } else {
        debug::Log::Message(debug::LOG_DEBUG, "Discarding garbage data...");
      }
      /*
      Algorithm should Get the header, Get the message id from header, then
      produce the packet from the header, finally Get the callback.
      */
      if(temp.GetSize() > 0) {
        debug::Log::Message(debug::LOG_DEBUG, "Comms packet unpacking...\n");
        Header header = temp.GetHeaderPacket();

        // Create the packet.
        packet = this->packet_manager.ProduceFromId(header.msg_id);

        if (conStateManager) {
          conStateManager->UpdatePing(header.source_id, header.GetSourceTime());
        }

        if(packet) {
          // Unpack the object stream.
          packet->Unpack(temp);
          Callback* callback = NULL;
          callback = this->packet_manager.Get(*packet);

          if(callback) {
            error_t error;
            /*
            TODO(Wallace): This might need to Run on a separate thread, or
            on a new thread, to prevent it from stopping the Receive handler.
            User figures out what to do with the packet.
            */
            error = callback->CallFunction(header, *packet, (CommNode&)*this);
            // Handle error.
            HandlePacket(error, header.source_id, packet);
          } else {
            // store the packet into the Receive queue.
            CommLock recvLock(recv_mutex);
            recv_queue->Enqueue(std::make_pair(header.source_id, packet));
          }
        } else {
          debug::Log::Message(debug::LOG_NOTIFY, "Unknown packet recieved.");
        }
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
  debug::Log::Message(debug::LOG_DEBUG, "recv ends!");
}

/***********************************************/
/******************* Public  *******************/
/***********************************************/
Comms::Comms(uint8_t platformID)
: CommNode(platformID)
, encrypt(encryption::CommEncryptor(encryption::AES))
,conStateManager(nullptr)
{
  decrypt = encryption::CommDecryptor(encryption::AES, &encrypt);
  this->recv_queue = new AutoQueue <std::pair<uint8_t, AbstractPacket*>>;
  this->send_queue = new AutoQueue <ObjectStream*>;
  if (ConnectionStateManager::ConStateEnabled) {
    conStateManager = std::make_shared <ConnectionStateManager>(this);
  }
  conn_layer = NULL;
}

Comms::~Comms()
{
  Stop();
  while (comm_thread_recv.IsJoinable() || comm_thread_send.IsJoinable()) {
    comm_thread_recv.Join();
    comm_cond_var_send.Set();
    comm_thread_send.Join();
  }
  free_pointer(conn_layer);
  if (conStateManager) {
    conStateManager->Stop();
  }
}



bool Comms::LoadKey(char* key)
{
  return static_cast<bool>(encrypt.LoadKey(key));
}


bool Comms::LoadKeyFromFile(char* keyFileName)
{
  if (encrypt.LoadKeyFromFile(keyFileName) > 0) {
    return true;
  }
  else {
    return false;
  }
}


bool Comms::InitConnection(transport_protocol_t conn_type, 
                           const char* port, 
                           const char* address, 
                           uint32_t baudrate)
{
  if (conn_layer != nullptr) {
    free_pointer(conn_layer);
  }

  uint16_t length = 0;
  bool connectionInitialized = false;
  switch (conn_type) {
    case UDP_LINK: 
    {
      if (address != NULL)
      {
        str_length(address, length);
        if (length < ADDRESS_LENGTH)
        {
          conn_layer = new UDPLink();
          connectionInitialized = conn_layer->InitConnection(port, address);
        }
      }
      break;
    }
    case SERIAL_LINK:
    {
      conn_layer = new SerialLink();
      connectionInitialized = conn_layer->InitConnection(port, NULL, baudrate);
   break;
    }
    case ZIGBEE_LINK:
    {
      conn_layer = new XBeeLink();
      connectionInitialized = conn_layer->InitConnection(port, NULL, baudrate);
   break;
      // TODO(Garcia): Will need to create throw directives instead.
    }
		case TCP_LINK:
		{
			conn_layer = new TCPLink();
			connectionInitialized = conn_layer->InitConnection(port, address, GetNodeId());
		break;
		}
    default:
    {
      debug::Log::Message(debug::LOG_WARNING, "NO CONNECTION ESTABLISHED!");
    }
  }
  if (connectionInitialized) {
    if (conStateManager) {
      conStateManager->LinkCallbacks();
    }
    return true;
  }
  return false;
}


bool Comms::AddAddress(uint8_t dest_id, const char* address , uint16_t port)
{
 if (conn_layer == NULL) return false;
 if (conn_layer->AddAddress(dest_id, address, port))
 {
   if (conStateManager) {
     conStateManager->AddConState(dest_id);
   }
   return true;
 }
 return false;
}


bool Comms::RemoveAddress(uint8_t dest_id)
{
  if (conn_layer == NULL) return false;
  if (conn_layer->RemoveAddress(dest_id))
  {
    if (conStateManager) {
      conStateManager->RemoveConState(dest_id);
    }
    return true;
  }
  return false;
}

bool Comms::Send(AbstractPacket& packet, uint8_t dest_id) {
  if (conn_layer == NULL) { 
    return false;
  }
  
  ObjectStream *stream = new ObjectStream();
  // Pack the stream with the packet.		
  packet.Pack(*stream);		
  Header header;

  header.dest_id = dest_id;
  header.source_id = this->GetNodeId();
  header.msg_id = packet.GetId();
  header.msg_len = stream->GetSize();
  stream->SetHeader(header);
  
  if(encrypt.Encrypt(stream)) {
    debug::Log::Message(debug::LOG_NOTE, "Packet was encrypted!\n");
  } else {
    debug::Log::Message(debug::LOG_WARNING, 
                "Packet was not encrypted! Either encryption was not created, or key was not loaded!");
  }
  
  send_mutex.Lock();
  send_queue->Enqueue(stream);
  send_mutex.Unlock();
  comm_cond_var_send.Set();
  return true;
}

bool comnet::Comms::ReplaceSendQueue(const Queue<ObjectStream*>* queue)
{
  CommLock sendLock(send_mutex);
  return comnet::CommNode::ReplaceSendQueue(queue);
}

bool comnet::Comms::ReplaceReceiveQueue(const Queue<std::pair<uint8_t, AbstractPacket*>>* queue)
{
  CommLock recvLock(recv_mutex);
  return comnet::CommNode::ReplaceReceiveQueue(queue);
}


AbstractPacket* Comms::Receive(uint8_t&  source_id) {
  CommLock recvLock(recv_mutex);
  AbstractPacket* packet = nullptr;
  if (conn_layer != nullptr && !recv_queue->IsEmpty()) {
    // This is a manual Receive function. The user does not need to call this function,
    // however it SHOULD be used to manually grab a packet from the "orphanage" queue.
    std::pair <uint8_t, AbstractPacket*> queueElm = recv_queue->Front();
    recv_queue->Dequeue();
    source_id = queueElm.first;
    return queueElm.second;
  }
  return NULL;
}


void Comms::Run()
{
  CommNode::Run();
  comm_thread_send = CommThread(&Comms::CommunicationHandlerSend, this);
  comm_thread_recv = CommThread(&Comms::CommunicationHandlerRecv, this);
  if (conStateManager) {
    conStateManager->Run();
  }
}


void Comms::Stop()
{
  CommNode::Stop();
}


void Comms::Pause()
{
  CommNode::Pause();
}


void Comms::LogToConsoles() {
}


void Comms::HandlePacket(error_t error, uint8_t sourceID, AbstractPacket* packet) {
  if ((error & CALLBACK_SUCCESS) == CALLBACK_SUCCESS) {
    debug::Log::Message(debug::LOG_NOTIFY, "PACKET SUCCESSFULL.");
  } else if ((error & CALLBACK_FAIL) == CALLBACK_FAIL) {
    debug::Log::Message(debug::LOG_NOTIFY, "PACKET FAILED.");
  }
  if ((error & CALLBACK_DESTROY_PACKET) == CALLBACK_DESTROY_PACKET) {
    debug::Log::Message(debug::LOG_NOTIFY, "DESTROYING PACKET.");
    free_pointer(packet);
  }
  else {
    if (packet != nullptr) {
      debug::Log::Message(debug::LOG_NOTIFY, "STORING PACKET.");
      CommLock recvLock(recv_mutex);
      recv_queue->Enqueue(std::make_pair(sourceID, packet));
    }
  }
}