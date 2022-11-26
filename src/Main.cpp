#include "Alice/Alice.hpp"
#include "Editor/EditorLayer.hpp"

int main(int argc, char* argv[])
{
    ALICE_PROFILE_BEGIN_SESSION("Startup", "AliceProfile-Start.json");
    Alice::Application* app = new Alice::Application("Alice Editor");
    ALICE_PROFILE_END_SESSION();

    app->PushLayer(new Alice::EditorLayer());
     
    ALICE_PROFILE_BEGIN_SESSION("Runtime", "AliceProfile-Runtime.json");
    app->Run();
    ALICE_PROFILE_END_SESSION();

    ALICE_PROFILE_BEGIN_SESSION("Shutdown", "AliceProfile-Shutdown.json");
    delete app;
    ALICE_PROFILE_END_SESSION();

    return 0;
}
