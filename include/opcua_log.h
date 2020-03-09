/**
 * @file opcua_log.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-01-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef H_OPCUA_LOG_H
#define H_OPCUA_LOG_H

#include <memory>
#include <stdio.h>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace NC_Log_space
{
	class NC_log
	{
	public:
		static void init();
		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return ms_ua_connection_logger; }
	private:
		static std::shared_ptr<spdlog::logger> ms_ua_connection_logger;
	};
}
//#ifdef DEBUG_OPC_UA_SERVER
	#define UA_LogW(...) NC_Log_space::NC_log::GetLogger()->warn(__VA_ARGS__)
	#define UA_LogE(...) NC_Log_space::NC_log::GetLogger()->error(__VA_ARGS__)
	#define UA_LogI(...) NC_Log_space::NC_log::GetLogger()->info(__VA_ARGS__)
	#define UA_LogT(...) NC_Log_space::NC_log::GetLogger()->trace(__VA_ARGS__)
/*#else 
	#define UA_LogW(...)
	#define UA_LogE(...)
	#define UA_LogI(...)
	#define UA_LogT(...)
#endif*/

#endif //H_OPCUA_LOG_H