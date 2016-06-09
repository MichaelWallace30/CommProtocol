/**
   Operating System Architecture Check.
 */
#ifndef __ARCH_H
#define __ARCH_H

// define the target architecture to determine which operating system will be compiled.
#define COM_OS_UNKNOWN        0
#define COM_OS_WINDOWS        1
#define COM_OS_LINUX          2
#define COM_OS_APPLE          3
#define COM_OS_QT             4

// determine our target platform that is going to be used for the enviroment.
#define COM_TARGET_OS         COM_OS_UNKNOWN

// WIN32 operating systems: Windows.

#if defined( _WIN32)
 #undef COM_TARGET_OS
 #define COM_TARGET_OS        COM_OS_WINDOWS
#endif // _WIN32 && WINDOWS

// Linux
#if defined(__linux__) && !defined(__APPLE__)
 #undef COM_TARGET_OS
 #define COM_TARGET_OS        COM_OS_LINUX
#endif // __linux__

// Apple : OSX
#if defined(__APPLE__) && defined(__MACH__)
 #undef COM_TARGET_OS
 #define COM_TARGET_OS        COM_OS_APPLE
#endif // __APPLE__ && __MACH__


//---------------------------------------------------
// configuring the target platform.
//---------------------------------------------------
#if !(COM_TARGET_OS)
 #error "Operating system is not known. Is it possible you are targeting an unknown platform?"
#endif

// Allow for specializations in what ever operating system we are building in.

#endif // __ARCH_H
