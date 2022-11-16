#include "Sandbox2D.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include "Alice/Common/Timer.hpp"

Sandbox2D::Sandbox2D()
    :Layer("Sandbox2D"), m_camera_controller(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
    std::string img_path = Alice::PathHelper::GeneratePath(
        Alice::FileType::Image,
        "KFC.jpg"
    );
    m_checkerboard_texture = Alice::Texture2D::Create(img_path);
}

void Sandbox2D::OnDetach()
{

}

#define PROFILE_SCOPE(name) Alice::Timer timer##__LINE__(name, [&](ProfileResult profile_result) { m_profile_results.push_back(profile_result); })

void Sandbox2D::OnUpdate(Alice::Timestep ts)
{
    PROFILE_SCOPE("Sandbox2D::OnUpdate");

    {
        PROFILE_SCOPE("Camera::OnUpdate");
        m_camera_controller.OnUpdate(ts);
    }
    

    Alice::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Alice::RenderCommand::Clear();

    Alice::Renderer2D::BeginScene(m_camera_controller.GetCamera());

    Alice::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
    Alice::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
    Alice::Renderer2D::DrawQuad({ 0.2f, 0.5f }, { 10.0f, 10.0f }, m_checkerboard_texture);

    Alice::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_square_color));

    for (auto& result : m_profile_results)
    {
        char label[50];
        strcpy(label, "%.3fms ");
        strcat(label, result.name);
        ImGui::Text(label, result.time);
    }
    m_profile_results.clear();

    ImGui::End();
}

void Sandbox2D::OnEvent(Alice::Event& event)
{
    m_camera_controller.OnEvent(event);
}
