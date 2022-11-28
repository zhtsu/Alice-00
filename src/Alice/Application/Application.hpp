#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Core/Window.hpp"
#include "Alice/Events/ApplicationEvent.hpp"
#include "Alice/Layer/LayerStack.hpp"
#include "Alice/ImGUI/ImGuiLayer.hpp"

namespace Alice
{

//
// 主程序
// 控制应用逻辑主循环
// 
class Application
{
public:
    Application(const std::string& name = "Application");
    virtual ~Application();

    // 事件回调函数
    // 此函数会被传入窗口实例中
    void OnEvent(Event& event);

    // 主循环
    void Run();

    // 依照次序添加一个层
    void PushLayer(Layer* layer);
    // 在最上层添加一个层
    void PushOverlay(Layer* layer);

    ImGuiLayer* GetImGuiLayer() { return m_imgui_layer; }

    void Close();

    inline static Application& Get() { return *s_instance; }
    inline Window& GetWindow() { return *m_window; }

private:
    // 窗口关闭事件时触发的回调函数
    bool OnWindowClose(WindowCloseEvent& event);
    // 窗口尺寸更新时触发
    bool OnWindowResize(WindowResizeEvent& event);

private:

    std::unique_ptr<Window> m_window;
    // ImGui 层，此层被添加在最上层
    ImGuiLayer* m_imgui_layer;

    // 是否继续执行主循环
    bool m_running = true;
    // 窗口是否被最小化
    bool m_minimized = false;

    // 存储所有层
    LayerStack m_layer_stack;

    // 上一帧被执行时的计时
    float m_last_frame_time = 0.0f;

    static Application* s_instance;
};

} // namespace Alice

