#pragma once

#include "Alice/Core/Window.hpp"
#include "GLFW/glfw3.h"
#include "Alice/Renderer/GraphicsContext.hpp"

namespace Alice
{

class WindowsWindow : public Window
{
public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    void OnUpdate() override;

    inline unsigned int GetWidth() const override
    {
        return m_data.width;
    }
    inline unsigned int GetHeight() const override
    {
        return m_data.height;
    }

    inline void SetEventCallback(const EventCallbackFn& callback) override
    {
        m_data.event_callback = callback;
    }

    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    inline virtual void* GetNativeWindow() const { return m_window; };

private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();

private:
    GLFWwindow* m_window;
    GraphicsContext* m_context;

    struct WindowData
    {
        std::string title;
        unsigned int width, height;
        bool vsync;
        EventCallbackFn event_callback;
    };


    WindowData m_data;
};

} // namespace Alice
