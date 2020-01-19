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
#include "cnc_mashine.h"

using namespace std;

int main(void) {
    SET_ENTRY_POINT
    cnc_mashine cnc_obj;
    cnc_obj.fill_io_container();
    
   
    return 0;
}