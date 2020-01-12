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
#include "cmp/cmp.h"
#include "entry_point.h"
#include "opcua_log.h"
#include "shared_memory.h"

using namespace std;

int main(void) {
    SET_ENTRY_POINT
    UA_LogI("Welcome to OPC UA server!");
    SharedMemory SM_Object(ShMemType::Client);
	uint8_t calulationBuffer[SH_MEM_BUF_SIZE];
	while (true)
	{
		 // SM_Object.ReadMemory(calulationBuffer, SH_MEM_BUF_SIZE);

		 // Do some calculation with the memory (calulationBuffer) then write it back to the shared memory 
		 
		 // SM_Object.WriteMemory(calulationBuffer, SH_MEM_BUF_SIZE);
	}
    return 0;
}