#include "Application.hpp"
#include "Alice/Events/ApplicationEvent.hpp"
#include "Alice/Log/Log.hpp"

namespace Alice
{

Application::Application()
{
    Alice::Log::Init();
}

Application::~Application()
{

}


void Application::Run()
{
    WindowResizeEvent e(960, 640);
    
    ALICE_TRACE(e);

    while(true);
}

} // namespace Alice
