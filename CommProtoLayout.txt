Michael W.

All of these files are structured in a certain way, so as to organize the library architecture, here is the layout:


-------------------------------------------------------------------------------------
| Directory    | Purpose                                                 | Language |
-------------------------------------------------------------------------------------
| architecture | Intended to hold all files used for operating system    | C/C++    |
|              | checking, configuration and anything OS specific that   |          |
|              | are not necessary for dealing with in the main src.     |          |
|              | This directory will hold most of the architecture of    |          |
|              | the Communications Protocol.                            |          |
-------------------------------------------------------------------------------------
| lib          | Used for holding any libraries we may use (ex. Crypto++ | C/C++    |
|              | , xbee, etc.).                                          |          |
-------------------------------------------------------------------------------------
| tools        | Contains custom made data structures, certain generic   | C/C++    |
|              | allocators, possibly some algorithms and what not. We   |          |
|              | Will most likely not need most of this crap, but just   |          |
|              | the data structures.                                    |          |
-------------------------------------------------------------------------------------
| CommProto    | The main course of our library. Intended to hold all    | C++      |
|              | files and pipelines regarding the use of our library.   |          |
|              | Most of the important stuff is going in here, such as   |          |
|              | packing, marshalling, sending data, receiving data,     |          |
|              | encrypting, etc.                                        |          |
-------------------------------------------------------------------------------------
| build        | Will hold our .lib/.a files or .dll/.so files for use   | Machine  |
-------------------------------------------------------------------------------------

target: Since UGV is currently using the oldest compiler, we will try to stick to using C++03 features, and as few
        C++11 syntax as possible. We can write C++11-14 syntax, but will need to do too much work using CMake
        to set up our source for compilation on older compilers.

Should things go right, we shouldn't need to create any scripts (unless we decide to create an installer..) 
to generate any more messages, only have the separate platforms do it instead, our job is mainly to create the protocol, 
have it be sent securely and properly so that GCS and all other platforms can understand it on their end, and optimize the library so that it may run efficiently. 

Architecture Layout:

For the Architecture layout, we are following your idea:

------------------------------------------------------------------------------------------
|                                  Application Layer                                     |
------------------------------------------------------------------------------------------
|                                    Process Layer                                       |
------------------------------------------------------------------------------------------
|                                 Communication layer                                    |
------------------------------------------------------------------------------------------

Application layer   : The layer intended for user to communicate to our library, telling it what they want to send and expect to receive.
Process Layer       : The layer intended to process information and pack it for sending out.
Communication Layer : The layer intended to send out the information and receive. This will be our lowest layer, which will do a good amount of the work. 


 
