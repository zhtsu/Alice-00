#pragma once

#include "Alice/Layer/Layer.hpp"
#include "Alice/Events/KeyEvent.hpp"
#include "Alice/Events/MouseEvent.hpp"
#include "Alice/Events/ApplicationEvent.hpp"

namespace Alice
{

//
// Used to debug or code Editor
//
class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ~ImGuiLayer();

    // Initialize
    void OnAttach() override;
    // Destroy
    void OnDetach() override;

    void OnEvent(Event& event);

    // Begin a new frame
    void Begin();
    // End a frame
    void End();

    void BlockEvents(bool block) { m_block_events = block; }

    void SetDarkThemeColors();
    void SetDarkGreenThemeColors();

private:
    bool m_block_events = true;
    float m_time;
};

} // namespace Alice
