#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Core/Window.hpp"
#include "Alice/Events/ApplicationEvent.hpp"
#include "Alice/Layer/LayerStack.hpp"
#include "Alice/ImGUI/ImGuiLayer.hpp"

namespace Alice
{

///
/// Control main loop
/// 
class Application
{
public:
    Application(const std::string& name = "Application");
    virtual ~Application();

    // Event callback function
    // This function will passed into the window instance
    void OnEvent(Event& event);

    // run main loop
    void Run();

    // Add a layer in squence
    void PushLayer(Layer* layer);
    // Add a layer in uppest
    void PushOverlay(Layer* layer);

    ImGuiLayer* GetImGuiLayer() { return m_imgui_layer; }

    void Close();

    inline static Application& Get() { return *s_instance; }
    inline Window& GetWindow() { return *m_window; }

private:
    // Callback when window is closed
    bool OnWindowClose(WindowCloseEvent& event);
    // Callback when window is resized
    bool OnWindowResize(WindowResizeEvent& event);

private:

    std::unique_ptr<Window> m_window;
    // ImGui layer, this layer added to uppest always
    ImGuiLayer* m_imgui_layer;

    bool m_running = true;
    bool m_minimized = false;

    LayerStack m_layer_stack;

    float m_last_frame_time = 0.0f;

    static Application* s_instance;
};

} // namespace Alice

