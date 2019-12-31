cmake_minimum_required (VERSION 3.0.0)

project (OPCUA_Server C CXX)
SET(CMAKE_CXX_COMPILER "C:/MinGW/bin/g++")
SET(CMAKE_C_COMPILER "C:/MinGW/bin/gcc")
set(SOURCE source/main.cpp)
add_executable (${PROJECT_NAME} ${SOURCE})