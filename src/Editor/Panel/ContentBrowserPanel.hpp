#pragma once

#include <filesystem>

namespace Alice
{

class ContentBrowserPanel
{
public:
    ContentBrowserPanel();

    void OnImGuiRender();

private:
    std::filesystem::path m_current_directory;
};

} // namespace Alice
