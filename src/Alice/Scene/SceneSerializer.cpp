#include "SceneSerializer.hpp"

#include <yaml-cpp/yaml.h>
#include "Alice/Log/Log.hpp"
#include "Entity.hpp"
#include "Components.hpp"

namespace YAML
{

template<>
struct convert<glm::vec3>
{
    static Node encode(const glm::vec3& rhs)
    {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        return node;
    }

    static bool decode(const Node& node, glm::vec3& rhs)
    {
        if (!node.IsSequence() || node.size() != 3)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        return true;
    }
};

template<>
struct convert<glm::vec4>
{
    static Node encode(const glm::vec4& rhs)
    {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.push_back(rhs.w);
        return node;
    }

    static bool decode(const Node& node, glm::vec4& rhs)
    {
        if (!node.IsSequence() || node.size() != 4)
            return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        rhs.w = node[3].as<float>();
        return true;
    }
};

} // namespace YAML

namespace Alice
{

SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
    : m_scene(scene)
{

}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vec3f)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << vec3f.x << vec3f.y << vec3f.z << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vec4f)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << vec4f.x << vec4f.y << vec4f.z << vec4f.w << YAML::EndSeq;
    return out;
}

static void SerializeEntity(YAML::Emitter& out, Entity entity)
{
    out << YAML::BeginMap;
    out << YAML::Key << "Entity" << YAML::Value << "121212121";

    // TagComponent
    if (entity.HasComponent<TagComponent>())
    {
        out << YAML::Key << "TagComponent";
        out << YAML::BeginMap;

        auto& tag = entity.GetComponent<TagComponent>().tag;
        out << YAML::Key << "Tag" << YAML::Value << tag;

        out << YAML::EndMap;
    }

    // TransformComponent
    if (entity.HasComponent<TransformComponent>())
    {
        out << YAML::Key << "TransformComponent";
        out << YAML::BeginMap;

        auto& transform = entity.GetComponent<TransformComponent>();
        out << YAML::Key << "Translation" << YAML::Value << transform.translation;
        out << YAML::Key << "Rotation" << YAML::Value << transform.rotation;
        out << YAML::Key << "Scale" << YAML::Value << transform.scale;

        out << YAML::EndMap;
    }

    // CameraComponent
    if (entity.HasComponent<CameraComponent>())
    {
        out << YAML::Key << "CameraComponent";
        out << YAML::BeginMap;

        auto& camera_component = entity.GetComponent<CameraComponent>();
        auto& camera = camera_component.camera;
        out << YAML::Key << "Camera" << YAML::Value;
        out << YAML::BeginMap;
        out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
        out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFov();
        out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetOrthographicNearClip();
        out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetOrthographicFarClip();
        out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
        out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
        out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
        out << YAML::EndMap;
        out << YAML::Key << "Primary" << YAML::Value << camera_component.is_primary;
        out << YAML::Key << "FixedAspectRatio" << YAML::Value << camera_component.is_fixed_aspect_ratio;

        out << YAML::EndMap;
    }

    // SpriteRendererComponent
    if (entity.HasComponent<SpriteRendererComponent>())
    {
        out << YAML::Key << "SpriteRendererComponent";
        out << YAML::BeginMap;

        auto& sprite = entity.GetComponent<SpriteRendererComponent>();
        out << YAML::Key << "Color" << YAML::Value << sprite.color;

        out << YAML::EndMap;
    }

    out << YAML::EndMap;
}

void SceneSerializer::Serialize(const std::string& filepath)
{
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << "Untitled";
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
    m_scene->m_registry.each([&](auto entity_id){
        Entity entity = { entity_id, m_scene.get() };
        if (!entity)
            return;

        SerializeEntity(out, entity);
    });
    out << YAML::EndSeq;
    out << YAML::EndMap;

    std::ofstream fout(filepath);
    fout << out.c_str();
}

void SceneSerializer::SerializeRuntime(const std::string& filepath)
{
    ALICE_ASSERT(false, "SceneSerializer::SerializeRuntime: Not implemented!");
}

bool SceneSerializer::Deserialize(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::stringstream str_stream;
    str_stream << stream.rdbuf();

    YAML::Node data = YAML::Load(str_stream.str());
    if (!data["Scene"])
        return false;

    std::string scene_name = data["Scene"].as<std::string>();
    ALICE_TRACE("Deserializing scene '{}'", scene_name);

    auto entities = data["Entities"];
    if (entities)
    {
        for (auto entity : entities)
        {
            // @TODO:
            // UUID wait to realize
            uint64_t uuid = entity["Entity"].as<uint64_t>();

            std::string name;
            auto tag_component = entity["TagComponent"];
            if (tag_component)
                name = tag_component["Tag"].as<std::string>();

            ALICE_TRACE("Deserialized entity with ID = {}, name = {}", uuid, name);

            Entity deserialize_entity = m_scene->CreateEntity(name);
            // TransformComponent
            auto transform_component = entity["TransformComponent"];
            if (transform_component)
            {
                // Entities always have TransformComponent
                auto& transform = deserialize_entity.GetComponent<TransformComponent>();
                transform.translation = transform_component["Translation"].as<glm::vec3>();
                transform.rotation = transform_component["Rotation"].as<glm::vec3>();
                transform.scale = transform_component["Scale"].as<glm::vec3>();
            }
            // CameraComponent
            auto camera_component = entity["CameraComponent"];
            if (camera_component)
            {
                auto& camera_comp = deserialize_entity.AddComponent<CameraComponent>();
                auto camera_props = camera_component["Camera"];
                camera_comp.camera.SetProjectionType((SceneCamera::ProjectionType)camera_props["ProjectionType"].as<int>());
                camera_comp.camera.SetPerspectiveVerticalFov(camera_props["PerspectiveFOV"].as<float>());
                camera_comp.camera.SetPerspectiveNearClip(camera_props["PerspectiveNear"].as<float>());
                camera_comp.camera.SetPerspectiveFarClip(camera_props["PerspectiveFar"].as<float>());
                camera_comp.camera.SetOrthographicSize(camera_props["OrthographicSize"].as<float>());
                camera_comp.camera.SetOrthographicNearClip(camera_props["OrthographicNear"].as<float>());
                camera_comp.camera.SetOrthographicFarClip(camera_props["OrthographicFar"].as<float>());
                camera_comp.is_primary = camera_component["Primary"].as<bool>();
                camera_comp.is_fixed_aspect_ratio = camera_component["FixedAspectRatio"].as<bool>();
            }
            // SpriteRendererComponent
            auto sprite_renderer_component = entity["SpriteRendererComponent"];
            if (sprite_renderer_component)
            {
                auto& sprite = deserialize_entity.AddComponent<SpriteRendererComponent>();
                sprite.color = sprite_renderer_component["Color"].as<glm::vec4>();
            }
        }
    }

    return true;
}

bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
{
    ALICE_ASSERT(false, "SceneSerializer::DeserializeRuntime: Not implemented!");
    return false;
}

} // namespace Alice