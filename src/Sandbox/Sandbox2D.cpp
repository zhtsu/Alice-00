#include "Sandbox2D.hpp"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "Alice/Debug/Instrumentor.hpp"

using namespace Alice;

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
    m_checkerboard_texture = Texture2D::Create("assets/images/KFC.jpg");
    m_sprite_sheet = Texture2D::Create("assets/images/sheet.png");

    m_sprite = SubTexture2D::CreateFromCoords(m_sprite_sheet, { 0, 0 }, { 280, 215 });

    m_particle.color_begin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    m_particle.color_end = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    m_particle.size_begin = 0.5f, m_particle.size_end = 0.0f, m_particle.size_variation = 0.3f;
    m_particle.life_time = 1.0f;
    m_particle.velocity = { 0.0f, 0.0f };
    m_particle.velocity_variation = { 3.0f, 1.0f };
    m_particle.position = { 0.0f, 0.0f };

    FramebufferSpecification framebuffer_spec;
    framebuffer_spec.width = 1280;
    framebuffer_spec.height = 720;
    m_framebuffer = Framebuffer::Create(framebuffer_spec);
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Timestep ts)
{
    ALICE_PROFILE_FUNCTION();

    m_camera_controller.OnUpdate(ts);

    Renderer2D::ResetStats();

    {
        ALICE_PROFILE_SCOPE("Renderer Prep");
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::Clear();
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 20.0f;

        ALICE_PROFILE_SCOPE("Renderer Draw");
        // Scene 1
        Renderer2D::BeginScene(m_camera_controller.GetCamera());

        Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, 45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
        Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });

        Renderer2D::EndScene();

        // Scene 2
        Renderer2D::BeginScene(m_camera_controller.GetCamera());

        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = { (x + 0.5f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
                Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
            }
        }

        Renderer2D::EndScene();
    }

    if (Input::IsMouseButtonPressed(ALICE_MOUSE_BUTTON_LEFT))
    {
        auto [x, y] = Input::GetMousePosition();
        auto width = Application::Get().GetWindow().GetWidth();
        auto height = Application::Get().GetWindow().GetHeight();

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

    m_particle_system.OnUpdate(ts);
    m_particle_system.OnRender(m_camera_controller.GetCamera());
}

void Sandbox2D::OnImGuiRender()
{
    ALICE_PROFILE_FUNCTION();

    ImGui::Begin("View");
    
    auto stats = Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.draw_calls);
    ImGui::Text("Quads: %d", stats.quad_count);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_square_color));

    ImGui::End();
}

void Sandbox2D::OnEvent(Event& event)
{
    m_camera_controller.OnEvent(event);
}
