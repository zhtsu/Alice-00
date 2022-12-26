#include "Entity.hpp"

#include "Components.hpp"

namespace Alice
{

uint64_t Entity::GetUUID()
{
    return GetComponent<IDComponent>().id;
}

} // namespace Alice
