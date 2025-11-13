/*
 * Godnux Kernel
 * Copyright (C) 2025 Barry
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef GODNUX_SHARED_LIBRARY_LOADER_HPP
#define GODNUX_SHARED_LIBRARY_LOADER_HPP

#include <string>
#include <functional>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace godnux {

class SharedLibraryLoader {
private:
#ifdef _WIN32
    HMODULE handle = nullptr;
#else
    void* handle = nullptr;
#endif
    std::string library_path;

public:
    SharedLibraryLoader();
    ~SharedLibraryLoader();
    
    // Load/unload library
    bool load_library(const std::string& path);
    bool unload_library();
    
    // Symbol resolution
    void* get_symbol(const std::string& symbol_name);
    
    // Utility functions
    bool is_loaded() const;
    const std::string& get_library_path() const { return library_path; }
    
    // Template for type-safe symbol resolution
    template<typename T>
    T get_function(const std::string& symbol_name) {
        void* symbol = get_symbol(symbol_name);
        if (!symbol) return nullptr;
        return reinterpret_cast<T>(symbol);
    }
};

} // namespace godnux

#endif // GODNUX_SHARED_LIBRARY_LOADER_HPP
