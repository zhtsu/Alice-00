#pragma once

#include <filesystem>
#include "Alice/Core/Base.hpp"

namespace Alice
{

class Texture2D;

class ContentBrowserPanel
{
public:
    ContentBrowserPanel();

    void OnImGuiRender();

private:
    std::filesystem::path m_current_directory;
    Ref<Texture2D> m_folder_icon;
    Ref<Texture2D> m_txt_icon;
    Ref<Texture2D> m_back_icon;
    // Ref<Texture2D> m_source_icon;
    // Ref<Texture2D> m_image_icon;
};

} // namespace Alice
