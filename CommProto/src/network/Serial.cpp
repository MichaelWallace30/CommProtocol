#include <CommProto/network/Serial.h>

/***********************************************/
/******************* Private *******************/
/***********************************************/
bool Serial::initWindows(std::string comPort, uint16_t baudrate)
{
	if (comPort[0] != '\\')
	{
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

	
	hSerial = CreateFile(comport,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

#ifdef UNICODE //clean up multybyte string to LPCWSTR
	delete str;
#endif

	if (hSerial == INVALID_HANDLE_VALUE)
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

	if (!GetCommState(hSerial, &dcbSerialParams))
	{
		//error getting state
		printf( "Error getting state\n");
		return false;
	}

	dcbSerialParams.BaudRate = CBR_19200;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	if (!SetCommState(hSerial, &dcbSerialParams))
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

	if (!SetCommTimeouts(hSerial, &timeouts))
	{
		//error occureeed
		printf( "Setting up times outs failed\n");
		return false;
	}

	printf( "Completed setting up time outs\n");

	return true;
}




/***********************************************/
/******************* Public  *******************/
/***********************************************/
Serial::Serial():CommsLink()
{		
	connectionEstablished = false;
}

Serial::~Serial() {  }

bool Serial::initConnection(std::string port, std::string address, uint32_t baudrate)
{ 

	//check os here
	connectionEstablished = initWindows(this->comPort, this->baudrate);

	return connectionEstablished;
}


bool Serial::send(uint8_t destID, uint8_t* txData, int32_t txLength)
{ 
	if (connectionEstablished)
	{
		unsigned long sentData = 0;//windows want LPDWORD == unsignled long != uint32_t || uint16_t
		if (!WriteFile(hSerial, txData, txLength, &sentData, NULL))
		{
			//error reading file
			printf("Failed to write serial\n");
			return false;
		}
		else
		{
			#ifdef SERIAL_DEBUG
			printf("**  Sent\t Length: %d, Sent: %d, destID: %d **\n", txLength, sentData, destID);
			#endif	
			return true;
		}		
	}
	return false;
}

bool Serial::recv(uint8_t* rx_data, uint32_t* rx_len)
{ 
	if (connectionEstablished)
	{

		unsigned long recvData = 0;//windows wants LPDWORD == unsignled long; LPWORD != uint32_t || uint16_t
		if (!ReadFile(hSerial, rx_data, MAX_BUFFER_SIZE, &recvData, NULL))
		{
			//error reading file
			printf("Failed to read serial\n");//this my print too much data might need to change. Im not sure if time out will return false for ReadFile
			return false;
		}
		else
		{
			#ifdef SERIAL_DEBUG
			printf("**  Recieved\t Length: %d  **\n", recvData);
			#endif	
			if (recvData > 0)
			{
				*rx_len = recvData;
				return true;
			}
		}
	}
	return false;//no error, no data recieved, connection timmed out, will retry next time.
}