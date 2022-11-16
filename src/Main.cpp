#include "Sandbox/Sandbox2D.hpp"

int main(int argc, char* argv[])
{
    auto app = new Alice::Application();
    app->PushLayer(new Sandbox2D());
    app->Run();
    delete app;

    return 0;
}
