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
#ifndef GODNUX_MODULE_REGISTRY_HPP
#define GODNUX_MODULE_REGISTRY_HPP

#include "../object.hpp"
#include "module.hpp"
#include <map>
#include <memory>
#include <vector>

namespace godnux {

class ModuleRegistry : public Object {
    GDCLASS(ModuleRegistry, Object);
    
private:
    static ModuleRegistry* singleton;
    std::map<std::string, std::unique_ptr<Module>> modules;
    
protected:
    static void _bind_methods();
    
public:
    ModuleRegistry();
    ~ModuleRegistry();
    
    static ModuleRegistry* get_singleton();
    
    // Module management
    bool register_module(std::unique_ptr<Module> module);
    bool unregister_module(const std::string& name);
    Module* get_module(const std::string& name);
    std::vector<Module*> get_all_modules();
    
    // Lifecycle management
    bool initialize_module(const std::string& name);
    bool shutdown_module(const std::string& name);
    void shutdown_all();
    
    // Statistics
    size_t get_module_count() const { return modules.size(); }
    size_t get_initialized_count() const;
    
    // Trust level queries
    std::vector<Module*> get_modules_by_trust_level(ModuleTrustLevel level);
};

} // namespace godnux

#endif // GODNUX_MODULE_REGISTRY_HPP
