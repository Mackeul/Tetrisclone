#include "Log.h"
//#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

std::shared_ptr<spdlog::logger> Log::s_Logger;

void Log::Init() {

	spdlog::set_pattern("%^[%T] %n: %v%$");

	//s_Logger = spdlog::stdout_color_mt("LOG");
	s_Logger = spdlog::basic_logger_mt("GAME", "game.log");
	s_Logger->set_level(spdlog::level::trace);

	LOG_INFO("Log system initialized...");
}
