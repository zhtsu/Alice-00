#pragma once

#include "Layer.hpp"
#include "Alice/PCH.hpp"

namespace Alice
{

//
// 应用层容器
// 此容器并非一个严格的栈
// 添加新层时可以不单向添加
// 遍历时则仅允许单向遍历
//
class LayerStack
{
public:
    LayerStack();
    ~LayerStack();

    // 添加普通层，普通层被按照次序添加
    void PushLayer(Layer* layer);
    // 添加覆盖层，覆盖层会直接被添加到最上层
    void PushOverlay(Layer* overlay);

    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
    std::vector<Layer*>::iterator end() { return m_layers.end(); }

private:
    std::vector<Layer*> m_layers;
    unsigned int m_layer_insert_index = 0;
};

} // namespace Alice
