/**
 * @file main.cpp
 * @author Al-Wadi (omar.alwadi@outlook.com)
 * @brief This is main file
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "entry_point.h"
#include "opcua_log.h"
#include "open62541/open62541.h"
#include "opcua_msgpack.h"
#include "cnc_machine.h"

using namespace std;
#define MARKER_SIZE 1
uint8_t * shm_address[5];
#define BIG_ENDIAN_TO_LITTILE_U32(var) var = ((var>>24)&0xff) | ((var<<8)&0xff0000) | ((var>>8)&0xff00) | ((var<<24)&0xff000000); 
  
uint16_t write_variable(soft_plc_io_opcua_st var,uint8_t *msg_buffer,uint16_t size)
{
    msgpack_container_ctx msg_container;
    uint16_t data_cnt = 0;
	cmp_ctx_s * msgpack_ctx = opcua_cmp_init(&msg_container,msg_buffer,size);
    cmp_write_u32(msgpack_ctx,var.index);
    shm_address[0] = &msg_buffer[data_cnt + MARKER_SIZE];
    data_cnt += opcua_cmp_msg_len(&msg_container);

    cmp_write_u32(msgpack_ctx,var.object_type);
    shm_address[1] = &msg_buffer[data_cnt + MARKER_SIZE];
    data_cnt += opcua_cmp_msg_len(&msg_container);

    cmp_write_s64(msgpack_ctx,var.address);

    shm_address[2] = &msg_buffer[data_cnt + MARKER_SIZE];
    data_cnt += opcua_cmp_msg_len(&msg_container);

    cmp_write_str32(msgpack_ctx,(char*)var.name,var.length);
    return opcua_cmp_msg_len(&msg_container);
}

int main(void) {
     
    SET_ENTRY_POINT
    cnc_machine machine;
    return 0;
}