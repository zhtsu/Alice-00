#pragma once

#include "Alice/Alice.hpp"

namespace Alice
{

class EditorLayer : public Layer
{
public:
    EditorLayer();
    virtual ~EditorLayer() = default;

    void OnAttach() override;
    void OnDetach() override;

    void OnUpdate(Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Event& event) override;

private:
    OrthographicCameraController m_camera_controller;

    // Temp
    Ref<VertexArray> m_square_vao;
    Ref<Shader> m_flat_color_shader;

    Ref<Texture2D> m_checkerboard_texture;
    Ref<Texture2D> m_sprite_sheet;
    Ref<SubTexture2D> m_sprite;
    Ref<Framebuffer> m_framebuffer;

    glm::vec4 m_square_color = { 0.2f, 0.3f, 0.8f, 1.0f };
    
    glm::vec2 m_viewport_size = { 0.0f, 0.0f };

    ParticleSystem m_particle_system;
    ParticleProps m_particle;
};

} // namespace Alice
