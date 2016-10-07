# Sets the macro for standard build options
#
if (CMAKE_VERSION VERSION_GREATER 3.0)
  macro(std_compiler_options lib)
    # Add some commands to the compiler.
    if (CXX_COMPILER MATCHES "MSVC")
      target_compile_options(${lib} PUBLIC /sdl-)
    else()
      target_compile_options(${lib} PUBLIC -std=c++11 -pthread)
    endif()
  endmacro()
else()
  macro(std_compiler_options lib)
    # do nothing.
  endmacro()
endif()

# macro intended on setting global scale compilation.
macro(check_global_compiler_options)
  if (CMAKE_VERSION LESS 3.0)
    if (CXX_COMPILER MATCHES "MSVC")
      set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " ${COMPILER_BUILD} /sdl-")
    else()
      set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " ${COMPILER_BUILD} -std=c++11 -pthread")
    endif()
  endif()
endmacro()