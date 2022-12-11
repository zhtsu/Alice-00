#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include "Alice/Log/Log.hpp"

namespace Alice
{

///
/// Read data from local file
///
class FileHelper
{
public:
static std::string Read(const std::string& filepath)
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
		ALICE_ERROR("Read: file {} failed: {}", filepath, e.what());
	}
	
    return content;
}

static void Write(const std::string& filepath, const std::string& content)
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
        ALICE_ERROR("Write: file {} failed: {}", filepath, e.what());
    }
}

};

} // namespace Alice
