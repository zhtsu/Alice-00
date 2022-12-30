#include "Entity.hpp"

#include "Components.hpp"

namespace Alice
{

uint64_t Entity::GetUUID()
{
    return GetComponent<IDComponent>().id;
}

const std::string& Entity::GetName()
{
    return GetComponent<TagComponent>().tag;
}

} // namespace Alice
