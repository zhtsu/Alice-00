#pragma once

#include "Alice/Layer/Layer.hpp"
#include "imgui.h"
#include "Alice/Platform/OpenGL/ImGuiOpenGLRenderer.h"

namespace Alice
{

class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ~ImGuiLayer();

    void OnAttach();
    void OnDetach();
    void OnUpdate();
    void OnEvent(Event& event);

private:

};

} // namespace Alice
