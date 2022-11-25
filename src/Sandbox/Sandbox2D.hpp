#pragma once

#include "Alice/Alice.hpp"

class Sandbox2D : public Alice::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Alice::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Alice::Event& event) override;

private:
    Alice::OrthographicCameraController m_camera_controller;

    // Temp
    Alice::Ref<Alice::VertexArray> m_square_vao;
    Alice::Ref<Alice::Shader> m_flat_color_shader;

    Alice::Ref<Alice::Texture2D> m_checkerboard_texture;
    Alice::Ref<Alice::Texture2D> m_sprite_sheet;
    Alice::Ref<Alice::SubTexture2D> m_sprite;
    Alice::Ref<Alice::Framebuffer> m_framebuffer;

    glm::vec4 m_square_color = { 0.2f, 0.3f, 0.8f, 1.0f };

    Alice::ParticleSystem m_particle_system;
    Alice::ParticleProps m_particle;
};
