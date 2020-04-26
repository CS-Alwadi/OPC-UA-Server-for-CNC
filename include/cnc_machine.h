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

#define CMP_DBG_RW(condition) (condition == 1 ? "PASS\n" : "FALIED\n")

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
    int64_t index_in_softplc;
    int64_t address_in_cmp;
    char name[30] = {};
    uint32_t name_len = 30;
    uint8_t value_size;            /*< Размер перемены в памяти*/
    uint8_t* p_value = NULL;       /*< Адрес переменной*/
}cnc_node_t;


class cnc_machine
{
    /****************************************************** PRIVATE ************************************************************/
    private:
    shared_memory *m_shared_mem_onject = NULL;  //Объект общей памяти 
    std::vector<cnc_node_t> m_io_container;    //Контейнер содержащий адресса переменных CMP в буфера общей памяти

    uint32_t nodes_quantity = 0;                //количество перемнных CMP
    
    /*TIME variables*/
    uint32_t cnc_epoch_time = 0;                //Текушее время СЧПУ в стандарте epoch time
    uint32_t server_epoch_time = 0;             //Текущее время OPC UA сервера

    /* MSGPACK Handlers */
    struct msgpack_container_ctx msg_container; //Контейнер для хранение и обработки msgpack буфер
	struct cmp_ctx_s * msgpack_ctx = NULL;      //указатель на структуру msgpack 
    
    /* OPC UA Handlers */
    UA_Server * opcua_server = NULL;            /**< OPC UA Server */
    UA_ServerConfig opcua_config;               /**< */
    UA_ServerNetworkLayer opcua_network_layer;
    UA_DataSource m_opcua_handlers;             //Обработчик чтение и записи команд из OPC UA
    bool is_opcua_server_running = true;

    PVOID m_buffer = NULL;                      //буфер общей памяти
    

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
    uint32_t fill_io_container();

    

    void config_opcua_server();
    void init_opcua_network_layer();
    bool fill_opcua_address_space();

    /****************************************************** PUBLIC ************************************************************/
    public:
    cnc_machine()
    {
        UA_LogW("OPC UA MACHINE STARTED");
        m_io_container.reserve(100);

        UA_LogW("Shared memory get started");
        while (false == shmem_init())
            cout << "Tring again\n";

        UA_LogW("Filling in io container");
        fill_io_container();

        UA_LogW("Set OPC UA network layer");
        init_opcua_network_layer();

        UA_LogW("Config OPC UA Server");
        config_opcua_server();

        UA_LogW("Creating OPC UA address space");
        fill_opcua_address_space();
        UA_LogW("****************************** Startin ******************************");
        start_opcua_server();
    }
    void start_opcua_server()
    {
        UA_Server_run(opcua_server, &is_opcua_server_running);
        UA_Server_delete(opcua_server);
        opcua_network_layer.deleteMembers(&opcua_network_layer);
    }
    void show_io_objects()
    {
        if(m_io_container.empty())
        {
             cout << "empty\n";
            return;
        }
        
        for(uint8_t i = 0; i < m_io_container.size(); i++)
        {
            cout << "Name " << m_io_container[i].name << " Address = " << m_io_container[i].address_in_cmp ;//<< " Value = " << *m_io_container[i].p_value;
        }
    }
};

#endif //H_CNC_MACHINE_H