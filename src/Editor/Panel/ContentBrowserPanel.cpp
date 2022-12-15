#include "ContentBrowserPanel.hpp"

#include <imgui.h>

namespace Alice
{

static const std::filesystem::path k_assets_path = "assets";

ContentBrowserPanel::ContentBrowserPanel()
    : m_current_directory(k_assets_path)
{

}

void ContentBrowserPanel::OnImGuiRender()
{
    ImGui::Begin("Content Browser");

    if (m_current_directory != std::filesystem::path(k_assets_path))
    {
        if (ImGui::Button("<-"))
        {
            m_current_directory = m_current_directory.parent_path();
        }
    }

    for (auto& iter : std::filesystem::directory_iterator(m_current_directory))
    {
        const auto& path = iter.path();
        auto relative_path = std::filesystem::relative(path, k_assets_path);
        std::string filename_string = relative_path.filename().string();
        if (iter.is_directory())
        {
            if (ImGui::Button(filename_string.c_str()))
            {
                m_current_directory /= path.filename();
            }
        }
        else
        {
            if (ImGui::Button(filename_string.c_str()))
            {

            }
        }
    }

    ImGui::End();
}

} // namespace Alice
