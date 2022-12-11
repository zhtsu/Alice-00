#include "ParticleSystem.hpp"

#include "Random.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/compatibility.hpp"
#include "Alice/Renderer/Renderer2D.hpp"

namespace Alice
{

ParticleSystem::ParticleSystem(uint32_t max_particles)
    : m_pool_index(max_particles - 1)
{
    m_particle_pool.resize(max_particles);
}

void ParticleSystem::OnUpdate(Timestep ts)
{
    for (auto& particle : m_particle_pool)
    {
        if (!particle.active)
        {
            continue;
        }

        if (particle.life_remaining <= 0.0f)
        {
            particle.active = false;
            continue;
        }

        particle.life_remaining -= ts;
        particle.position += particle.velocity * (float)ts;
        particle.rotation += 0.01f * ts;
    }
}

void ParticleSystem::OnRender(OrthographicCamera& camera)
{
    Renderer2D::BeginScene(camera);
    for (auto& particle : m_particle_pool)
    {
        if (!particle.active)
        {
            continue;
        }

        float life = particle.life_remaining / particle.life_time;
        glm::vec4 color = glm::lerp(particle.color_end, particle.color_begin, life);
        float size = glm::lerp(particle.size_end, particle.size_begin, life);
        glm::vec3 position = { particle.position.x, particle.position.y, 0.2f };
        Renderer2D::DrawRotatedQuad(position, { size, size }, particle.rotation, color);
    }
    Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particle_props)
{
    Particle& particle = m_particle_pool[m_pool_index];
    particle.active = true;
    particle.position = particle_props.position;
    particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

    particle.velocity = particle_props.velocity;
    particle.velocity.x += particle_props.velocity_variation.x * (Random::Float() - 0.5f);
    particle.velocity.y += particle_props.velocity_variation.y * (Random::Float() - 0.5f);

    particle.color_begin = particle_props.color_begin;
    particle.color_end = particle_props.color_end;

    particle.life_time = particle_props.life_time;
    particle.life_remaining = particle_props.life_time;
    particle.size_begin = particle_props.size_begin + particle_props.size_variation * (Random::Float() - 0.5f);
    particle.size_end = particle_props.size_end;

    m_pool_index = --m_pool_index % m_particle_pool.size();
}

} // namespace Alice
