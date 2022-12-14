#pragma once

#include "Layer.hpp"
#include "Alice/PCH.hpp"

namespace Alice
{

class LayerStack
{
public:
    LayerStack();
    ~LayerStack();

    // Add a layer in sequence
    void PushLayer(Layer* layer);
    // Add a layer in uppest
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
