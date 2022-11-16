#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

namespace Alice
{

struct ProfileResult
{
    std::string name;
    long long start, end;
    uint32_t thread_id;
};

struct InstrumentationSession
{
    std::string name;
};

class Instrumentor
{
public:
    Instrumentor()
        : m_current_session(nullptr), m_profile_count(0)
    {

    }

    void BeginSession(const std::string& name, const std::string& filepath="results.json")
    {
        m_output_stream.open(filepath);
        WriteHeader();
        m_current_session = new InstrumentationSession{ name };
    }

    void EndSession()
    {
        WriteFooter();
        m_output_stream.close();
        delete m_current_session;
        m_current_session = nullptr;
        m_profile_count = 0;
    }

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

class InstrumentationTimer
{
public:

private:

};

} // namespace Alice

