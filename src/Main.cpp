#include "Alice/Application/Application.hpp"
#include "Alice/Layer/Layer.hpp"
#include "Alice/UI/ImGuiLayer.hpp"
#include "Alice/Log/Log.hpp"

class ExampleLayer : public Alice::Layer
{
public:
    ExampleLayer() : Layer("Example")
    {

    }

    void OnUpdate() override
    {
        ALICE_INFO("ExampleLayer::Update");
    }

    void OnEvent(Alice::Event& event) override
    {
        ALICE_TRACE("ExampleLayer::OnEvent", event);
    }
};

int main(int argc, char* argv[])
{
    auto app = new Alice::Application();
    app->PushLayer(new ExampleLayer());
    app->PushLayer(new Alice::ImGuiLayer());
    app->Run();
    delete app;

    return 0;
}
