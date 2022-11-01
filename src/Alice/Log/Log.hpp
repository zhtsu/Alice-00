#pragma once 

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "Alice/PCH.hpp"

namespace Alice
{

// 日志类，用于打印引擎日志信息
class Log
{
public:
    Log() {}
    ~Log() {}

    static void Init();

    // 获取日志对象
    inline static std::shared_ptr<spdlog::logger>& GetLogger()
    {
        return m_logger;
    }

private:
    static std::shared_ptr<spdlog::logger> m_logger;
};

} // namespace Alice

// 打印日志宏命令
// 使用此系列宏前请确保已经调用了 Log::Init 方法
#define ALICE_TRACE(...)   Alice::Log::GetLogger()->trace(__VA_ARGS__)
#define ALICE_INFO(...)    Alice::Log::GetLogger()->info(__VA_ARGS__)
#define ALICE_WARN(...)    Alice::Log::GetLogger()->warn(__VA_ARGS__)
#define ALICE_ERROR(...)   Alice::Log::GetLogger()->error(__VA_ARGS__)

// 异常终止断言
#define ALICE_ASSERT(x, ...) { if (!(x)) { ALICE_ERROR("Assertion Failed: {}", __VA_ARGS__); abort();} }
