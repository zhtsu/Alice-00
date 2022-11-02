#include "Alice/Alice.hpp"

class ExampleLayer : public Alice::Layer
{
public:
    ExampleLayer() : Layer("Example")
    {
        
    }

    void OnUpdate() override
    {
        if (Alice::Input::IsKeyPressed(ALICE_KEY_TAB))
        {
            ALICE_TRACE("Tab key is pressed!");
        }
    }

    void OnEvent(Alice::Event& event) override
    {
        
    }
};

int main(int argc, char* argv[])
{
    auto app = new Alice::Application();
    app->PushLayer(new ExampleLayer());
    app->Run();
    delete app;

    return 0;
}
