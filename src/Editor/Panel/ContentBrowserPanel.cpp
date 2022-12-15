#include "ContentBrowserPanel.hpp"

#include <imgui.h>
#include "Alice/Renderer/Texture.hpp"

namespace Alice
{

static const std::filesystem::path k_assets_path = "assets";

ContentBrowserPanel::ContentBrowserPanel()
    : m_current_directory(k_assets_path)
{
    m_folder_icon = Texture2D::Create("resources/icons/folder.png");
    m_txt_icon = Texture2D::Create("resources/icons/txt.png");
    m_back_icon = Texture2D::Create("resources/icons/back.png");
}

void ContentBrowserPanel::OnImGuiRender()
{
    ImGui::Begin("Content Browser");

    if (m_current_directory != std::filesystem::path(k_assets_path))
    {
        bool is_back = ImGui::ImageButton(
            reinterpret_cast<ImTextureID>(m_back_icon->GetRendererID()),
            { 10, 10 }, { 0, 1 }, { 1, 0 });
        if (is_back)
        {
            m_current_directory = m_current_directory.parent_path();
        }
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::ImageButton(
            reinterpret_cast<ImTextureID>(m_back_icon->GetRendererID()),
            { 10, 10 }, { 0, 1 }, { 1, 0 });
        ImGui::PopStyleColor();
    }

    static float padding = 16.0f;
    static float thumbnail_size = 64.0f;
    float cell_size = thumbnail_size + padding;

    float panel_width = ImGui::GetContentRegionAvail().x;
    int column_count = (int)(panel_width / cell_size);
    if (column_count < 1)
        column_count = 1;

    ImGui::Columns(column_count, 0, false);

    for (auto& iter : std::filesystem::directory_iterator(m_current_directory))
    {
        const auto& path = iter.path();
        auto relative_path = std::filesystem::relative(path, k_assets_path);
        std::string filename_string = relative_path.filename().string();

        Ref<Texture2D> icon = iter.is_directory() ? m_folder_icon : m_txt_icon;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->GetRendererID()),
            { thumbnail_size, thumbnail_size }, { 0, 1 }, { 1, 0 });
        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (iter.is_directory())
                m_current_directory /= path.filename();
        }

        ImGui::TextWrapped(filename_string.c_str());

        ImGui::NextColumn();
    }

    ImGui::End();
}

} // namespace Alice
