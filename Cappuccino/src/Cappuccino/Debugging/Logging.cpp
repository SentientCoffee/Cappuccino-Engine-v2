#include "CappPCH.h"
#include "Cappuccino/Debugging/Logging.h"

#include <spdlog/sinks/stdout_color_sinks.h>

using namespace Capp;

std::shared_ptr<spdlog::logger> Logger::_engineLogger = nullptr;
std::shared_ptr<spdlog::logger> Logger::_clientLogger = nullptr;

void Logger::init() {
	spdlog::set_pattern("%^[%n] %v%$");
	_engineLogger = spdlog::stdout_color_mt("CAPPUCCINO");
	_engineLogger->set_level(spdlog::level::trace);
	_clientLogger = spdlog::stdout_color_mt("CLIENT");
	_clientLogger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger> Logger::getEngineLogger() { return _engineLogger; }
std::shared_ptr<spdlog::logger> Logger::getClientLogger() { return _clientLogger; }
