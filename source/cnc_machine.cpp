#include "cnc_machine.h"
static std::vector<cnc_node_t> * p_io_container = NULL;

char error_codes_names[MAX_ERROR_CODES_SIZE + 1][sizeof("CNC_ADDRESS_SPACE_NOT_INIT")] =
    {
        {[CNC_OK] = "CNC_OK"},
        {[CNC_ADDRESS_SPACE_NOT_INIT] = "CNC_ADDRESS_SPACE_NOT_INIT"},
        {[CNC_MASHIN_STOPED] = "CNC_MASHIN_STOPED"},
        {[CNC_NO_SHARED_FILE] = "CNC_NO_SHARED_FILE"},
    };

static UA_StatusCode read_node_callback(void *handle, const UA_NodeId nodeid,
                          UA_Boolean includeSourceTimeStamp,
                          const UA_NumericRange *range, UA_DataValue *value)
{

    for(uint8_t i = 0; i < p_io_container[0].size();i++)
    {
        if(p_io_container[0][i].index_in_softplc == nodeid.identifier.numeric)
        {
            static int64_t temp_value = 0;
            static struct msgpack_container_ctx msg_container;
            struct cmp_ctx_s * msgpack_ctx =  opcua_cmp_init(&msg_container,p_io_container[0][i].p_value,15);
            UA_LogI("---- Reading value of variable with index {0} {1}", p_io_container[0][i].index_in_softplc , CMP_DBG_RW(cmp_read_s64(msgpack_ctx,&temp_value)) );
            UA_Variant_setScalarCopy(&value->value, &temp_value, &UA_TYPES[UA_TYPES_INT64]);
            value->hasValue = true;
            return UA_STATUSCODE_GOOD;
        }
    }
    
    return UA_STATUSCODE_BADUNEXPECTEDERROR;
}

static UA_StatusCode write_node_callback(void *handle, const UA_NodeId nodeid,
                           const UA_Variant *data, const UA_NumericRange *range)
{
    return UA_STATUSCODE_GOOD;
}   
bool cnc_machine::shmem_init()
{
    m_shared_mem_onject = new shared_memory(Client);
    if(false == m_shared_mem_onject->is_init())
    {
        delete m_shared_mem_onject;
        m_shared_mem_onject = NULL;
        return false;
    }
    msgpack_ctx = opcua_cmp_init(&msg_container,(uint8_t*)m_shared_mem_onject->get_pshared_memory(),m_shared_mem_onject->GetSizeOfView());
    return true;
}

uint32_t cnc_machine::fill_io_container()
{
    cout << "fill_io_container()\n";
    cout << "Read array value:" << CMP_DBG_RW(cmp_read_array(msgpack_ctx,&nodes_quantity));

    cnc_node_t temp_softplc_node;
    int64_t temp_value;
    uint8_t *  temp_pvalue;

    for (uint32_t i = 0; i < nodes_quantity; i++)
    {
        cout << "-- Read IO index [ " << i << " ] in memory\n";
        cout << "---- Reading index in SoftPLC " <<  CMP_DBG_RW(cmp_read_integer(msgpack_ctx,&temp_softplc_node.index_in_softplc));
        cout << "---- Reading address in CMP " <<  CMP_DBG_RW(cmp_read_integer(msgpack_ctx,&temp_softplc_node.address_in_cmp));
        cout << "---- Reading name of variable " << CMP_DBG_RW(cmp_read_str(msgpack_ctx,temp_softplc_node.name,&temp_softplc_node.name_len));
        temp_softplc_node.p_value =  msg_container.p_cmp_write;
        cout << "---- Reading value of variable " << CMP_DBG_RW(cmp_read_s64(msgpack_ctx,&temp_value));
        cout << "---- Value = " << temp_value << endl;
        m_io_container.push_back(temp_softplc_node);
        temp_softplc_node.name_len = 32;
    }
    p_io_container = &m_io_container;
    return 0;
}

void cnc_machine::config_opcua_server()
{
    opcua_config = UA_ServerConfig_standard;
    opcua_config.networkLayers = &opcua_network_layer;
    opcua_config.networkLayersSize = 1;
    opcua_server = UA_Server_new(opcua_config);
}
void cnc_machine::init_opcua_network_layer()
{
    opcua_network_layer = UA_ServerNetworkLayerTCP(UA_ConnectionConfig_standard, 4840);
    cout << "Setted port 4840\n";
}

bool cnc_machine::fill_opcua_address_space()
{
    
    if(m_io_container.empty())
        return false;

    cout << "fill_opcua_address_space \n";
    for(uint32_t i = 0; i < m_io_container.size(); i++)
    {
        cout << "io name [ " << m_io_container[i].name << " ] index = " << m_io_container[i].index_in_softplc << endl;
        UA_VariableAttributes attr;
        UA_VariableAttributes_init(&attr);
        attr.displayName = UA_LOCALIZEDTEXT("en_US", (char *)m_io_container[i].name);

        UA_NodeId currentNodeId = UA_NODEID_NUMERIC(1,m_io_container[i].index_in_softplc);
        UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, (char *)m_io_container[i].name);
        UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
        UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
        UA_NodeId variableTypeNodeId = UA_NODEID_NULL;

    
        UA_DataSource timeDataSource;
        timeDataSource.handle = NULL;
        timeDataSource.read = read_node_callback;
        timeDataSource.write = write_node_callback;

        UA_Server_addDataSourceVariableNode(opcua_server, 
        currentNodeId, 
        parentNodeId,
        parentReferenceNodeId,
        currentName,
        variableTypeNodeId,
        attr,
        timeDataSource,
        NULL);
    }

    return true;
    
}