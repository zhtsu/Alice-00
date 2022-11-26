#pragma once

#include "Alice/Application/Application.hpp"
#include "Sandbox2D.hpp"

class SandboxApp : public Alice::Application
{
public:
    SandboxApp(const std::string& name = "Sandbox") : Application(name)
    {
        PushLayer(new Sandbox2D);
    }
};
