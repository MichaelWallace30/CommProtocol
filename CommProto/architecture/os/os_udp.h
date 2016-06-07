#if (COM_TARGET_OS == COM_OS_WINDOWS)    
    #include <Windows.h>	
    #pragma comment(lib,"ws2_32.lib")
	#define s_addr  S_un.S_addr 
	#define closSocket(x)	closesocket(x); WSACleanup()
#else    
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>    
    #include <string.h>
	#include <arpa/net.h>
	#define closSocket(x)	close(x)
#endif

