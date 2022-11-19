#pragma once

#include "Alice/PCH.hpp"

namespace Alice
{

// 唯一智能指针类型重命名
template<class T>
using Scope = std::unique_ptr<T>;

// 共享智能指针类型重命名
template<class T>
using Ref = std::shared_ptr<T>;

template<class T, class... Args>
constexpr Ref<T> CreateRef(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

} // namespace Alice
