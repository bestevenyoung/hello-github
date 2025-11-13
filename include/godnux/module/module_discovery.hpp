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
#ifndef GODNUX_MODULE_DISCOVERY_HPP
#define GODNUX_MODULE_DISCOVERY_HPP

#include "module_manifest.hpp"
#include <string>
#include <vector>
#include <filesystem>

namespace godnux {

class ModuleDiscovery {
private:
    std::vector<std::string> search_paths;
    
public:
    ModuleDiscovery();
    
    // Configuration
    void add_search_path(const std::string& path);
    void set_search_paths(const std::vector<std::string>& paths);
    const std::vector<std::string>& get_search_paths() const { return search_paths; }
    
    // Discovery operations
    std::vector<ModuleManifest> discover_modules();
    ModuleManifest discover_module(const std::string& module_name);
    
    // File system operations
    static bool validate_module_path(const std::string& path);
    static std::string find_module_library(const std::string& module_name, const std::vector<std::string>& search_paths);
    
private:
    ModuleManifest load_manifest_from_file(const std::filesystem::path& manifest_path);
    std::vector<std::filesystem::path> find_manifest_files();
};

} // namespace godnux

#endif // GODNUX_MODULE_DISCOVERY_HPP
