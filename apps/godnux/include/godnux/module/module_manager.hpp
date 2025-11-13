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
#ifndef GODNUX_MODULE_MANAGER_HPP
#define GODNUX_MODULE_MANAGER_HPP

#include "module_discovery.hpp"
#include "shared_library_loader.hpp"
#include <memory>
#include <vector>

namespace godnux {

class ModuleManager {
public:
    ModuleManager();
    ~ModuleManager();
    
    // Load all discovered modules
    void load_all_modules();
    
    // Load a specific module by path
    bool load_module(const std::string& path);
    
    // Unload a specific module
    bool unload_module(const std::string& name);
    
    // Unload all modules
    void unload_all_modules();
    
    // Get a loaded module by name
    std::shared_ptr<Module> get_module(const std::string& name) const;
    
    // Get all loaded modules
    std::vector<std::shared_ptr<Module>> get_loaded_modules() const;
    
    // Get module information for all loaded modules
    std::vector<ModuleInfo> get_loaded_module_info() const;
    
    // Auto-load modules on startup
    void auto_load_modules();

private:
    ModuleDiscovery m_discovery;
    SharedLibraryLoader m_loader;
    std::unordered_map<std::string, std::shared_ptr<Module>> m_modules;
    
    bool initialize_module(std::shared_ptr<Module> module);
};

} // namespace godnux

#endif // GODNUX_MODULE_MANAGER_HPP