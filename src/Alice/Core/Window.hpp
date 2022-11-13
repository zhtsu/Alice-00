#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Events/Event.hpp"

namespace Alice
{

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

// 窗口抽象类
class Window
{
public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() {}

    virtual void OnUpdate() = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;

    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual void* GetNativeWindow() const = 0;

    static Window* Create(const WindowProps& props = WindowProps());
};

} // namespace Alice
