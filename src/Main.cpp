#include "Alice/Alice.hpp"
#include "Editor/EditorApp.hpp"
#include "Sandbox/SandboxApp.hpp"

int main(int argc, char* argv[])
{
    //
    ALICE_PROFILE_BEGIN_SESSION("Startup", "AliceProfile-Start.json");
    
    Alice::Application* app = new EditorApp();
    // 测试程序
    // Alice::Application* app = new SandboxApp();

    ALICE_PROFILE_END_SESSION();

    //
    ALICE_PROFILE_BEGIN_SESSION("Runtime", "AliceProfile-Runtime.json");
    
    app->Run();
    
    ALICE_PROFILE_END_SESSION();

    //
    ALICE_PROFILE_BEGIN_SESSION("Shutdown", "AliceProfile-Shutdown.json");
    
    delete app;
    
    ALICE_PROFILE_END_SESSION();

    return 0;
}
