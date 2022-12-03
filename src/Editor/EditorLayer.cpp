#include "EditorLayer.hpp"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "Alice/Debug/Instrumentor.hpp"

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

    m_square_entity = m_active_scene->CreateEntity("Square");
    m_square_entity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

    m_camera_entity = m_active_scene->CreateEntity("Camera");
    m_camera_entity.AddComponent<CameraComponent>();

    m_second_camera = m_active_scene->CreateEntity("Clip-Space");
    auto& sc = m_second_camera.AddComponent<CameraComponent>();
    sc.is_primary = false;
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

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
                Application::Get().Close();

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
    
    ImGui::Begin("Profile");
    
    auto stats = Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.draw_calls);
    ImGui::Text("Quads: %d", stats.quad_count);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    if (m_square_entity)
    {
        ImGui::Separator();
        auto& tag = m_square_entity.GetComponent<TagComponent>().tag;
        ImGui::Text("%s", tag.c_str());
        auto& square_color = m_square_entity.GetComponent<SpriteRendererComponent>().color;
        ImGui::ColorEdit4("Square Color", glm::value_ptr(square_color));
        ImGui::Separator();
    }

    ImGui::DragFloat3(
        "Camera Transform",
        glm::value_ptr(m_camera_entity.GetComponent<TransformComponent>().transform[3])
    );
    
    if (ImGui::Checkbox("Camera A", &m_primary_camera))
    {
        m_camera_entity.GetComponent<CameraComponent>().is_primary = m_primary_camera;
        m_second_camera.GetComponent<CameraComponent>().is_primary = !m_primary_camera;
    }

    {
        auto& camera = m_second_camera.GetComponent<CameraComponent>().camera;
        float ortho_size = camera.GetOrthographicSize();
        if (ImGui::DragFloat("Sceond Camera Otrho Size", &ortho_size))
            camera.SetOrthographicSize(ortho_size);
    }

    ImGui::End();

    // Viewport
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    ImGui::Begin("Viewport");

    m_viewport_focused = ImGui::IsWindowFocused();
    m_viewport_hovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->BlockEvents(m_viewport_focused || m_viewport_hovered);

    ImVec2 viewport_panel_size = ImGui::GetContentRegionAvail();
    m_viewport_size = { viewport_panel_size.x, viewport_panel_size.y };

    // @TODO:
    // Framebuffer disappear after window was minimized.
    uint32_t frame_buffer_texture = m_framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)frame_buffer_texture, ImVec2{ m_viewport_size.x, m_viewport_size.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

void EditorLayer::OnEvent(Event& event)
{
    m_camera_controller.OnEvent(event);
}

} // namespace Alice
