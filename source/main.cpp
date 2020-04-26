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
#include <signal.h>
#include "entry_point.h"
#include "opcua_log.h"
#include "open62541/open62541.h"
#include "opcua_msgpack.h"
#include "cnc_machine.h"
#include "open62541/open62541.h"

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

/*UA_Boolean running = true;
static void stopHandler(int sig) {
UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
running = false;
}
static UA_StatusCode readCurrentTime(void *handle, const UA_NodeId nodeid, UA_Boolean sourceTimeStamp,
    const UA_NumericRange *range, UA_DataValue *dataValue) {

    cout << "readed nodeid " << nodeid.identifier.numeric  << endl;

    UA_DateTime now = UA_DateTime_now();
    UA_Variant_setScalarCopy(&dataValue->value, &now,
    &UA_TYPES[UA_TYPES_DATETIME]);
    dataValue->hasValue = true;
    return UA_STATUSCODE_GOOD;
}
static UA_StatusCode
writeCurrentTime(void *handle, const UA_NodeId nodeid, const UA_Variant *data,
const UA_NumericRange *range) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
    "Changing the system time is not implemented");
    return UA_STATUSCODE_BADINTERNALERROR;
}
static void addCurrentTimeDataSourceVariable(UA_Server *server) {
    static UA_UInt32 s_nodeid = 1;
    UA_VariableAttributes attr;
    UA_VariableAttributes_init(&attr);
    attr.displayName = UA_LOCALIZEDTEXT("en_US", "Current time - data source");

    UA_NodeId currentNodeId = UA_NODEID_NUMERIC(1,s_nodeid);
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "current-time-datasource");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NULL;

    s_nodeid++;
    UA_DataSource timeDataSource;
    timeDataSource.handle = NULL;
    timeDataSource.read = readCurrentTime;
    timeDataSource.write = writeCurrentTime;

    UA_Server_addDataSourceVariableNode(server, currentNodeId, parentNodeId,
    parentReferenceNodeId, currentName,
    variableTypeNodeId, attr,
    timeDataSource, NULL);
  
}*/
int main(void) {

    SET_ENTRY_POINT
    cnc_machine proccess;

    return 0;
}