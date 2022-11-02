#pragma once

#include "Alice/Layer/Layer.hpp"
#include "Alice/Events/KeyEvent.hpp"
#include "Alice/Events/MouseEvent.hpp"
#include "Alice/Events/ApplicationEvent.hpp"

namespace Alice
{

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ~ImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnImGuiRender() override;

    void Begin();
    void End();

private:
    float m_time;
};

} // namespace Alice
