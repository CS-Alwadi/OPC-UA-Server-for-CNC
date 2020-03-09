#ifndef H_CNC_MACHINE_H
#define H_CNC_MACHINE_H


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

typedef struct cnc_node_st{
    uint8_t value_size;            /*< Размер перемены в памяти*/
    uint8_t* address_value = NULL; /*< Адрес переменной*/
}cnc_node_t;


class cnc_machine
{
    shared_memory *m_shared_mem_onject = NULL;  //Объект общей памяти 
    UA_DataSource m_opcua_handlers;             //Обработчик чтение и записи команд из OPC UA
    std::vector<cnc_node_t> mp_io_container;    //Контейнер содержащий адресса переменных CMP в буфера общей памяти
    PVOID m_buffer = NULL;                      //буфер общей памяти
    uint32_t nodes_quantity = 0;                //количество перемнных CMP
    struct msgpack_container_ctx msg_container; //Контейнер для хранение и обработки msgpack буфер
	struct cmp_ctx_s * msgpack_ctx = NULL;      //указатель на структуру msgpack 
    uint32_t cnc_epoch_time = 0;                //Текушее время СЧПУ в стандарте epoch time
    uint32_t server_epoch_time = 0;             //Текущее время OPC UA сервера

    private:
    void create_opc_nodes();

    public:
    cnc_machine()
    {
        UA_LogI("cnc_mashine");
        mp_io_container.reserve(100);
    }

    /**
     * @brief Иницализация работы структур для работы с общей памяти
     * 
     * @return true - успешно
     * @return false - не успешно
     */
    bool shmem_init();

    /**
     * @brief 
     * 
     * @return int8_t 
     */
    int8_t fill_io_container();

    /**
     * @brief 
     * 
     * @param handle 
     * @param nodeid 
     * @param includeSourceTimeStamp 
     * @param range 
     * @param value 
     * @return UA_StatusCode 
     */
    UA_StatusCode read_node_from_cnc(void *handle, const UA_NodeId nodeid,
                          UA_Boolean includeSourceTimeStamp,
                          const UA_NumericRange *range, UA_DataValue *value);

    /**
     * @brief  
     * 
     * @param handle 
     * @param nodeid 
     * @param data 
     * @param range 
     * @return UA_StatusCode 
     */
    UA_StatusCode write_node_to_cnc(void *handle, const UA_NodeId nodeid,
                           const UA_Variant *data, const UA_NumericRange *range);
};

#endif //H_CNC_MACHINE_H