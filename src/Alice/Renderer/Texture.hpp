#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Core/Core.hpp"

namespace Alice
{

//
// 平台无关的纹理
//
class Texture
{
public:
    virtual ~Texture() = default;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void SetData(void* data, uint32_t size) = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind() = 0;
};

class Texture2D : public Texture
{
public:
    static Ref<Texture2D> Create(uint32_t width, uint32_t height);
    static Ref<Texture2D> Create(const std::string& path);
};

} // namespace Alice
