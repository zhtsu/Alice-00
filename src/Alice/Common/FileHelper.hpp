#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include "Alice/Log/Log.hpp"

namespace Alice
{

// 文件类
// 这是一个工具类
// 用来从文件中读取字符串
class FileHelper
{
public:
// 读取文件中的内容并以字符串形式返回
static std::string Read(const char* filepath)
{
    std::string content;
    std::ifstream file_stream;

    file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		file_stream.open(filepath);
        std::stringstream string_stream;
        string_stream << file_stream.rdbuf();
        file_stream.close();
        content = string_stream.str();
	}
	catch(const std::exception& e)
	{
		ALICE_ERROR("Read file {} failed: {}", filepath, e.what());
	}
	
    return content;
}

// 向文件中写入内容
// \param filepath 文件路径
// \param content  需要写入的内容
static void Write(const char* filepath, std::string content)
{
    std::ofstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        file.open(filepath, std::ios::binary);
        file << content;
        file.close();
    }
    catch(const std::exception& e)
    {
        ALICE_ERROR("Write file {} failed: {}", filepath, e.what());
    }
}

};

} // namespace Alice
