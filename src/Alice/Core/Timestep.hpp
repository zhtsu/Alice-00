#pragma once

namespace Alice
{

///
/// Duration of a frame
///
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
