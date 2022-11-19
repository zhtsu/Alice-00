#pragma once

namespace Alice
{

//
// 平台无关的渲染上下文
//
class GraphicsContext
{
public:
    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;
};

} // namespace Alice
