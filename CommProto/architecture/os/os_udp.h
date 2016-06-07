#if (COM_TARGET_OS == COM_OS_WINDOWS)    
    #include <Windows.h>
    #pragma comment(lib,"ws2_32.lib")    
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <stdint.h>
    #include <string.h>
#endif