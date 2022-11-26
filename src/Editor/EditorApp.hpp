#pragma once

#include "Alice/Application/Application.hpp"
#include "EditorLayer.hpp"

class EditorApp : public Alice::Application
{
public:
    EditorApp(const std::string& name = "Alice Editor") : Application(name)
    {
        PushLayer(new Alice::EditorLayer());
    }
};
