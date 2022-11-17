#include "Alice/Alice.hpp"
#include "Sandbox/Sandbox2D.hpp"

int main(int argc, char* argv[])
{
    ALICE_PROFILE_BEGIN_SESSION("Startup", "AliceProfile-Start.json");
    Alice::Application* app = new Alice::Application();
    ALICE_PROFILE_END_SESSION();

    app->PushLayer(new Sandbox2D());
     
    ALICE_PROFILE_BEGIN_SESSION("Runtime", "AliceProfile-Runtime.json");
    app->Run();
    ALICE_PROFILE_END_SESSION();

    ALICE_PROFILE_BEGIN_SESSION("Shutdown", "AliceProfile-Shutdown.json");
    delete app;
    ALICE_PROFILE_END_SESSION();

    return 0;
}
