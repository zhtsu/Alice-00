#include "Sandbox2D.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "Alice/Debug/Instrumentor.hpp"

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
    m_checkerboard_texture = Alice::Texture2D::Create("assets/images/KFC.jpg");
    m_sprite_sheet = Alice::Texture2D::Create("assets/images/sheet.png");

    m_sprite = Alice::SubTexture2D::CreateFromCoords(m_sprite_sheet, { 0, 0 }, { 280, 215 });

    m_particle.color_begin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    m_particle.color_end = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    m_particle.size_begin = 0.5f, m_particle.size_end = 0.0f, m_particle.size_variation = 0.3f;
    m_particle.life_time = 1.0f;
    m_particle.velocity = { 0.0f, 0.0f };
    m_particle.velocity_variation = { 3.0f, 1.0f };
    m_particle.position = { 0.0f, 0.0f };

    Alice::FramebufferSpecification framebuffer_spec;
    framebuffer_spec.width = 960;
    framebuffer_spec.height = 640;
    m_framebuffer = Alice::Framebuffer::Create(framebuffer_spec);
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Alice::Timestep ts)
{
    ALICE_PROFILE_FUNCTION();

    m_camera_controller.OnUpdate(ts);

    Alice::Renderer2D::ResetStats();

    {
        ALICE_PROFILE_SCOPE("Renderer Prep");
        m_framebuffer->Bind();
        Alice::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Alice::RenderCommand::Clear();
    }

#if 0
    {
        static float rotation = 0.0f;
        rotation += ts * 20.0f;

        ALICE_PROFILE_SCOPE("Renderer Draw");
        // Scene 1
        Alice::Renderer2D::BeginScene(m_camera_controller.GetCamera());

        Alice::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, 45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
        Alice::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
        Alice::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, rotation, m_checkerboard_texture);

        Alice::Renderer2D::EndScene();

        // Scene 2
        Alice::Renderer2D::BeginScene(m_camera_controller.GetCamera());

        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = { (x + 0.5f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
                Alice::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
            }
        }

        Alice::Renderer2D::EndScene();
    }

    if (Alice::Input::IsMouseButtonPressed(ALICE_MOUSE_BUTTON_LEFT))
    {
        auto [x, y] = Alice::Input::GetMousePosition();
        auto width = Alice::Application::Get().GetWindow().GetWidth();
        auto height = Alice::Application::Get().GetWindow().GetHeight();

        auto bounds = m_camera_controller.GetBounds();
        auto pos = m_camera_controller.GetCamera().GetPosition();
        x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
        y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
        m_particle.position = { x + pos.x, y + pos.y };
        for (int i = 0; i < 5; i++)
        {
            m_particle_system.Emit(m_particle);
        }
    }
#endif
    Alice::Renderer2D::BeginScene(m_camera_controller.GetCamera());
    Alice::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_sprite);
    Alice::Renderer2D::EndScene();
    m_framebuffer->Unbind();

    m_particle_system.OnUpdate(ts);
    m_particle_system.OnRender(m_camera_controller.GetCamera());
}

void Sandbox2D::OnImGuiRender()
{
    ALICE_PROFILE_FUNCTION();

    static bool is_dockspace_open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
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

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &is_dockspace_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
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
                Alice::Application::Get().Close();

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("View");
    
    // auto stats = Alice::Renderer2D::GetStats();
    // ImGui::Text("Renderer2D Stats:");
    // ImGui::Text("Draw Calls: %d", stats.draw_calls);
    // ImGui::Text("Quads: %d", stats.quad_count);
    // ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    // ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    // ImGui::ColorEdit4("Square Color", glm::value_ptr(m_square_color));

    ImGui::Image((void*)m_framebuffer->GetColorAttachmentRendererID(), ImVec2{ 960.0f, 640.0f });
    ImGui::End();

    ImGui::End();
}

void Sandbox2D::OnEvent(Alice::Event& event)
{
    m_camera_controller.OnEvent(event);
}
