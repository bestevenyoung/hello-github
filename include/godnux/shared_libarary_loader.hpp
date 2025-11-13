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

#include "object.hpp"
#include <dlfcn.h>
#include <string>
#include <memory>

namespace godnux {

class SharedLibraryLoader : public Object {
public:
    SharedLibraryLoader();
    ~SharedLibraryLoader();
    
    void* load_library(const std::string& path);
    void* get_symbol(void* handle, const std::string& symbol);
    bool unload_library(void* handle);
    
private:
    // Simple implementation for now
};

} // namespace godnux

#endif // GODNUX_SHARED_LIBRARY_LOADER_HPP