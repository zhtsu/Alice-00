#pragma once

#include "Alice/pch.hpp"

namespace Alice
{

// 应用内可能发生的事件类型
enum class EventType
{
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

// 根据 1 在二进制中的位置区分类别
#define BIT(x) (1 << x) 

// 事件种类
// 每一种事件种类中可能包含多种事件类型
enum EventCategory
{
    None = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput       = BIT(1),
    EventCategoryKeyboard    = BIT(2),
    EventCategoryMouse       = BIT(3),
    EventCategoryButton      = BIT(4)
};

// 这两个宏用于在事件类中快速地声明事件相关方法
#define _EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                                virtual EventType GetEventType() const override {  return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }

// 转换宏
#define EVENT_CLASS_TYPE(type) _EVENT_CLASS_TYPE(type)

#define EVENT_CLASS_GATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

// 事件抽象基类
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

protected:
    bool m_handled;
};

// 事件分配器
class EventDispatcher
{
    template<class T>
    using EventFn = std::function<bool(T&)>;

public:
    EventDispatcher(Event& event) : m_event(event) {}

    // 为事件分配一个回调函数
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

} // namespace Alice

