#ifndef H_CNC_MASHINE_H
#define H_CNC_MASHINE_H


#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>
#include "cnc_error_codes.h"
#include "open62541/open62541.h"
#include "shared_memory.h"
#include "opcua_log.h"
#include "opcua_msgpack.h"

using namespace std;
#define SOFT_PLC_MAX_NAME_LEN_C 50
/**
 * @brief Структура описивающая образ входной/выходной переменной в OPC UA узле
 * 
 */
struct soft_plc_io_opcua_st{
  uint32_t index; //  
  uint32_t nodeIndx; // unicue index of node
  uint32_t object_type; // SoftPlcObjectTypes SPLC_OBJECT_TYPE_IN или SPLC_OBJECT_TYPE_OUT / SPLC_OBJECT_TYPE_IN_SYNCH или SPLC_OBJECT_TYPE_OUT_SYNCH
  int64_t address; // Address in CPM
  uint8_t bitIndex; // bit index for bit values
  //SoftPlcIoKind memKind; // the kind of memory in wich varialbe is paced // NOT USED YET!
  uint8_t  name[SOFT_PLC_MAX_NAME_LEN_C];
  uint32_t type;// SoftPlcIOTypes
  uint32_t length; // lenght of data
  int64_t* pIntValAddr; // pointer to int value
  double*  pDoubleValAddr; // pointer to doubele value
  };

class cnc_mashine
{
    shared_memory *m_shared_mem_onject = NULL;
    UA_DataSource m_opcua_handlers;
    std::vector<soft_plc_io_opcua_st> mp_io_container;
    public:
    cnc_mashine()
    {

    }
    int8_t fill_io_container()
    {
        std::ifstream file;
        file.open("opc_ua_shared_file.bin");
        if(!file.is_open())
        {
            UA_LogE("{0}",error_codes_names[CNC_NO_SHARED_FILE]);
            return CNC_NO_SHARED_FILE;
        }
            
        return 0;
    }
};

#endif //H_CNC_MASHINE_H