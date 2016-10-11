#define _DEBUG 1
#include <CommProto/comms.h>
#include <CommProto/tools/data_structures/auto_queue.h>
#include <CommProto/serialization/objectstream.h>
#include <CommProto/architecture/macros.h>

#include <CommProto/network/udplink.h>
#include <CommProto/network/seriallink.h>
#include <CommProto/network/xbeelink.h>

#include <CommProto/debug/comms_debug.h>

#include <CommProto/callback.h>

using namespace comnet;

/***********************************************/
/******************* Private *******************/
/***********************************************/

/** function for communication thread */
void Comms::CommunicationHandlerSend()
{
	while (this->IsRunning())
	{
		if (!send_queue->IsEmpty())
		{
      send_mutex.Lock();
			//Send data here
			ObjectStream *temp = send_queue->Front();
			send_queue->Dequeue();
			conn_layer->Send(temp->header_packet.dest_id, temp->GetBuffer(), temp->GetSize());
			free_pointer(temp);
      send_mutex.Unlock();
		}
//		COMMS_DEBUG("IM GOING!!\n");
	}
}

/** function for communication thread */
void Comms::CommunicationHandlerRecv() {
  while (this->IsRunning()) {
    recv_mutex.Lock();
    AbstractPacket* packet = NULL;
    //Send data here
	  uint8_t stream_buffer[MAX_BUFFER_SIZE];
    uint32_t recv_len = 0;
    conn_layer->Recv(stream_buffer, &recv_len);
    ObjectStream *temp = new ObjectStream();
    temp->SetBuffer((char*)stream_buffer, recv_len);
    /*
      Algorithm should Get the header, Get the message id from header, then
      produce the packet from the header, finally Get the callback.
     */
    if (temp->GetSize() > 0) {
      COMMS_DEBUG("Comms packet unpacking...\n");
      Header header = temp->DeserializeHeader();
      // Create the packet.
      packet = this->packet_manager.ProduceFromId(header.msg_id);
    
      if (packet) {
        // Unpack the object stream.
        packet->Unpack(*temp);
        Callback* callback = NULL;
        callback = this->packet_manager.Get(*packet);

        if (callback) {
          error_t error;
          /*
            TODO(Wallace): This might need to Run on a separate thread, or 
            on a new thread, to prevent it from stopping the Receive handler.
            User figures out what to do with the packet.
          */
          error = callback->CallFunction(header, *packet);
          // Handle error.
          switch (error) {
            case CALLBACK_SUCCESS:
              cout << "Successful callback" << endl; break;
            default:  
              cout << "Nothing" << endl;
          };
        } else {
          // store the packet into the Receive queue.
          recv_queue->Enqueue(packet);
        }
      } else {
        COMMS_DEBUG("Unknown packet recieved.\n");
      }	
    }

    free_pointer(temp);		
    recv_mutex.Unlock();	
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));	
  }
  COMMS_DEBUG("recv ends!\n");
}

/***********************************************/
/******************* Public  *******************/
/***********************************************/
Comms::Comms(uint8_t platformID)
: CommNode(platformID)
{
	this->recv_queue = new AutoQueue <AbstractPacket*>;
	this->send_queue = new AutoQueue <ObjectStream*>;
	conn_layer = NULL;
}

Comms::~Comms()
{
	free_pointer(conn_layer);
}

bool Comms::InitConnection(transport_protocol_t conn_type, const char* port, const char* address, uint32_t baudrate)
{
	uint16_t length = 0;
	switch (conn_type)
	{
		case UDP_LINK:
		{			
			
			str_length(address, length);
			if (length < ADDRESS_LENGTH)
			{	
			  COMMS_DEBUG("UDP connection.\n");
				conn_layer = new UDPLink();
				return conn_layer->InitConnection(port, address);
			}
			break;
		}
		case SERIAL_LINK:
		{
			
			str_length(address, length);
			if (length < ADDRESS_LENGTH)
			{
				conn_layer = new SerialLink();
				return conn_layer->InitConnection(port, NULL, baudrate);
			}
			break;
		
		}
		case ZIGBEE_LINK:
		{
      str_length(address, length);
      if (length < ADDRESS_LENGTH + 2) {
        conn_layer = new experimental::XBeeLink();
        return conn_layer->InitConnection(port, NULL, baudrate);
      }
      break;
    }
		default:
		  COMMS_DEBUG("NO CONNECTION\n");
		{return false;}
	}
	return true;
}


bool Comms::AddAddress(uint8_t dest_id, const char* address , uint16_t port)
{
	if (conn_layer == NULL) return false;
	return conn_layer->AddAddress(dest_id, address, port);
}


bool Comms::RemoveAddress(uint8_t dest_id)
{
	if (conn_layer == NULL) return false;
	return conn_layer->RemoveAddress(dest_id);
}


bool Comms::Send(AbstractPacket* packet, uint8_t dest_id) {
  if (conn_layer == NULL) { 
    return false;
  }
  
  ObjectStream *stream = new ObjectStream();
  // Pack the stream with the packet.		
  packet->Pack(*stream);		
  Header header;

  header.dest_id = dest_id;
  header.source_id = this->GetNodeId();
  header.msg_id = packet->GetId();
  header.msg_len = stream->GetSize();
  //
  //call encryption here
  //
  stream->SerializeHeader(header);
  send_queue->Enqueue(stream);

  return true;
}


AbstractPacket* Comms::Receive(uint8_t&  source_id) {
  if (conn_layer == NULL) return NULL;
  if (!recv_queue->IsEmpty()) {
    cout << "Message recv in Comms" << endl;
    // This is a manual Receive function. The user does not need to call this function,
    // however it SHOULD be used to manually grab a packet from the "orphanage" queue.
    recv_queue->Dequeue();  
  }
	
  return NULL;
}


void Comms::Run()
{
  CommNode::Run();
  comm_thread_send = CommThread(&Comms::CommunicationHandlerSend, this);
  comm_thread_recv = CommThread(&Comms::CommunicationHandlerRecv, this);
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
