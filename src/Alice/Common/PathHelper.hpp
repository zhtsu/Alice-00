#pragma once

#include <direct.h>
#include <string>

namespace Alice
{

///
/// File type for Alice
/// Used to select file's father path
///
enum class FileType
{
    Shader,
    Image,
    Sound,
    Music
};


class PathHelper
{
public:
static std::string GetCurrentPath()
{
    const int length = 512;
    char buffer[length];
    getcwd(buffer, 512);
    std::string current_path = buffer;

    int pos = current_path.find("\\");
    while (pos != current_path.npos)
    {
        current_path.replace(pos, 1, "/");
        pos = current_path.find("\\");
    }

    return current_path; 
}

///
/// Generate local file path by file type for Alice
///
static std::string GeneratePath(FileType filetype, const std::string& filename)
{
    std::string filepath = GetCurrentPath();
    
    switch (filetype)
    {
    case FileType::Shader:
        filepath += "/assets/shaders/";
        break;

    case FileType::Image:
        filepath += "/assets/images/";
        break;

    case FileType::Sound:
        filepath += "/assets/sounds/";
        break;

    case FileType::Music:
        filepath += "/assets/musics/";
        break;
    
    default:
        break;
    }
    filepath += filename;

    return filepath.c_str();
}

};

} // namespace Alice
