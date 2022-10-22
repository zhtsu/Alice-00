#include "Alice/Application/Application.hpp"

int main(int argc, char* argv[])
{
    auto app = new Alice::Application();
    app->Run();
    delete app;

    return 0;
}
