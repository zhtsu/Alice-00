#pragma once

#include "Alice/PCH.hpp"

namespace Alice
{

enum class EventType
{
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

#define BIT(x) (1 << x) 

enum EventCategory
{
    None = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput       = BIT(1),
    EventCategoryKeyboard    = BIT(2),
    EventCategoryMouse       = BIT(3),
    EventCategoryButton      = BIT(4)
};

#define _EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                                virtual EventType GetEventType() const override {  return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_TYPE(type) _EVENT_CLASS_TYPE(type)

#define EVENT_CLASS_GATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

class Event
{
    friend class EventDispatcher;

public:
    Event():m_handled(false) {}

    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    inline bool IsInCategory(EventCategory category)
    {
        return GetCategoryFlags() & category;
    }

    bool m_handled;
};

class EventDispatcher
{
    template<class T>
    using EventFn = std::function<bool(T&)>;

public:
    EventDispatcher(Event& event) : m_event(event) {}

    template<typename T>
    bool Dispatch(EventFn<T> func)
    {
        if (m_event.GetEventType() == T::GetStaticType())
        {
            m_event.m_handled = func(*(T*)&m_event);
            return true;
        }

        return false;
    }

private:
    Event& m_event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
    return os << e.ToString();
}

#define ALICE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

} // namespace Alice

