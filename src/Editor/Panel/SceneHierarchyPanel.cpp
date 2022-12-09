#include "SceneHierarchyPanel.hpp"

#include <imgui.h>
#include <imgui_internal.h>
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

    // Click blank to cancel focus
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        m_selected_context = {};

    // Right-click on blank space to create Entity
    if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
    {
        if (ImGui::MenuItem("Create Entity"))
        {
            m_entity_created_count++;
            std::string new_entity_num = std::to_string(m_entity_created_count);
            m_selected_context = m_context->CreateEntity("Notitled Entity " + new_entity_num);
        }

        ImGui::EndPopup();
    }

    // End Scene Hierarchy
    ImGui::End();

    // Properties
    ImGui::Begin("Properties");

    if (m_selected_context)
        DrawComponents(m_selected_context);

    // End Properties
    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
    auto& tag = entity.GetComponent<TagComponent>().tag;

    ImGuiTreeNodeFlags flags = 
        ((m_selected_context == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
    
    bool is_opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
    
    if (ImGui::IsItemClicked())
    {
        m_selected_context = entity;
    }

    bool entity_deleted = false;
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Delete Entity"))
            entity_deleted = true;

        ImGui::EndPopup();
    }

    if (is_opened)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        bool is_opened = ImGui::TreeNodeEx((void*)9876543, flags, tag.c_str());
        if (is_opened)
            ImGui::TreePop();

        ImGui::TreePop();
    }

    if (entity_deleted)
    {
        // @TODO: 
        // !!!!! BUG !!!!!
        // Entities that removed first are still rendered until a new SpriteRendererComponent is added to a new Entity.
        m_context->DestroyEntity(entity);
        if (m_selected_context == entity)
            m_selected_context = {};
    }
}

template<class T, class UIFunction>
static void DrawComponent(const std::string& name, Entity entity, UIFunction uifunc)
{
    ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
    tree_node_flags |= ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap;
    tree_node_flags |= ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    if (entity.HasComponent<T>())
    {
        auto& component = entity.GetComponent<T>();
        ImVec2 contentRegion_avail = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
        float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool is_open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), tree_node_flags, name.c_str());
        ImGui::PopStyleVar();
        ImGui::SameLine(contentRegion_avail.x - line_height * 0.5f);
        if (ImGui::Button("+", ImVec2{ line_height, line_height }))
        {
            ImGui::OpenPopup("Component Settings");
        }

        bool is_remove_component = false;
        if (ImGui::BeginPopup("Component Settings"))
        {
            if (ImGui::MenuItem("Remove Component"))
                is_remove_component = true;

            ImGui::EndPopup();
        }

        if (is_open)
        {
            uifunc(component);
            ImGui::TreePop();
        }

        if (is_remove_component)
            entity.RemoveComponent<T>();
    }
}

static void DrawVec3Control(const std::string& label, glm::vec3& value, float reset_value = 0.0f, float column_width = 100.0f)
{
    ImGuiIO& io = ImGui::GetIO();
    auto bold_font = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, column_width);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

    float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 button_size = { line_height + 3.0f, line_height };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(bold_font);
    if (ImGui::Button("X", button_size))
        value.x = reset_value;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##x", &value.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(bold_font);
    if (ImGui::Button("Y", button_size))
        value.y = reset_value;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##y", &value.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushFont(bold_font);
    if (ImGui::Button("Z", button_size))
        value.z = reset_value;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##z", &value.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::Columns(1);

    ImGui::PopID();
}

void SceneHierarchyPanel::DrawComponents(Entity entity)
{
    if (entity.HasComponent<TagComponent>())
    {
        auto& tag = entity.GetComponent<TagComponent>().tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), tag.c_str());
        if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
        {
            tag = std::string(buffer);
        }
    }

    // Add Component Button
    ImGui::SameLine();
    ImGui::PushItemWidth(-1);
    if (m_selected_context)
    {
        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent"))
        {
            if (!m_selected_context.HasComponent<CameraComponent>() && ImGui::MenuItem("Camera"))
            {
                m_selected_context.AddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (!m_selected_context.HasComponent<SpriteRendererComponent>() && ImGui::MenuItem("Sprite Renderer"))
            {
                m_selected_context.AddComponent<SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
    ImGui::PopItemWidth();

    DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
    {
        DrawVec3Control("Translation", component.translation);
        glm::vec3 rotation = glm::degrees(component.rotation);
        DrawVec3Control("Rotation", rotation);
        component.rotation = glm::radians(rotation);
        DrawVec3Control("Scale", component.scale);
    });

    DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
    {
        auto& camera = component.camera;

        ImGui::Checkbox("Primary", &component.is_primary);

        const char* projection_type_strings[] = { "Perspective", "Orthographic" };
        const char* current_project_type_string = projection_type_strings[(int)camera.GetProjectionType()];
        if (ImGui::BeginCombo("Projection", current_project_type_string))
        {
            for (int i = 0; i < 2; i++)
            {
                bool is_selected = (current_project_type_string == projection_type_strings[i]);
                if (ImGui::Selectable(projection_type_strings[i], is_selected))
                {
                    current_project_type_string = projection_type_strings[i];
                    camera.SetProjectionType((SceneCamera::ProjectionType)i);
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
        {
            float perspective_vertical_fov = glm::degrees(camera.GetPerspectiveVerticalFov());
            if (ImGui::DragFloat("Vertical FOV", &perspective_vertical_fov))
                camera.SetPerspectiveVerticalFov(glm::radians(perspective_vertical_fov));

            float perspective_near = camera.GetPerspectiveNearClip();
            if (ImGui::DragFloat("Near", &perspective_near))
                camera.SetPerspectiveNearClip(perspective_near);

            float perspective_far = camera.GetPerspectiveFarClip();
            if (ImGui::DragFloat("Far", &perspective_far))
                camera.SetPerspectiveFarClip(perspective_far);
        }

        if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
        {
            float ortho_size = camera.GetOrthographicSize();
            if (ImGui::DragFloat("Size", &ortho_size))
                camera.SetOrthographicSize(ortho_size);

            float ortho_near = camera.GetOrthographicNearClip();
            if (ImGui::DragFloat("Near", &ortho_near))
                camera.SetOrthographicNearClip(ortho_near);

            float ortho_far = camera.GetOrthographicFarClip();
            if (ImGui::DragFloat("Far", &ortho_far))
                camera.SetOrthographicFarClip(ortho_far);
        }

        ImGui::Checkbox("Fixed Aspect Ratio", &component.is_fixed_aspect_ratio);
        ImGui::TreePop();
    });

    DrawComponent<SpriteRendererComponent>("SpriteRenderer", entity, [](auto& component)
    {
        ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
    });
}

} // namespace Alice
