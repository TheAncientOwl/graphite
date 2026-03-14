/// --------------------------------------------------------------------------
///                     Copyright (c) by ImGui-Lab 2026
/// --------------------------------------------------------------------------
/// @license https://github.com/TheAncientOwl/imgui-lab/blob/main/LICENSE
///
/// @file DynamicLibrary.hpp
/// @author Alexandru Delegeanu
/// @version 0.1
/// @brief DLL utility class.
///

#pragma once

#include <filesystem>
#include <iostream>
#include <string>

namespace Graphite::Core::Plugin {

#if defined(_WIN32)
#include <windows.h>
using LibHandle = HMODULE;
#else
#include <dlfcn.h>
using LibHandle = void*;
#endif

class DynamicLibrary
{
private:
    DynamicLibrary(const char* path)
    {
#if defined(_WIN32)
        handle = LoadLibraryA(path.c_str());
#else
        handle = dlopen(path, RTLD_LAZY);
#endif
    }

public:
    DynamicLibrary(const std::string& path) : DynamicLibrary(path.c_str()) {}
    DynamicLibrary(const std::filesystem::path& path) : DynamicLibrary(path.c_str()) {}

    ~DynamicLibrary()
    {
        if (handle)
        {
#if defined(_WIN32)
            FreeLibrary(handle);
#else
            dlclose(handle);
#endif
        }
    }

    void* getSymbol(const std::string& name)
    {
#if defined(_WIN32)
        return (void*)GetProcAddress(handle, name.c_str());
#else
        return dlsym(handle, name.c_str());
#endif
    }

    bool isLoaded() const { return handle != nullptr; }

private:
    LibHandle handle;
};

} // namespace Graphite::Core::Plugin
