#include "EditorLayer.hpp"
#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include "Alice/Debug/Instrumentor.hpp"
#include "Alice/Scene/SceneSerializer.hpp"
#include "Alice/Utils/FileDialogs.hpp"
// #include "Alice/Math/Math.hpp"

namespace Alice
{

EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_camera_controller(1280.0f / 720.0f)
{

}

void EditorLayer::OnAttach()
{
    FramebufferSpecification framebuffer_spec;
    framebuffer_spec.attachments = { FramebufferTextureFormat::RGB8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
    framebuffer_spec.width = 1280;
    framebuffer_spec.height = 720;
    m_framebuffer = Framebuffer::Create(framebuffer_spec);

    m_active_scene = CreateRef<Scene>();
    m_scene_hierarchy_panel.SetContext(m_active_scene);
    m_editor_camera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
}

void EditorLayer::OnDetach()
{

}

void EditorLayer::OnUpdate(Timestep ts)
{
    ALICE_PROFILE_FUNCTION();

    // Resize
    FramebufferSpecification spec = m_framebuffer->GetSpecification();
    if (m_viewport_size.x > 0.0f && m_viewport_size.y > 0.0f &&
        (spec.width != m_viewport_size.x || spec.height != m_viewport_size.y))
    {
        m_framebuffer->Resize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
        m_camera_controller.OnResize(m_viewport_size.x, m_viewport_size.y);
        m_editor_camera.SetViewportSize(m_viewport_size.x, m_viewport_size.y);
        m_active_scene->OnViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
    }

    // Update Camera
    if (m_viewport_focused)
        m_camera_controller.OnUpdate(ts);

    m_editor_camera.OnUpdate(ts);

    // Render
    Renderer2D::ResetStats();
    m_framebuffer->Bind();
    RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    RenderCommand::Clear();

    // Update Scene
    m_active_scene->OnUpdateEditor(ts, m_editor_camera);

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
        ALICE_TRACE("Pixel data: {}", pixel_data);
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

    // Stats
    ImGui::Begin("Stats");
    
    // Renderer2D Stats
    auto stats = Renderer2D::GetStats();
    ImGui::Separator();
    ImGui::Text("Renderer2D Stats");
    ImGui::Separator();
    ImGui::Text("Draw Calls: %d", stats.draw_calls);
    ImGui::Text("Quads: %d", stats.quad_count);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::NewLine();

    // ECS Stats
    ImGui::Separator();
    ImGui::Text("ECS Stats");
    ImGui::Separator();
    ImGui::Text("Entity Capacity: %d", m_active_scene->GetEntityCapacity());
    ImGui::Text("Alive Entities: %d", m_active_scene->GetAliveEntityCount());
    ImGui::Text("Rendered Entities: %d", m_active_scene->GetRenderedEntitiesCount());
    ImGui::NewLine();

    ImGui::End();

    // Viewport
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    ImGui::Begin("Viewport");
    auto viewport_offset = ImGui::GetCursorPos();

    m_viewport_focused = ImGui::IsWindowFocused();
    m_viewport_hovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->BlockEvents(m_viewport_focused && m_viewport_hovered);

    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    m_viewport_size = { viewport_panel_size.x, viewport_panel_size.y };

    uint32_t frame_buffer_texture = m_framebuffer->GetColorAttachmentRendererID();
    ImGui::Image(reinterpret_cast<void*>(frame_buffer_texture), ImVec2{ m_viewport_size.x, m_viewport_size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    
    auto window_size = ImGui::GetWindowSize();
    ImVec2 min_bound = ImGui::GetWindowPos();
    min_bound.x += viewport_offset.x;
    min_bound.y += viewport_offset.y;

    ImVec2 max_bound = { min_bound.x + window_size.x, min_bound.y + window_size.y };
    m_viewport_bounds[0] = { min_bound.x, min_bound.y };
    m_viewport_bounds[1] = { max_bound.x, max_bound.y };

    // Gizmos
    Entity selected_entity = m_scene_hierarchy_panel.GetSelectedEntity();
    if (selected_entity && m_gizmo_type != -1)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        float window_width = (float)ImGui::GetWindowWidth();
        float window_height = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height);
        
        // Camera
        //
        // Runtime Camera
        // auto camera_entity = m_active_scene->GetPrimaryCameraEntity();
        // const auto& camera = camera_entity.GetComponent<CameraComponent>().camera;
        // const glm::mat4& camera_proj = camera.GetProjection();
        // glm::mat4 camera_view = glm::inverse(camera_entity.GetComponent<TransformComponent>().GetTransform());

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

        // @TODO:
        // !!!!! BUG !!!!!
        // Manipulate make transfrom to NaN when try to changed translation or rotation by gizmos. 
        //
        // if (is_maipulated && ImGuizmo::IsUsing())
        // {
        //     glm::vec3 translation, rotation, scale;
        //     Math::DecomposeTransform(transform, translation, rotation, scale);
        //     glm::vec3 delta_rotation = rotation - transform_comp.rotation;
        //     transform_comp.translation = translation;
        //     transform_comp.rotation += delta_rotation;
        //     transform_comp.scale = scale;
        // }
    }

    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

void EditorLayer::OnEvent(Event& event)
{
    m_camera_controller.OnEvent(event);
    m_editor_camera.OnEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(ALICE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
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
    {
        m_active_scene = CreateRef<Scene>();
        m_active_scene->OnViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
        m_scene_hierarchy_panel.SetContext(m_active_scene);

        SceneSerializer serializer(m_active_scene);
        serializer.Deserialize(filepath);
    }
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

} // namespace Alice
