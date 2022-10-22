#pragma once

#include "Alice/pch.hpp"

#include "Event.hpp"

namespace Alice
{

// 窗口尺寸改变事件
class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : m_width(width), m_height(height) {}

    inline unsigned int GetWidth() const { return m_width; }
    inline unsigned int GetHeight() const { return m_height; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_width << ", " << m_height;

        return ss.str();
    }

    EVENT_CLASS_GATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(WindowResize)

private:
    unsigned int m_width, m_height;
};

// 窗口关闭事件
class WindowCloseEvent : public Event
{
public:
    EVENT_CLASS_GATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(WindowClose)
};

// 程序响应事件
class AppTickEvent : public Event
{
public:
    EVENT_CLASS_GATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(AppTick)
};

// 程序更新事件
class AppUpdateEvent : public Event
{
public:
    EVENT_CLASS_GATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(AppUpdate)
};

// 程序渲染事件
class AppRenderEvent : public Event
{
public:
    EVENT_CLASS_GATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(AppRender)
};

} // namespace Alice

