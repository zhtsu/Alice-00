#pragma once

#include <iostream>
#include <chrono>

namespace Alice
{
//
// 计时器
//
template<class Fn>
class Timer
{
public:
    Timer(const char* name, Fn&& func)
        : m_name(name), m_stopped(false), m_func(func)
    {
        m_start_timepoint = std::chrono::steady_clock::now();
    }

    ~Timer()
    {
        if (!m_stopped) 
        {
            Stop();
        }
    }

    void Stop()
    {
        auto end_timepoint = std::chrono::steady_clock::now();
        
        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_timepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch().count();

        m_stopped = true;

        float duration = (end - start) * 0.001f;
        m_func({ m_name, duration });
    }

private:
    const char* m_name;
    Fn m_func;
    std::chrono::time_point<std::chrono::steady_clock> m_start_timepoint;
    bool m_stopped;
};

} // namespace Alice
