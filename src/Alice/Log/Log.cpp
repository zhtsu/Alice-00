#include "Log.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Alice
{

std::shared_ptr<spdlog::logger> Log::m_logger;

void Log::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    
    m_logger = spdlog::stdout_color_mt("ALICE");
    m_logger->set_level(spdlog::level::trace);

    m_logger->info("Log::Init: Initialized Logger!");
}

} // namespace Alice
