# CommProto jni wrapping.
cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(CommProtoJNI)


find_package(JNI)
if (JNI_FOUND)
  message(STATUS "JNI_INCLUDE_DIR=${JNI_INCLUDE_DIRS}")
  message(STATUS "JNI_LIBS=${JNI_LIBRARIES}")
else()
  message(FATAL "Java Native Interface was not found on this machine!")
endif()


message("Build Complete...")