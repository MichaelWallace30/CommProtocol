#include <CallBack.h>

namespace Comnet {


// Helper delegate, is not to be known by the user.
public delegate error_t Callb(const comnet::Header& header, AbstractPacket& packet);


Int32 ErrorFunct(Header^ h, ABSPacket^ Pack, CommNode^ node) {
  return -100;
}


CallBack::CallBack()
: funct(gcnew CommFunct(ErrorFunct))
{
}


CallBack::CallBack(CommFunct^ funct)
: funct(funct)
{
}



CallBack::~CallBack() 
{
}


void CallBack::SetCallBackListenter(CommFunct^ ptr) {
  funct = ptr;
}


Int32 CallBack::CallFunction(Header^ header, ABSPacket^ packet, CommNode^ node) {
  return funct(header, packet, node);

}
} // Comnet