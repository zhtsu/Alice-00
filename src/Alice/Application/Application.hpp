#pragma once

#include "Alice/PCH.hpp"
#include "Alice/Window/Window.hpp"

namespace Alice
{

// 引擎主程序
class Application
{
public:
    Application();
    ~Application();

    void OnEvent(Event& event);

    void Run();

private:
    std::unique_ptr<Window> m_window;
    bool m_running;
};

} // namespace Alice

