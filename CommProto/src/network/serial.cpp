/* 
  Implementation of x-platform serial connections.

  Copyright (C) 2016  Michael Wallace, Mario Garcia, Alex Craig.

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
#include <CommProto/network/serial.h>
#include <CommProto/network/crc32.h>

#include <CommProto/debug/log.h>


namespace comnet {
namespace network {


/***********************************************/
/******************* Private *******************/
/***********************************************/
#if COMMPROTO_TARGET_OS == COMMMPROTO_OS_WINDOWS


// Defines the Serial keyword for the preprocessor, allows us to focus on only one 
// set of serial functions in an operating system.
#define WINDOWS_SERIAL

/**
  Initializes windows serial port.
*/
bool InitWindows(Serial& serial, const char* comPort, uint32_t baudrate)
{
  serial_t& h_serial = serial.GetSerialPort();

  char comPortCat[80];

  if(comPort[0] != '\\') {
    // "\\\\.\\"  allows windows ports above 9
    strcpy(comPortCat, "\\\\.\\");
  }

  strcat(comPortCat, (char*)comPort);

#ifdef UNICODE	//convert multybyte string to LPCWSTR
  uint16_t length = 0;
  str_length(comPortCat, length);
  wchar_t* str = new wchar_t[length * 2];
  MultiByteToWideChar(CP_ACP, 0, comPortCat, -1, str, length * 2);
  LPCWSTR comport = str;
#else
  //open up serial port  
  const char* comport = comPortCat;
#endif
  //Nonoverlapped port
  h_serial.h_serial = CreateFile(comport,
    GENERIC_READ | GENERIC_WRITE,
    0,
    0,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    0);

#ifdef UNICODE //clean up multybyte string to LPCWSTR
  delete str;
#endif
  
  if (h_serial.h_serial == INVALID_HANDLE_VALUE)
  {
    if (GetLastError() == ERROR_FILE_NOT_FOUND)
    {
      //serial port does not exist.
      debug::Log::Message(debug::LOG_ERROR, "comport not found\n");
      return false;
    }
    // some other error occured.
    debug::Log::Message(debug::LOG_ERROR, "Unknown error\n");
    return false;
  }
  else
  {
		debug::Log::Message(debug::LOG_NOTIFY, "Serial port created\n");
  }
  
  
  //setup parameters
  
  DCB dcbSerialParams = { 0 };
  
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
  
  if (!GetCommState(h_serial.h_serial, &dcbSerialParams))
  {
    //error getting state
		debug::Log::Message(debug::LOG_ERROR, "Error getting state\n");
    return false;
  }
  
  dcbSerialParams.BaudRate = (DWORD)baudrate;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;
  
  if (!SetCommState(h_serial.h_serial, &dcbSerialParams))
  {
    //error setting serial port state
    debug::Log::Message(debug::LOG_ERROR, "Error setting serial port state\n");
    return false;
  }
  
  
	debug::Log::Message(debug::LOG_NOTIFY, "Completed setting serial port state\n");
  

  //time out code not needed?
  
  COMMTIMEOUTS timeouts = { 0 };
  
  timeouts.ReadIntervalTimeout = 5;
  timeouts.ReadTotalTimeoutConstant = 5;
  timeouts.ReadTotalTimeoutMultiplier = 2;
  timeouts.WriteTotalTimeoutConstant = 25;
  timeouts.WriteTotalTimeoutMultiplier = 5;
  
  if (!SetCommTimeouts(h_serial.h_serial, &timeouts))
  {
    //error occureeed
		debug::Log::Message(debug::LOG_ERROR, "Setting up times outs failed\n");
    return false;
  }
  
	debug::Log::Message(debug::LOG_NOTIFY, "Completed setting up time outs\n");
  h_serial.serial_s = SERIAL_CONNECTED;
  return true;
}


inline bool
WindowsSend(Serial& serial, uint8_t dest_id, uint8_t* txData, int32_t txLength) {
  unsigned long sent_data = 0;//windows want LPDWORD == unsignled long != uint32_t || uint16_t
	debug::Log::Message(debug::LOG_NOTIFY, "Sending packet\n.");
  serial_t& h_serial = serial.GetSerialPort();
  
  if (!WriteFile(h_serial.h_serial, txData, txLength, &sent_data, NULL)) {
    //error reading file
    debug::Log::Message(debug::LOG_ERROR, "Failed to write serial\n");
    return false;
  }
  else {
		LOG_PRINTF(debug::LOG_NOTIFY, "**  Sent\t Length: %d, Sent: %d, destID: %d **\n", txLength, sent_data, dest_id);
		return true;
  }		

  return false;
}


inline bool
WindowsRead(Serial& serial, uint8_t* rx_data, uint32_t* rx_len) {
  serial_t& hSerial = serial.GetSerialPort();
  unsigned long recv_data = 0;//windows wants LPDWORD == unsignled long; LPWORD != uint32_t || uint16_t
  if (!ReadFile(hSerial.h_serial, rx_data, MAX_BUFFER_SIZE, &recv_data, NULL)) {
    //error reading file

    debug::Log::Message(debug::LOG_ERROR, "Failed to read serial.\n"); // This may print too much data might need to change. 
                                       // Im not sure if time out will return false for ReadFile
    return false;
  } else {
    if (recv_data > 0) {
			LOG_PRINTF(debug::LOG_ERROR, "**  Recieved\t Length: %d  **\n", recv_data);
      *rx_len = recv_data;
      return true;
    }
  }
  
  return false;//no error, no data recieved, connection timmed out, will retry next time.
}

inline bool
WindowsClose(Serial& serial) {
  serial_t& h_serial = serial.GetSerialPort();
  if (h_serial.serial_s == SERIAL_CLOSED && CloseHandle(h_serial.h_serial))
  {
    h_serial.serial_s = SERIAL_CLOSED;
    return true;
  }
  return false;
}

#else

#define UNIX_SERIAL 


inline bool
InitUnixSerial(Serial& serial, const char* port, uint32_t baudrate) {
  bool result = false;
  serial_t& h_serial = serial.GetSerialPort();

  LOG_PRINTF(debug::LOG_NOTIFY, "port: %s\n connecting...\n", port);
  h_serial.fd = open(port, (O_RDWR | O_NOCTTY));

  if (h_serial.fd == -1) {
		LOG_PRINTF(debug::LOG_ERROR, "port failed to open: err number %d\n", errno);
  } else {
    struct termios options;
    fcntl(h_serial.fd, F_SETFL, 0);
    tcgetattr(h_serial.fd, &options);
    memset(&options, 0, sizeof(options));
    speed_t speed = B115200;
    switch (baudrate) {
      case 0:      { speed = B0;      break; }
      case 50:     { speed = B50;     break; }
      case 75:     { speed = B75;     break; } 
      case 110:    { speed = B110;    break; }
      case 134:    { speed = B134;    break; }
      case 150:    { speed = B150;    break; }
      case 200:    { speed = B200;    break; }
      case 300:    { speed = B300;    break; }
      case 600:    { speed = B600;    break; }
      case 1200:   { speed = B1200;   break; }
      case 1800:   { speed = B1800;   break; }
      case 2400:   { speed = B2400;   break; }
      case 4800:   { speed = B4800;   break; }
      case 9600:   { speed = B9600;   break; }
      case 19200:  { speed = B19200;  break; }
      case 38400:  { speed = B38400;  break; }
      case 57600:  { speed = B57600;  break; }
      case 115200: { speed = B115200; break; }
    }
    
    cfsetispeed(&options, speed);
    cfsetospeed(&options, speed);

    // Since we are going with 8 bit size, ignoring parity...
    options.c_cflag |= (CLOCAL | CREAD | CS8);
    options.c_iflag = (IGNBRK | IGNPAR);
    options.c_oflag = 0;
    // timeout properties
    options.c_cc[VMIN] = 1;
    tcflush(h_serial.fd, TCIFLUSH);
    tcsetattr(h_serial.fd, TCSANOW, &options);

    result = true;
    debug::Log::Message(debug::LOG_NOTIFY, "Connected\n");
    h_serial.serial_s = SERIAL_CONNECTED;
  }

  return result;
}


inline bool
UnixSend(Serial& serial, uint8_t dest_id, uint8_t* tx_data, int32_t tx_length) {
  bool result = false;
  serial_t& h_serial = serial.GetSerialPort();

  int32_t bytes_written = write(h_serial.fd, tx_data, tx_length);
  if (bytes_written < 0) {
		debug::Log::Message(debug::LOG_ERROR, "write() has failed to send!\n");
  } else {
    LOG_PRINTF(debug::LOG_NOTIFY, "**  Sent\t Length: %d, Sent: %d, destID: %d **\n", tx_length, bytes_written, dest_id);
    result = true;
  }
  
  return result;
}


inline bool
UnixRead(Serial& serial, uint8_t* rx_data, uint32_t* rx_len) {
  bool result = false;
  serial_t& h_serial = serial.GetSerialPort();
	debug::Log::Message(debug::LOG_NOTIFY, "\n\nReading serial\n\n");
  int32_t bytes_read = read(h_serial.fd, rx_data, 256);
  if (bytes_read < 0) {
    LOG_PRINTF(debug::LOG_ERROR, "Failed to read from package. erro number %d\n", errno);
  } else {
    *rx_len = bytes_read;
    LOG_PRINTF(debug::LOG_NOTIFY, "**  Recieved\t Length: %d  **\n", bytes_read);
    result = true;
  }

  return result;
}

inline bool
UnixClose(Serial& serial) {
  serial_t& h_serial = serial.GetSerialPort();
  if (h_serial.serial_s != SERIAL_CLOSED) {
    if (close(h_serial.fd) > 0)
    {
      h_serial.serial_s = SERIAL_CLOSED;
      return true;
    }
  }
  return false;
}

#endif // COM_TARGET_OS == COM_OS_WINDOWS


inline bool
OpenPort(Serial& serial, const char* comport, uint32_t baudrate) {
#if defined WINDOWS_SERIAL
  return InitWindows(serial, comport, baudrate);
#elif defined UNIX_SERIAL
  return InitUnixSerial(serial, comport, baudrate);
#endif
}

inline bool
SendToPort(Serial& serial, uint8_t dest_id, uint8_t* tx_data, uint32_t tx_length) {
  serial_t& h_serial = serial.GetSerialPort();
  if (h_serial.serial_s == SERIAL_CONNECTED) {
#if defined WINDOWS_SERIAL
    return WindowsSend(serial, dest_id, tx_data, tx_length);
#elif defined UNIX_SERIAL
    return UnixSend(serial, dest_id, tx_data, tx_length);
#endif
  }

  return false;
}


inline bool
ReadFromPort(Serial& serial, uint8_t* rx_data, uint32_t* rx_len) {
  serial_t& h_serial = serial.GetSerialPort();
  if (h_serial.serial_s == SERIAL_CONNECTED) {
#if defined WINDOWS_SERIAL
    return WindowsRead(serial, rx_data, rx_len);
#elif defined UNIX_SERIAL
    return UnixRead(serial, rx_data, rx_len);
#endif
  }
  return false;
}


inline bool 
ClosePortHelper(Serial& serial) {
  serial_t& h_serial = serial.GetSerialPort();
#if defined WINDOWS_SERIAL
  WindowsClose(serial);
#elif defined UNIX_SERIAL
  UnixClose(serial);
#endif 
  return true;
}


/***********************************************/
/******************* Public  *******************/
/***********************************************/
Serial::Serial()
{		  
  connection_established = false;
  h_serial.serial_s = SERIAL_CLOSED;
  id = -1;
}

Serial::Serial(uint32_t id)
: id(id)
{
  connection_established = false;
  h_serial.serial_s = SERIAL_CLOSED;
}


Serial::~Serial() {  }


bool Serial::OpenConnection(const char* port, const char* address, uint32_t baudrate)
{ 
  //check os here
  connection_established = OpenPort(*this, port, baudrate);
  LOG_PRINTF(debug::LOG_NOTIFY, "Port is now: %d\n", h_serial.fd);
  return connection_established;
}


bool Serial::Send(uint8_t dest_id, uint8_t* tx_data, uint32_t tx_length)
{
  unsigned int crc = Crc32(tx_data, tx_length);
  uint8_t crc_data[CRC32_SIZE];
  Crc32ToArr(tx_data, tx_length, crc_data);
  parser.ParseSend(tx_data, tx_length, crc_data, buffer_send);//length adjusted
  return SendToPort(*this, dest_id, buffer_send, tx_length);
}


bool Serial::Recv(uint8_t* rx_data, uint32_t* rx_len) {
 
 bool valid = true;
 //Get new message if parser is done
 if (parser.ParseReceiveDone()){					
  valid = ReadFromPort(*this, buffer_recv, rx_len);
 }
 //parse data
 if (valid){
  valid = parser.ParseReceive(rx_data, *rx_len, buffer_recv);
  if (*rx_len > 0){
   unsigned int crc_recv = TruncateCrc32(rx_data, rx_len);
   unsigned int crc = Crc32(rx_data, *rx_len);
   return crc_recv == crc;
  }
  else{
   return false;
  }
 }		
 
  return valid;
}


serial_status Serial::GetStatus() {
  return h_serial.serial_s;
}


bool Serial::CloseSerialPort() {
  return ClosePortHelper(*this);
}


serial_t& Serial::GetSerialPort() {
  return h_serial;
}
} // namespace Network
} // namespace Comnet 
