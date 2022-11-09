#pragma once

#include <direct.h>
#include <string>

namespace Alice
{

// 文件种类
// 用来决定文件夹路径
// 确保 Alice 的文件夹结构从未被改变过
// 否则此枚举类型将失效
enum class FileType
{
    Shader,
    Image,
    Sound,
    Music
};

// 路径类
// 这是一个静态工具类
// 用来获取资源路径
class PathHelper
{
public:
// 获取当前工作路径
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

// 获取 Alice 资源的相对路径
static std::string GeneratePath(FileType filetype, std::string filename)
{
    std::string filepath = GetCurrentPath();
    switch (filetype)
    {
    case FileType::Shader:
        filepath += "/shaders/";
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
