/**
 * @file main.cpp
 * @author Al-Wadi (omar.alwadi@outlook.com)
 * @brief This is main file
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include <signal.h>

/*#include "cmp/cmp.h"
#include "opcua_msgpack.h"
#include "entry_point.h"
#include "opcua_log.h"
#include "shared_memory.h"
*/
#include "open62541/open62541.h"
using namespace std;
UA_Boolean running = true;
static void stopHandler(int sig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "received ctrl-c");
    running = false;
}

int main(void) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_ServerConfig config = UA_ServerConfig_standard;
    UA_ServerNetworkLayer nl =
        UA_ServerNetworkLayerTCP(UA_ConnectionConfig_standard, 16666);
    config.networkLayers = &nl;
    config.networkLayersSize = 1;
    UA_Server *server = UA_Server_new(config);

    UA_Server_run(server, &running);

    UA_Server_delete(server);
    nl.deleteMembers(&nl);
    
    return 0;
}