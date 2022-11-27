#pragma once

#include "Alice/Layer/Layer.hpp"
#include "Alice/Events/KeyEvent.hpp"
#include "Alice/Events/MouseEvent.hpp"
#include "Alice/Events/ApplicationEvent.hpp"

namespace Alice
{

//
// 封装 imgui 的 UI 层
// 此层用于编写编辑器
//
class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ~ImGuiLayer();

    // 初始化当前层
    void OnAttach() override;
    // 清理当前层
    void OnDetach() override;

    void OnEvent(Event& event);

    // 开始当前层的描述
    void Begin();
    // 结束描述
    void End();

private:
    float m_time;
};

} // namespace Alice
