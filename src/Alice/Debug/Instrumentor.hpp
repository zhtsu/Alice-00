#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>

namespace Alice
{

//
// 用于分析性能的数据
//
struct ProfileResult
{
    std::string name;
    long long start, end;
    uint32_t thread_id;
};

//
// 仪表的一次会话
// 每一次会话会记录下需要的性能数据
//
struct InstrumentationSession
{
    std::string name;
};

//
// 仪表
// 负责创建和销毁每一次会话
//
class Instrumentor
{
public:
    Instrumentor()
        : m_current_session(nullptr), m_profile_count(0)
    {

    }

    // 开启一次会话
    void BeginSession(const std::string& name, const std::string& filepath="results.json")
    {
        m_output_stream.open(filepath);
        WriteHeader();
        m_current_session = new InstrumentationSession{ name };
    }

    // 结束一次会话
    void EndSession()
    {
        WriteFooter();
        m_output_stream.close();
        delete m_current_session;
        m_current_session = nullptr;
        m_profile_count = 0;
    }

    // 写入一组数据
    void WriteProfile(const ProfileResult& result)
    {
        if (m_profile_count++ > 0)
        {
            m_output_stream << ",";
        }

        std::string _name = result.name;
        std::replace(_name.begin(), _name.end(), '"', '\'');

        m_output_stream << "{";
        m_output_stream << "\"cat\":\"function\",";
        m_output_stream << "\"dur\":" << (result.end - result.start) << ',';
        m_output_stream << "\"name\":\"" << _name << "\",";
        m_output_stream << "\"ph\":\"X\",";
        m_output_stream << "\"pid\":0,";
        m_output_stream << "\"tid\":" << result.thread_id << ",";
        m_output_stream << "\"ts\":" << result.start;
        m_output_stream << "}";

        m_output_stream.flush();
    }

    void WriteHeader()
    {
        m_output_stream << "{\"otherData\": {},\"traceEvents\":[";
        
        m_output_stream.flush();
    }

    void WriteFooter()
    {
        m_output_stream << "]}";

        m_output_stream.flush();
    }

    // 获取全局唯一的静态仪表
    static Instrumentor& Get()
    {
        static Instrumentor instance;
        
        return instance;
    }

private:
    InstrumentationSession* m_current_session;
    std::ofstream m_output_stream;
    int m_profile_count;
};

//
// 计时器
// 通过将此计时器放入一段代码的生命周期
// 可以记录下该段代码的执行用时
//
class InstrumentationTimer
{
public:
    InstrumentationTimer(const char* name)
        : m_name(name), m_stopped(false)
    {
        m_start_timepoint = std::chrono::high_resolution_clock::now();
    }

    ~InstrumentationTimer()
    {
        if (!m_stopped)
        {
            Stop();
        }
    }

    void Stop()
    {
        auto end_timepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_timepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch().count();
    
        uint32_t thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::Get().WriteProfile({ m_name, start, end, thread_id });
    }

private:
    const char* m_name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_timepoint;
    bool m_stopped;
};

} // namespace Alice

#define ALICE_PROFILE 1
#if ALICE_PROFILE
    #define ALICE_PROFILE_BEGIN_SESSION(name, filepath) ::Alice::Instrumentor::Get().BeginSession(name, filepath)
    #define ALICE_PROFILE_END_SESSION()                 ::Alice::Instrumentor::Get().EndSession()
    #define ALICE_PROFILE_SCOPE(name)                   ::Alice::InstrumentationTimer timer##__LINE__(name)
    #define ALICE_PROFILE_FUNCTION()                    ALICE_PROFILE_SCOPE(__FUNCTION__)
#else
    #define ALICE_PROFILE_BEGIN_SESSION(name, filepath)
    #define ALICE_PROFILE_END_SESSION()
    #define ALICE_PROFILE_SCOPE(name)
    #define ALICE_PROFILE_FUNCTION()
#endif