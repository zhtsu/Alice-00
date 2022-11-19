#pragma once

#include "Event.hpp"

namespace Alice
{

//
// 鼠标移动事件
//
class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y)
        : m_mouse_x(x), m_mouse_y(y) {}

    inline float GetX() const { return m_mouse_x; }
    inline float GetY() const { return m_mouse_y; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << m_mouse_x << ", " << m_mouse_y;
    
        return ss.str();
    }

    EVENT_CLASS_GATEGORY(EventCategoryMouse | EventCategoryInput)
    EVENT_CLASS_TYPE(MouseMoved)

private:
    float m_mouse_x, m_mouse_y;
};

//
// 鼠标滚动事件
//
class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float x_offset, float y_offset)
        : m_x_offset(x_offset), m_y_offset(y_offset) {}

    inline float GetXOffset() const { return m_x_offset; }
    inline float GetYOffset() const { return m_y_offset; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << m_x_offset << ", " << m_y_offset;
    
        return ss.str();
    }

    EVENT_CLASS_GATEGORY(EventCategoryMouse | EventCategoryInput)
    EVENT_CLASS_TYPE(MouseScrolled)

private:
    float m_x_offset, m_y_offset;
};

//
// 鼠标按键事件基类
//
class MouseButtonEvent : public Event
{
public:
    inline int GetMouseButton() const { return m_button; }

    EVENT_CLASS_GATEGORY(EventCategoryMouse | EventCategoryInput);

protected:
    MouseButtonEvent(int button) : m_button(button) {}


    int m_button;
};

//
// 鼠标按键按压事件
//
class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button)
        : MouseButtonEvent(button) {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << m_button;
        
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

//
// 鼠标按键释放事件
//
class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button)
        : MouseButtonEvent(button) {}

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << m_button;
        
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

} // namespace Alice
