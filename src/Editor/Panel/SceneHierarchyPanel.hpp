#pragma once

#include "Alice/Core/Base.hpp"
#include "Alice/Scene/Scene.hpp"

namespace Alice
{

class SceneHierarchyPanel
{
public:
    SceneHierarchyPanel() = default;
    SceneHierarchyPanel(const Ref<Scene>& context);

    void SetContext(const Ref<Scene>& context);

    void OnImGuiRender();

private:
    void DrawEntityNode(Entity entity);

private:
    Ref<Scene> m_context;
    Entity m_selected_context;
};

} // namespace Alice
