cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project("pyCommProto")


# Set the python version for which to have cmake look for.
set(pyVersion)

if (DEFINED version)
  set(pyVersion ${version})
  set(Python_ADDITIONAL_VERSIONS ${pyVersion})
else()
  message(FATAL_ERROR "Please specify the python version you wish to build: e.g: cmake -Dversion=3.6 <path-to-cmake-file>")
endif()

find_package(PythonLibs REQUIRED)
message("Python build...")
message("${PYTHON_LIBRARY}")