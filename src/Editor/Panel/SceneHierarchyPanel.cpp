#include "SceneHierarchyPanel.hpp"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "Alice/Scene/Components.hpp"

namespace Alice
{

SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
{
    SetContext(context);
}

void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
{
    m_context = context;
}

void SceneHierarchyPanel::OnImGuiRender()
{
    // Scene Hierarchy
    ImGui::Begin("Scene Hierarchy");
    
    m_context->m_registry.each([&](auto entity_id){
        Entity entity{ entity_id, m_context.get() };
        DrawEntityNode(entity);
    });

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        m_selected_context = {};

    ImGui::End();

    // Properties
    ImGui::Begin("Properties");

    if (m_selected_context)
        DrawComponents(m_selected_context);

    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
    auto& tag = entity.GetComponent<TagComponent>().tag;

    ImGuiTreeNodeFlags flags = 
        ((m_selected_context == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool is_opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
    
    if (ImGui::IsItemClicked())
    {
        m_selected_context = entity;
    }

    if (is_opened)
    {
        ImGui::TreePop();
    }
}

void SceneHierarchyPanel::DrawComponents(Entity entity)
{
    if (entity.HasComponent<TagComponent>())
    {
        auto& tag = entity.GetComponent<TagComponent>().tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), tag.c_str());
        if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
        {
            tag = std::string(buffer);
        }
    }

    if (entity.HasComponent<TransformComponent>())
    {
        if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
        {
            auto& transform = entity.GetComponent<TransformComponent>().transform;
            ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

            ImGui::TreePop();
        }
    }
}

} // namespace Alice
