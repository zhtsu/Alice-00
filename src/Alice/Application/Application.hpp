#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Window/Window.hpp"
#include "Alice/Events/ApplicationEvent.hpp"
#include "Alice/Layer/LayerStack.hpp"

namespace Alice
{

// 引擎主程序
class Application
{
public:
    Application();
    ~Application();

    void OnEvent(Event& event);

    void Run();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

private:
    bool OnWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> m_window;
    bool m_running;
    LayerStack m_layer_stack;
};

} // namespace Alice

