#include <CallBack.h>

namespace Comnet {


// Helper delegate, is not to be known by the user.
public delegate error_t Callb(const header_t& header, AbstractPacket& packet);


Int32 ErrorFunct(CHeader^ h, ABSPacket^ pack) {
  return -100;
}


CallBack::CallBack(ABSPacket^ ref)
: funct(gcnew CommFunct(ErrorFunct))
, reference(ref)
{
  Callb^ temp = gcnew Callb(this, &CallBack::helper);
  IntPtr ptr = Marshal::GetFunctionPointerForDelegate(temp);
  callback = new Callback(static_cast<callback_t>(ptr.ToPointer()));
}


CallBack::CallBack(ABSPacket^ ref, CommFunct^ funct)
: funct(funct)
, reference(ref)
{
  Callb^ temp = gcnew Callb(this, &CallBack::helper);
  IntPtr ptr = Marshal::GetFunctionPointerForDelegate(temp);
  callback = new Callback(static_cast<callback_t>(ptr.ToPointer()));
}



CallBack::~CallBack() 
{
  delete callback;
  callback = nullptr;
}


void CallBack::SetCallBackListenter(CommFunct^ ptr) {
  funct = ptr;
}


Int32 CallBack::CallFunction(CHeader^ header, ABSPacket^ packet) {
  return funct(header, packet);

}


error_t CallBack::helper(const header_t& header, AbstractPacket& packet) {
  CHeader^ managedHeader = gcnew CHeader();
  managedHeader->header = (header_t *)&header;  

  ABSPacket^ managedPacket = reference->Create();
  managedPacket->SetAbstractPacket(&packet);

  return funct(managedHeader, managedPacket);
}
}