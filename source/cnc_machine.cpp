#include "cnc_machine.h"

char error_codes_names[MAX_ERROR_CODES_SIZE + 1][sizeof("CNC_ADDRESS_SPACE_NOT_INIT")] =
    {
        {[CNC_OK] = "CNC_OK"},
        {[CNC_ADDRESS_SPACE_NOT_INIT] = "CNC_ADDRESS_SPACE_NOT_INIT"},
        {[CNC_MASHIN_STOPED] = "CNC_MASHIN_STOPED"},
        {[CNC_NO_SHARED_FILE] = "CNC_NO_SHARED_FILE"},
    };

bool cnc_machine::shmem_init()
{
    this->m_shared_mem_onject = new shared_memory(Server);
    if(this->m_shared_mem_onject->is_init())
    {
        this->msgpack_ctx = opcua_cmp_init(&this->msg_container,(uint8_t*)this->m_shared_mem_onject->get_shmem_pointer(),1000);
        return true;
    }
    delete m_shared_mem_onject;
    return false;
}

int8_t cnc_machine::fill_io_container()
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
void cnc_machine::create_opc_nodes()
{
    for(uint32_t i = 0; i < this->nodes_quantity;i++)
        {
            
        }
}