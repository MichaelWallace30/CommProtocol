# CommProtocol version 1.0.7b
Git hub to work on new Comm Protocol design.

## Design Goals
- Powerful
- Robust
- Adaptable

This Com Protocol should be able to handle data dynamically, without needing to be hardcoded in. This will allow programmers more flexibility to change and adapt based on the agreement from both ends of the link, without too much work on the library itself. It should be able to allow programmers to define what is needed to be sent between both parties, so that the maintainers of this library have room to optimize the library itself without having to figure out what needs to be sent.
  
Current Beta Features:
- Cross-platform library (Linux/POSIX/Windows)
- Supports Loose Coupling.
- Allows connections via Serial, UDP, and Zigbee.
- C# Wrapper
- Designed in an Object Oriented manner to make it easier to use the library.  
  
Features currently implementing:  
- TCP implementation  
- AES Encryptions for protocol.  
- JNI Wrapping for communications use.  
- Network Automation
- Publish/Subscribe designing
  
## Setting Up CommProtocol
Building CommProtocol should be relatively easy, you will need the following tools in order to make your life easier:
  
- CMake (At least 2.8 and above)
  
Simply download this repository (Clone if you will), navigate to the directory/folder of where you cloned this repository, and create a new folder called "build." inside this build directory, you will open a command prompt (or bash shell) inside and use the following command:

```
cmake -Dtest=ON ../
```

Which will execute the cmake build for CommProtocol. When finished, you will either see the .sln project of NGCP-Comnet-v2 (on Windows) OR a makefile for Linux.  
If you are on Windows, simply open the .sln file which will hold all files and projects for CommProtocol, and compile from the ALL_BUILDS project.  
If you are on Linux, simply call inside the current directory (assuming you are still in "build" directory):
```
make
```

Which will compile all the projects for CommProtocol.  
## Design Analysis
Coming soon...
