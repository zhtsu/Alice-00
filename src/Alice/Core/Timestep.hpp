#pragma once

namespace Alice
{

//
// 一帧的计时步长
// 仅仅是对 float 类型的简单封装
//
class Timestep
{
public:
    Timestep(float time = 0.0f)
        : m_time(time)
    {
        
    }

    operator float() const { return m_time; }

    float GetSeconds() const { return m_time; }
    float GetMilliseconds() const { return m_time * 1000.0f; }

private:
    float m_time;
};

} // namespace Alice
