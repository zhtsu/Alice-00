#include "EditorLayer.hpp"
#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include "Alice/Debug/Instrumentor.hpp"
#include "Alice/Scene/SceneSerializer.hpp"
#include "Alice/Utils/FileDialogs.hpp"
#include "Alice/Math/Math.hpp"

namespace Alice
{

// Defined in EditorLayer.cpp
extern const std::filesystem::path g_assets_path;

EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_camera_controller(1280.0f / 720.0f)
{

}

void EditorLayer::OnAttach()
{
    FramebufferSpecification framebuffer_spec;
    framebuffer_spec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
    framebuffer_spec.width = 1280;
    framebuffer_spec.height = 720;
    m_framebuffer = Framebuffer::Create(framebuffer_spec);

    m_active_scene = CreateRef<Scene>();
    m_scene_hierarchy_panel.SetContext(m_active_scene);
    m_editor_camera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

    m_icon_play = Texture2D::Create("resources/icons/play.png");
    m_icon_pause = Texture2D::Create("resources/icons/pause.png");
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnUpdate(Timestep ts)
{
    ALICE_PROFILE_FUNCTION();

    m_active_scene->OnViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);

    // Resize
    FramebufferSpecification spec = m_framebuffer->GetSpecification();
    if (m_viewport_size.x > 0.0f && m_viewport_size.y > 0.0f &&
        (spec.width != m_viewport_size.x || spec.height != m_viewport_size.y))
    {
        m_framebuffer->Resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
        m_camera_controller.OnResize(m_viewport_size.x, m_viewport_size.y);
        m_editor_camera.SetViewportSize(m_viewport_size.x, m_viewport_size.y);
    }

    m_editor_camera.OnUpdate(ts);

    // Render
    Renderer2D::ResetStats();
    m_framebuffer->Bind();
    RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    RenderCommand::Clear();

    // Clear framebuffer attachment by empty entity ID
    m_framebuffer->ClearAttachment(1, -1);

    // Update Scene
    switch (m_scene_state)
    {
        case SceneState::Edit:
        {
            // Update Camera
            if (m_viewport_focused)
                m_camera_controller.OnUpdate(ts);

            m_active_scene->OnUpdateEditor(ts, m_editor_camera);
            break;
        }
        case SceneState::Play:
        {
            m_active_scene->OnUpdateRuntime(ts);
            break;
        }
    }

    auto[mx, my] = ImGui::GetMousePos();
    mx -= m_viewport_bounds[0].x;
    my -= m_viewport_bounds[0].y;
    glm::vec2 viewport_size = m_viewport_bounds[1] - m_viewport_bounds[0];
    my = viewport_size.y - my;
    int mouse_x = (int)mx;
    int mouse_y = (int)my;

    if (mouse_x >= 0 && mouse_y >= 0 && mouse_x < (int)viewport_size.x && mouse_y < (int)viewport_size.y)
    {
        int pixel_data = m_framebuffer->ReadPixel(1, mouse_x, mouse_y);
        if (pixel_data == -1)
            m_hovered_entity = {};
        else
            m_hovered_entity = { (entt::entity)pixel_data, m_active_scene.get() };
    }

    m_framebuffer->Unbind();
}

void EditorLayer::OnImGuiRender()
{
    ALICE_PROFILE_FUNCTION();

    static bool is_dockspace_open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &is_dockspace_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float min_window_size_x = style.WindowMinSize.x;
    style.WindowMinSize.x = 360.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    // Recover WindowMinSize.x
    style.WindowMinSize.x = min_window_size_x;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl + N"))
                NewScene();

            if (ImGui::MenuItem("Open", "Ctrl + O"))
                OpenScene();

            if (ImGui::MenuItem("Save As", "Ctrl + Shift + S"))
                SaveSceneAs();

            if (ImGui::MenuItem("Exit"))
                Application::Get().Close();

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    m_scene_hierarchy_panel.OnImGuiRender();
    m_content_browser_panel.OnImGuiRender();

    // Stats
    ImGui::Begin("Stats");
    
    // Renderer2D Stats
    auto stats = Renderer2D::GetStats();
    ImGui::Separator();
    ImGui::Text("Renderer2D");
    ImGui::Separator();
    ImGui::Text("Draw Calls: %d", stats.draw_calls);
    ImGui::Text("Quads: %d", stats.quad_count);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::NewLine();

    // ECS Stats
    ImGui::Separator();
    ImGui::Text("ECS");
    ImGui::Separator();
    ImGui::Text("Entity Capacity: %d", m_active_scene->GetEntityCapacity());
    ImGui::Text("Alive Entities: %d", m_active_scene->GetAliveEntityCount());
    ImGui::Text("Rendered Entities: %d", m_active_scene->GetRenderedEntitiesCount());
    std::string hovered_entity_name = "None";
    if (m_hovered_entity)
        hovered_entity_name = m_hovered_entity.GetComponent<TagComponent>().tag;
    ImGui::Text("Hovered Entity: %s (%d)", hovered_entity_name.c_str(), (uint32_t)m_hovered_entity);
    std::string selected_entity_name = "None";
    if ( m_scene_hierarchy_panel.GetSelectedEntity())
        selected_entity_name =  m_scene_hierarchy_panel.GetSelectedEntity().GetComponent<TagComponent>().tag;
    ImGui::Text("Selected Entity: %s (%d)", selected_entity_name.c_str(), (uint32_t)m_scene_hierarchy_panel.GetSelectedEntity());
    ImGui::NewLine();

    ImGui::End();

    // Viewport
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    ImGui::Begin("##viewport", nullptr, ImGuiDockNodeFlags_AutoHideTabBar);

    auto viewport_min_region = ImGui::GetWindowContentRegionMin();
    auto viewport_max_region = ImGui::GetWindowContentRegionMax();
    auto viewport_offset = ImGui::GetWindowPos();
    m_viewport_bounds[0] = { viewport_min_region.x + viewport_offset.x, viewport_min_region.y + viewport_offset.y };
    m_viewport_bounds[1] = { viewport_max_region.x + viewport_offset.x, viewport_max_region.y + viewport_offset.y };

    m_viewport_focused = ImGui::IsWindowFocused();
    m_viewport_hovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->BlockEvents(!m_viewport_hovered);

    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    m_viewport_size = { viewport_panel_size.x, viewport_panel_size.y };

    uint32_t frame_buffer_texture = m_framebuffer->GetColorAttachmentRendererID();
    ImGui::Image(reinterpret_cast<void*>(frame_buffer_texture),
        ImVec2{ m_viewport_size.x, m_viewport_size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    // Open Scene from Content Browser
    if (ImGui::BeginDragDropTarget())
    {
        // @TODO:
        // No hightlight effect when drag the file to the viewport panel
        // An unreasonable solution is to set the ImGuiStyleVar_WindowPadding more than 0
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
        {
            const wchar_t* path = (const wchar_t*)payload->Data;
            OpenScene(std::filesystem::path(g_assets_path) / path);
        }
        ImGui::EndDragDropTarget();
    }

    // Gizmos
    Entity selected_entity = m_scene_hierarchy_panel.GetSelectedEntity();
    if (selected_entity && m_gizmo_type != -1)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        float window_width = (float)ImGui::GetWindowWidth();
        float window_height = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height);
        
        // Editor Camera
        const glm::mat4& camera_proj = m_editor_camera.GetProjection();
        glm::mat4 camera_view = m_editor_camera.GetViewMatrix();

        // Entity
        auto& transform_comp = selected_entity.GetComponent<TransformComponent>();
        glm::mat4 transform = transform_comp.GetTransform();

        // Snapping
        bool snap = Input::IsKeyPressed(ALICE_KEY_LEFT_CONTROL);
        
        float snap_value = 0.5f;
        if (m_gizmo_type == ImGuizmo::OPERATION::ROTATE)
            snap_value = 45.0f;

        float snap_values[3] = { snap_value, snap_value, snap_value };

        bool is_maipulated = ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_proj),
            (ImGuizmo::OPERATION)m_gizmo_type, ImGuizmo::LOCAL, glm::value_ptr(transform),
            nullptr, snap ? snap_values : nullptr);

        if (is_maipulated && ImGuizmo::IsUsing())
        {
            glm::vec3 translation, rotation, scale;
            Math::DecomposeTransform(transform, translation, rotation, scale);
            glm::vec3 delta_rotation = rotation - transform_comp.rotation;
            transform_comp.translation = translation;
            transform_comp.rotation += delta_rotation;
            transform_comp.scale = scale;
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();

    PutToolbar();

    ImGui::End();
}

void EditorLayer::PutToolbar()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    auto& colors = ImGui::GetStyle().Colors;;
    const auto& button_hovered = colors[ImGuiCol_ButtonHovered];
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
        ImVec4(button_hovered.x, button_hovered.y, button_hovered.z, 0.5f));
    const auto& button_active = colors[ImGuiCol_ButtonActive];
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
        ImVec4(button_active.x, button_active.y, button_active.z, 0.5f));

    ImGui::Begin("##toolbar", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse | ImGuiDockNodeFlags_AutoHideTabBar);

    Ref<Texture2D> icon = (m_scene_state == SceneState::Play ? m_icon_pause : m_icon_play);
    float size = ImGui::GetWindowHeight() - 4.0f;
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
    if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->GetRendererID()), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
    {
        if (m_scene_state == SceneState::Edit)
            OnScenePlay();
        else if (m_scene_state == SceneState::Play)
            OnSceneStop();
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    ImGui::End();
}

void EditorLayer::OnEvent(Event& event)
{
    m_camera_controller.OnEvent(event);
    m_editor_camera.OnEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(ALICE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    dispatcher.Dispatch<MouseButtonPressedEvent>(ALICE_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
{
    if (event.GetRepeatCount() > 0)
        return false;

    bool control = Input::IsKeyPressed(ALICE_KEY_LEFT_CONTROL) || Input::IsKeyPressed(ALICE_KEY_RIGHT_CONTROL);
    bool shift = Input::IsKeyPressed(ALICE_KEY_LEFT_SHIFT) || Input::IsKeyPressed(ALICE_KEY_RIGHT_SHIFT);
    switch (event.GetKeyCode())
    {
        case ALICE_KEY_N:
        {
            if (control)
                NewScene();
            break;
        }
        case ALICE_KEY_O:
        {
            if (control)
                OpenScene();
            break;
        }
        case ALICE_KEY_S:
        {
            if (control && shift)
                SaveSceneAs();
            break;
        }
        // Gizmos
        case ALICE_KEY_Q:
        {
            if (!ImGuizmo::IsUsing())
                m_gizmo_type = -1;
            break;
        }
        case ALICE_KEY_W:
        {
            if (!ImGuizmo::IsUsing())
                m_gizmo_type = ImGuizmo::OPERATION::TRANSLATE;
            break;
        }
        case ALICE_KEY_E:
        {
            if (!ImGuizmo::IsUsing())
                m_gizmo_type = ImGuizmo::OPERATION::ROTATE;
            break;
        }
        case ALICE_KEY_R:
        {
            if (!ImGuizmo::IsUsing())
                m_gizmo_type = ImGuizmo::OPERATION::SCALE;
            break;
        }
        default:
        {
            break;
        }
    }

    return true;
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
{
    if (event.GetMouseButton() == ALICE_MOUSE_BUTTON_LEFT)
    {
        if (m_viewport_hovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(ALICE_KEY_LEFT_ALT))
            m_scene_hierarchy_panel.SetSelectedEntity(m_hovered_entity);
    }
    return true;
}

void EditorLayer::NewScene()
{
    m_active_scene = CreateRef<Scene>();
    m_active_scene->OnViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
    m_scene_hierarchy_panel.SetContext(m_active_scene);
}

void EditorLayer::OpenScene()
{
    std::string filepath = FileDialogs::OpenFile("ascene");
    if (!filepath.empty())
        OpenScene(filepath);
}

void EditorLayer::OpenScene(const std::filesystem::path& path)
{
    m_active_scene = CreateRef<Scene>();
    m_active_scene->OnViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
    m_scene_hierarchy_panel.SetContext(m_active_scene);

    SceneSerializer serializer(m_active_scene);
    serializer.Deserialize(path.string());
}

void EditorLayer::SaveSceneAs()
{
    std::string filepath = FileDialogs::SaveFile("ascene");
    if (!filepath.empty())
    {
        SceneSerializer serializer(m_active_scene);
        serializer.Serialize(filepath);
    }
}

void EditorLayer::OnScenePlay()
{
    m_scene_state = SceneState::Play;
}

void EditorLayer::OnSceneStop()
{
    m_scene_state = SceneState::Edit;
}

} // namespace Alice
