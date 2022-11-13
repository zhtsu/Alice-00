#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Core/Window.hpp"
#include "Alice/Events/ApplicationEvent.hpp"
#include "Alice/Layer/LayerStack.hpp"
#include "Alice/UI/ImGuiLayer.hpp"

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

    inline static Application& Get() { return *s_instance; }
    inline Window& GetWindow() { return *m_window; }

private:
    bool OnWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> m_window;
    std::unique_ptr<ImGuiLayer> m_imgui_layer;
    bool m_running = true;
    LayerStack m_layer_stack;

    float m_last_frame_time = 0.0f;

    static Application* s_instance;
};

} // namespace Alice

