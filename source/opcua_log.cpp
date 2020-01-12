#include "opcua_log.h"

namespace NC_Log_space
{
	std::shared_ptr<spdlog::logger> NC_log::ms_ua_connection_logger;
	void NC_log::init()
	{
		spdlog::set_pattern("[%T %o ms] [%^%n%$]: %v");
		ms_ua_connection_logger = spdlog::stdout_color_mt("OPC UA CNC");
		ms_ua_connection_logger->set_level(spdlog::level::trace);
	}
}
