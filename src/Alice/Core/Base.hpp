#pragma once

#include "Alice/PCH.hpp"

namespace Alice
{

template<class T>
using Scope = std::unique_ptr<T>;

template<class T, class... Args>
constexpr Scope<T> CreateScope(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<class T>
using Ref = std::shared_ptr<T>;

template<class T, class... Args>
constexpr Ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

} // namespace Alice
