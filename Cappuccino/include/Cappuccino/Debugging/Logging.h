#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Capp {

	class Logger {
	public:

		static void init();

		static std::shared_ptr<spdlog::logger> getEngineLogger();
		static std::shared_ptr<spdlog::logger> getClientLogger();

	private:

		static std::shared_ptr<spdlog::logger> _engineLogger;
		static std::shared_ptr<spdlog::logger> _clientLogger;
		
	};
	
}

#if CAPP_DEBUG

// Engine log macros
#define CAPP_PRINT_CRITICAL(message, ...)	::Capp::Logger::getEngineLogger()->critical(message, ##__VA_ARGS__);
#define CAPP_PRINT_ERROR(message, ...)		::Capp::Logger::getEngineLogger()->error(message, ##__VA_ARGS__);
#define CAPP_PRINT_WARNING(message, ...)	::Capp::Logger::getEngineLogger()->warn(message, ##__VA_ARGS__);
#define CAPP_PRINT_INFO(message, ...)		::Capp::Logger::getEngineLogger()->info(message, ##__VA_ARGS__);
#define CAPP_PRINT(message, ...)			::Capp::Logger::getEngineLogger()->trace(message, ##__VA_ARGS__);


// Client log macros 
#define DEBUG_PRINT_CRITICAL(message, ...)	::Capp::Logger::getClientLogger()->critical(message, ##__VA_ARGS__);
#define DEBUG_PRINT_ERROR(message, ...)		::Capp::Logger::getClientLogger()->error(message, ##__VA_ARGS__);
#define DEBUG_PRINT_WARNING(message, ...)	::Capp::Logger::getClientLogger()->warn(message, ##__VA_ARGS__);
#define DEBUG_PRINT_INFO(message, ...)		::Capp::Logger::getClientLogger()->info(message, ##__VA_ARGS__);
#define DEBUG_PRINT(message, ...)			::Capp::Logger::getClientLogger()->trace(message, ##__VA_ARGS__);

#else

#define CAPP_PRINT_CRITICAL(message, ...)
#define CAPP_PRINT_ERROR(message, ...)
#define CAPP_PRINT_WARNING(message, ...)
#define CAPP_PRINT_INFO(message, ...)
#define CAPP_PRINT(message, ...)

// Client log macros 
#define DEBUG_PRINT_CRITICAL(message, ...)
#define DEBUG_PRINT_ERROR(message, ...)
#define DEBUG_PRINT_WARNING(message, ...)
#define DEBUG_PRINT_INFO(message, ...)
#define DEBUG_PRINT(message, ...)

#endif

// Asserts
#if CAPP_ENABLE_ASSERTS

#define CAPP_ASSERT(exp, ...)		if(!(exp)) {\
										CAPP_PRINT_ERROR("ASSERTION FAILED: {0}\n{1}", #exp, ##__VA_ARGS__);\
										__debugbreak();\
									}

#define DEBUG_ASSERT(exp, ...)		if(!(exp)) {\
										DEBUG_PRINT_ERROR("ASSERTION FAILED: {0}\n{1}", #exp, ##__VA_ARGS__);\
										__debugbreak();\
									}

#else

#define CAPP_ASSERT(exp, ...)
#define DEBUG_ASSERT(exp, ...)

#endif