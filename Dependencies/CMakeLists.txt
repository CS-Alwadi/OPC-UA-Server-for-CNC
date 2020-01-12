message("CMAKE_CURRENT_SOURCE_DIR -> ${CMAKE_CURRENT_SOURCE_DIR}")

#Генерация библиотеки cmp (C - MSGPACK) из исходников
message(STATUS "Creating cmp lib")
FILE(GLOB_RECURSE CMP_HEADER_LIST "${CMAKE_CURRENT_SOURCE_DIR}/cmp*.h")     
FILE(GLOB_RECURSE CMP_SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/cmp*.c")
add_library(cmp ${CMP_HEADER_LIST} ${CMP_SRC_LIST})
target_include_directories(cmp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#Генерация библиотеки spdlog (C++ logging) из исходников
message(STATUS "Creating cmp spdlog")
FILE(GLOB_RECURSE SPDLOG_HEADER_LIST "${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include/spdlog/**.h")     
FILE(GLOB_RECURSE SPDLOG_SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/spdlog/src/*.cpp")
add_library(spdlog ${SPDLOG_HEADER_LIST} ${SPDLOG_SRC_LIST})
target_compile_definitions(spdlog PUBLIC SPDLOG_COMPILED_LIB)
target_include_directories(spdlog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)


#Сборка open62514
#message(STATUS "Creating open62541 lib")
#FILE(GLOB_RECURSE OPCUA_HEADER_LIST "${CMAKE_CURRENT_SOURCE_DIR}/open62541*.h")     
#FILE(GLOB_RECURSE OPCUA_SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/open62541*.c")
#add_library(open62541 ${OPCUA_HEADER_LIST} ${OPCUA_SRC_LIST})
#target_include_directories(open62541 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})