#pragma once

#include "Alice/Events/Event.hpp"
#include "Alice/Core/Timestep.hpp"

namespace Alice
{

//
// 应用层
//
class Layer
{
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    // 初始化层
    virtual void OnAttach() {}
    // 清理层
    virtual void OnDetach() {}
    // 更新层
    virtual void OnUpdate(Timestep ts) {}
    // 渲染 imgui 层
    virtual void OnImGuiRender() {}
    // 事件回调函数
    virtual void OnEvent(Event& event) {}

    inline const std::string& GetName() const { return m_debug_name; }

protected:
    std::string m_debug_name;
};

} // namespace Alice
