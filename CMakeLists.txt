cmake_minimum_required (VERSION 3.0.0)

project (OPCUA_Server C CXX)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
    SET(CMAKE_CXX_COMPILER "C:/MinGW/bin/g++")
    SET(CMAKE_C_COMPILER "C:/MinGW/bin/gcc")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  #  message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
endif()

set(SOURCE source/main.cpp)
add_executable (${PROJECT_NAME} ${SOURCE})