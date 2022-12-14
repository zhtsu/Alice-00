#pragma once 

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "Alice/PCH.hpp"

namespace Alice
{

class Log
{
public:
    Log() {}
    ~Log() {}

    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetLogger()
    {
        return m_logger;
    }

private:
    static std::shared_ptr<spdlog::logger> m_logger;
};

} // namespace Alice

#define ALICE_TRACE(...)   Alice::Log::GetLogger()->trace(__VA_ARGS__)
#define ALICE_INFO(...)    Alice::Log::GetLogger()->info(__VA_ARGS__)
#define ALICE_WARN(...)    Alice::Log::GetLogger()->warn(__VA_ARGS__)
#define ALICE_ERROR(...)   Alice::Log::GetLogger()->error(__VA_ARGS__)

#define ALICE_ASSERT(x, ...) { if (!(x)) { ALICE_ERROR("Assertion Failed: {}", __VA_ARGS__); abort();} }
