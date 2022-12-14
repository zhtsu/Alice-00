#pragma once

#include "Event.hpp"

namespace Alice
{


class KeyEvent : public Event
{
public:
    inline int GetKeyCode() const { return m_key_code; }

    EVENT_CLASS_GATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
    KeyEvent(int key_code) : m_key_code(key_code) {}


    int m_key_code;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(int key_code, int repeat_count)
        : KeyEvent(key_code), m_repeat_count(repeat_count) {}

    inline int GetRepeatCount() const { return m_repeat_count; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << m_key_code << " (" << m_repeat_count << ")";
    
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)

private:
    int m_repeat_count;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(int key_code) : KeyEvent(key_code) {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << m_key_code;
    
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
    KeyTypedEvent(int key_code) : KeyEvent(key_code) {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << m_key_code;
    
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
};

} // namespace Alice
