#include <CommProto/network/Serial.h>

/***********************************************/
/******************* Private *******************/
/***********************************************/
#if COM_TARGET_OS == COM_OS_WINDOWS

#define WINDOWS_SERIAL

bool Serial::initWindows(std::string comPort, uint32_t baudrate)
{
  if (comPort[0] != '\\') {
      // "\\\\.\\"  allows windows ports above 9
      comPort = "\\\\.\\" + comPort;
  } 

#ifdef UNICODE	//convert multybyte string to LPCWSTR
  wchar_t* str = new wchar_t[comPortInput.length() * 2];
  MultiByteToWideChar(CP_ACP, 0, comPortInput.c_str(), -1, str, comPortInput.length() * 2);
  LPCWSTR comport = str;
#else
  //open up serial port  
  const char* comport =  comPort.c_str();
#endif
  hSerial.h_serial = CreateFile(comport,
				GENERIC_READ | GENERIC_WRITE,
				0,
				0,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				0);

#ifdef UNICODE //clean up multybyte string to LPCWSTR
  delete str;
#endif
  
  if (hSerial.h_serial == INVALID_HANDLE_VALUE)
    {
      if (GetLastError() == ERROR_FILE_NOT_FOUND)
	{
	  //serial port does not exist.
	  printf( "comport not found\n");
	  return false;
	}
      // some other error occured.
      printf( "Unknown error\n");
      return false;
    }
  else
    {
      
      printf( "Serial port created\n");
    }
  
  
  //setup parameters
  
  DCB dcbSerialParams = { 0 };
  
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
  
  if (!GetCommState(hSerial.h_serial, &dcbSerialParams))
    {
      //error getting state
      printf( "Error getting state\n");
      return false;
    }
  
  dcbSerialParams.BaudRate = (DWORD)baudrate;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;
  
  if (!SetCommState(hSerial.h_serial, &dcbSerialParams))
    {
      //error setting serial port state
      printf( "Error setting serial port state\n");
      return false;
    }
  
  
  printf( "Completed setting serial port state\n");
  

  //time out code not needed?
  
  COMMTIMEOUTS timeouts = { 0 };
  
  timeouts.ReadIntervalTimeout = 50;
  timeouts.ReadTotalTimeoutConstant = 50;
  timeouts.ReadTotalTimeoutMultiplier = 10;
  timeouts.WriteTotalTimeoutConstant = 50;
  timeouts.WriteTotalTimeoutMultiplier = 10;
  
  if (!SetCommTimeouts(hSerial.h_serial, &timeouts))
    {
      //error occureeed
      printf( "Setting up times outs failed\n");
      return false;
    }
  
  printf( "Completed setting up time outs\n");
  hSerial.serial_s = SERIAL_CONNECTED;
  return true;
}

inline bool
Serial::windowsSend(uint8_t destID, uint8_t* txData, int32_t txLength) {
  unsigned long sentData = 0;//windows want LPDWORD == unsignled long != uint32_t || uint16_t
  if (!WriteFile(hSerial.h_serial, txData, txLength, &sentData, NULL)) {
    //error reading file
    printf("Failed to write serial\n");
    return false;
  }
  else {
#ifdef SERIAL_DEBUG
    printf("**  Sent\t Length: %d, Sent: %d, destID: %d **\n", txLength, sentData, destID);
#endif	
    return true;
  }		

  return false;
}

inline bool
Serial::windowsRead(uint8_t* rx_data, uint32_t* rx_len) {    
  unsigned long recvData = 0;//windows wants LPDWORD == unsignled long; LPWORD != uint32_t || uint16_t
  if (!ReadFile(hSerial.h_serial, rx_data, MAX_BUFFER_SIZE, &recvData, NULL)) {
    //error reading file
    printf("Failed to read serial\n"); // This may print too much data might need to change. 
                                       // Im not sure if time out will return false for ReadFile
    return false;
  } else {
#ifdef SERIAL_DEBUG
    printf("**  Recieved\t Length: %d  **\n", recvData);
#endif	
    if (recvData > 0) {
      *rx_len = recvData;
      return true;
    }
  }
  
  return false;//no error, no data recieved, connection timmed out, will retry next time.
}

#else 

#define UNIX_SERIAL 

inline bool
Serial::initUnixSerial(const char* port, uint32_t baudrate) {
  bool result = false;
  printf("port: %s\n connecting...\n", port);
  hSerial.fd = open(port, (O_RDWR | O_NOCTTY));

  if (hSerial.fd == -1) {
    printf("port failed to open: err number %d\n", errno);
  } else {
    struct termios options;
    fcntl(hSerial.fd, F_SETFL, 0);
    tcgetattr(hSerial.fd, &options);
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
    tcflush(hSerial.fd, TCIFLUSH);
    tcsetattr(hSerial.fd, TCSANOW, &options);

    result = true;
    printf("Connected\n");
  }

  return result;
}

inline bool
Serial::unixSend(uint8_t destID, uint8_t* txData, int32_t txLength) {
  bool result = false;
  
  int32_t bytesWritten = write(hSerial.fd, txData, txLength);
  if (bytesWritten < 0) {
    printf("write() has failed to send!\n");
  } else {
#ifdef SERIAL_DEBUG
      printf("**  Sent\t Length: %d, Sent: %d, destID: %d **\n", txLength, bytesWritten, destID);
#endif	
    result = true;
  }
  
  return result;
}

inline bool
Serial::unixRead(uint8_t* rx_data, uint32_t* rx_len) {
  bool result = false;
#ifdef SERIAL_DEBUG
  printf("\n\nReading serial\n\n");
#endif
  int32_t bytesRead = read(hSerial.fd, rx_data, 256);
  if (bytesRead < 0) {
    printf("Failed to read from package. erro number %d\n", errno);
  } else {
    *rx_len = bytesRead;
#ifdef SERIAL_DEBUG
    printf("**  Recieved\t Length: %d  **\n", bytesRead);
#endif	    
    result = true;
  }

  return result;
}

#endif // COM_TARGET_OS == COM_OS_WINDOWS

inline bool
Serial::openPort(std::string comPort, uint32_t baudrate) {
#if defined WINDOWS_SERIAL
  return initWindows(comPort, baudrate);
#elif defined UNIX_SERIAL
  return initUnixSerial(comPort.c_str(), baudrate);
#endif
}

inline bool
Serial::sendToPort(uint8_t destID, uint8_t* txData, uint32_t txLength) {
  if (hSerial.serial_s == SERIAL_CONNECTED) {
#if defined WINDOWS_SERIAL
    return windowsSend(destID, txData, txLength);
#elif defined UNIX_SERIAL
    return unixSend(destID, txData, txLength);
#endif
  }

  return false;
}

inline bool
Serial::readFromPort(uint8_t* rx_data, uint32_t* rx_len) {
  if (hSerial.serial_s == SERIAL_CONNECTED) {
#if defined WINDOWS_SERIAL
    return windowsRead(rx_data, rx_len);
#elif defined UNIX_SERIAL
    return unixRead(rx_data, rx_len);
#endif
  }
  return false;
}


/***********************************************/
/******************* Public  *******************/
/***********************************************/
Serial::Serial():CommsLink()
{		
  connectionEstablished = false;
  hSerial.serial_s = SERIAL_OPEN;
}

Serial::~Serial() {  }

bool Serial::initConnection(std::string port, std::string address, uint32_t baudrate)
{ 
  
  //check os here
  connectionEstablished = openPort(port,baudrate);
  hSerial.serial_s = SERIAL_CONNECTED;
  
  return connectionEstablished;
}

bool Serial::send(uint8_t destID, uint8_t* txData, int32_t txLength)
{ 
  return sendToPort(destID, txData, txLength);
}

bool Serial::recv(uint8_t* rx_data, uint32_t* rx_len)
{ 
  return readFromPort(rx_data, rx_len);
}
