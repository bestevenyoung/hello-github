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
#include "godnux/module/shared_library_loader.hpp"
#include <iostream>

namespace godnux {

SharedLibraryLoader::SharedLibraryLoader() {
    // Constructor
}

SharedLibraryLoader::~SharedLibraryLoader() {
    unload_library();
}

bool SharedLibraryLoader::load_library(const std::string& path) {
    if (is_loaded()) {
        unload_library();
    }
    
#ifdef _WIN32
    handle = LoadLibraryA(path.c_str());
#else
    handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
#endif
    
    if (!handle) {
#ifdef _WIN32
        std::cerr << "SharedLibraryLoader: Failed to load library '" << path 
                  << "', error: " << GetLastError() << std::endl;
#else
        std::cerr << "SharedLibraryLoader: Failed to load library '" << path 
                  << "', error: " << dlerror() << std::endl;
#endif
        return false;
    }
    
    library_path = path;
    std::cout << "SharedLibraryLoader: Loaded library '" << path << "'" << std::endl;
    return true;
}

bool SharedLibraryLoader::unload_library() {
    if (!handle) return true;
    
    bool success = true;
#ifdef _WIN32
    success = FreeLibrary(handle);
    if (!success) {
        std::cerr << "SharedLibraryLoader: Failed to unload library '" << library_path 
                  << "', error: " << GetLastError() << std::endl;
    }
#else
    success = (dlclose(handle) == 0);
    if (!success) {
        std::cerr << "SharedLibraryLoader: Failed to unload library '" << library_path 
                  << "', error: " << dlerror() << std::endl;
    }
#endif
    
    if (success) {
        std::cout << "SharedLibraryLoader: Unloaded library '" << library_path << "'" << std::endl;
    }
    
    handle = nullptr;
    library_path.clear();
    return success;
}

void* SharedLibraryLoader::get_symbol(const std::string& symbol_name) {
    if (!handle) {
        std::cerr << "SharedLibraryLoader: No library loaded" << std::endl;
        return nullptr;
    }
    
#ifdef _WIN32
    void* symbol = GetProcAddress(handle, symbol_name.c_str());
#else
    void* symbol = dlsym(handle, symbol_name.c_str());
#endif
    
    if (!symbol) {
#ifdef _WIN32
        std::cerr << "SharedLibraryLoader: Symbol '" << symbol_name 
                  << "' not found, error: " << GetLastError() << std::endl;
#else
        std::cerr << "SharedLibraryLoader: Symbol '" << symbol_name 
                  << "' not found, error: " << dlerror() << std::endl;
#endif
    } else {
        std::cout << "SharedLibraryLoader: Resolved symbol '" << symbol_name << "'" << std::endl;
    }
    
    return symbol;
}

bool SharedLibraryLoader::is_loaded() const {
    return handle != nullptr;
}

} // namespace godnux
