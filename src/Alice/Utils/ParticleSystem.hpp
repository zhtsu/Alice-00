#pragma once

#include "glm/glm.hpp"
#include "Alice/PCH.hpp"
#include "Alice/Core/Timestep.hpp"
#include "Alice/Renderer/Shader.hpp"
#include "Alice/Renderer/OrthographicCamera.hpp"
#include "glad/glad.h"

namespace Alice
{

struct ParticleProps
{
    glm::vec2 position;
    glm::vec2 velocity, velocity_variation;
    glm::vec4 color_begin, color_end;
    float size_begin, size_end, size_variation;
    float life_time = 1.0f;
};

class ParticleSystem
{
public:
    ParticleSystem(uint32_t max_particles = 10000);

    void OnUpdate(Timestep ts);
    void OnRender(OrthographicCamera& camera);

    void Emit(const ParticleProps& particle_props);

private:
    struct Particle
    {
        glm::vec2 position;
        float rotation = 0.0f;
        glm::vec2 velocity;
        glm::vec4 color_begin, color_end;
        float size_begin, size_end;
        
        float life_time = 1.0f;
        float life_remaining = 0.0f;

        bool active = false;
    };

    std::vector<Particle> m_particle_pool;
    uint32_t m_pool_index;
};

} // namespace Alice
