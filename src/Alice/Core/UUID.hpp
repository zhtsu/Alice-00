#pragma once

#include "Alice/PCH.hpp"

namespace Alice
{

class UUID
{
public:
    UUID();
    UUID(uint64_t uuid);
    UUID(const UUID&) = default;

    operator uint64_t() const { return m_uuid; }

private:
    uint64_t m_uuid;
};

} // namespace Alice

namespace std
{

template<>
struct hash<Alice::UUID>
{
    std::size_t operator()(const Alice::UUID& uuid) const
    {
        return hash<uint64_t>()((uint64_t)uuid);
    }
};

} // namespace std
