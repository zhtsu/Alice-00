#pragma once

#include <string>
#include <nfd.h>
#include "Alice/Log/Log.hpp"

namespace Alice
{

class FileDialogs
{
public:
    static std::string OpenFile(const char* filter)
    {
        nfdchar_t* out_path = NULL;
        nfdresult_t result = NFD_OpenDialog(filter, NULL, &out_path);
        if (result == NFD_OKAY)
        {
            ALICE_INFO("FileDialogs::OpenFile: Success! File: {}", out_path);
            std::string ret(out_path);
            free(out_path);
            return ret;
        }
        else if (result == NFD_CANCEL)
        {
            ALICE_INFO("FileDialogs::OpenFile: User pressed cancel.");
        }
        else
        {
            ALICE_ERROR("FileDialogs::OpenFile: Error! {}", NFD_GetError());
        }

        return std::string();
    }

    static std::string SaveFile(const char* filter)
    {
        nfdchar_t* save_path = NULL;
        nfdresult_t result = NFD_SaveDialog(filter, NULL, &save_path);
        if (result == NFD_OKAY)
        {
            ALICE_INFO("FileDialogs::SaveFile: Success! File: {}", save_path);
            std::string ret(save_path);
            free(save_path);
            return ret;
        }
        else if (result == NFD_CANCEL)
        {
            ALICE_INFO("FileDialogs::SaveFile: User pressed cancel.");
        }
        else
        {
            ALICE_ERROR("FileDialogs::SaveFile: Error! {}", NFD_GetError());
        }

        return std::string();
    }
};

} // namespace Alice
