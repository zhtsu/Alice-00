#pragma once

#include "Alice/Core/Base.hpp"
#include "Alice/Scene/Scene.hpp"
#include "Alice/Scene/Entity.hpp"

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
    void DrawComponents(Entity entity);

private:
    Ref<Scene> m_context;
    Entity m_selected_context;
};

} // namespace Alice
