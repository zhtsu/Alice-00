#include "EditorLayer.hpp"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "Alice/Debug/Instrumentor.hpp"
#include "Alice/Scene/SceneSerializer.hpp"
#include "Alice/Utils/FileDialogs.hpp"

namespace Alice
{

EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_camera_controller(1280.0f / 720.0f)
{

}

void EditorLayer::OnAttach()
{
    FramebufferSpecification framebuffer_spec;
    framebuffer_spec.width = 960;
    framebuffer_spec.height = 640;
    m_framebuffer = Framebuffer::Create(framebuffer_spec);

    m_active_scene = CreateRef<Scene>();
    
    m_scene_hierarchy_panel.SetContext(m_active_scene);
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

        m_active_scene->OnViewportResize((uint32_t)m_viewport_size.x, (uint32_t)m_viewport_size.y);
    }

    // Update Camera
    if (m_viewport_focused)
        m_camera_controller.OnUpdate(ts);

    // Render
    Renderer2D::ResetStats();
    m_framebuffer->Bind();
    RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    RenderCommand::Clear();

    // Update Scene
    m_active_scene->OnUpdate(ts);

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

    m_viewport_focused = ImGui::IsWindowFocused();
    m_viewport_hovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->BlockEvents(m_viewport_focused || m_viewport_hovered);

    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    m_viewport_size = { viewport_panel_size.x, viewport_panel_size.y };

    uint32_t frame_buffer_texture = m_framebuffer->GetColorAttachmentRendererID();
    ImGui::Image(reinterpret_cast<void*>(frame_buffer_texture), ImVec2{ m_viewport_size.x, m_viewport_size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    
    // Gizmos


    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

void EditorLayer::OnEvent(Event& event)
{
    m_camera_controller.OnEvent(event);

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
