#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Events/Event.hpp"

namespace Alice
{

//
// Window 类构造信息结构体
//
struct WindowProps
{
    std::string title;
    unsigned int width;
    unsigned int height;

    WindowProps(
        const std::string& title_ = "Alice Engine",
        unsigned int width_ = 960,
        unsigned int height_ = 640
    ) : title(title_), width(width_), height(height_)
    {

    }
};

//
// 平台无关的窗口抽象类
//
class Window
{
public:
    // 回调函数类型重命名
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() {}

    virtual void OnUpdate() = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;

    // 设置窗口的事件回调
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    // 设置垂直同步
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    // 获取当前的窗口实例
    virtual void* GetNativeWindow() const = 0;

    // 创建一个窗口
    // 由具体平台实现
    static Window* Create(const WindowProps& props = WindowProps());
};

} // namespace Alice
