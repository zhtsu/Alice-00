#pragma once

#include "Alice/PCH.hpp"

#include "Event.hpp"

namespace Alice
{

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

class WindowCloseEvent : public Event
{
public:
    EVENT_CLASS_GATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(WindowClose)
};

class AppTickEvent : public Event
{
public:
    EVENT_CLASS_GATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(AppTick)
};

class AppUpdateEvent : public Event
{
public:
    EVENT_CLASS_GATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(AppUpdate)
};

class AppRenderEvent : public Event
{
public:
    EVENT_CLASS_GATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(AppRender)
};

} // namespace Alice

