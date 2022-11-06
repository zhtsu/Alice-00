#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Window/Window.hpp"
#include "Alice/Events/ApplicationEvent.hpp"
#include "Alice/Layer/LayerStack.hpp"
#include "Alice/UI/ImGuiLayer.hpp"
#include "Alice/Renderer/Shader.hpp"
#include "Alice/Renderer/Buffer.hpp"
#include "Alice/Renderer/VertexArray.hpp"

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
    bool m_running;
    LayerStack m_layer_stack;
    
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<VertexBuffer> m_vertex_buffer;
    std::shared_ptr<IndexBuffer> m_index_buffer;
    std::shared_ptr<VertexArray> m_vertex_array;

    std::shared_ptr<Shader> m_blue_shader;
    std::shared_ptr<VertexArray> m_square_vao;

    static Application* s_instance;
};

} // namespace Alice

