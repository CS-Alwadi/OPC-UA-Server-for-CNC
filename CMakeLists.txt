cmake_minimum_required (VERSION 3.0.0 )
message("CMAKE_CURRENT_SOURCE_DIR -> ${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DDEBUG_OPC_UA_SERVER)
add_definitions(-D_WIN_32_)

project (OPCUA_Server LANGUAGES C CXX)
# Cross-compile 
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
    SET(CMAKE_CXX_COMPILER "C:/cygwin64/bin/g++")
    SET(CMAKE_C_COMPILER "C:/cygwin64/bin/gcc")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
endif()

add_subdirectory(Dependencies)
add_subdirectory(source)
