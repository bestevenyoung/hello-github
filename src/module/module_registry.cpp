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
#include "godnux/module/module_registry.hpp"
#include <iostream>

namespace godnux {

ModuleRegistry* ModuleRegistry::singleton = nullptr;

ModuleRegistry* ModuleRegistry::get_singleton() {
    if (!singleton) {
        singleton = new ModuleRegistry();
    }
    return singleton;
}

ModuleRegistry::ModuleRegistry() {
    std::cout << "ModuleRegistry: Initialized" << std::endl;
}

ModuleRegistry::~ModuleRegistry() {
    shutdown_all();
    std::cout << "ModuleRegistry: Shutdown complete" << std::endl;
}

bool ModuleRegistry::register_module(std::unique_ptr<Module> module) {
    if (!module) {
        std::cerr << "ModuleRegistry: Cannot register null module" << std::endl;
        return false;
    }
    
    const std::string& name = module->get_name();
    if (modules.find(name) != modules.end()) {
        std::cerr << "ModuleRegistry: Module '" << name << "' already registered" << std::endl;
        return false;
    }
    
    modules[name] = std::move(module);
    std::cout << "ModuleRegistry: Registered module '" << name << "'" << std::endl;
    return true;
}

bool ModuleRegistry::unregister_module(const std::string& name) {
    auto it = modules.find(name);
    if (it == modules.end()) {
        std::cerr << "ModuleRegistry: Module '" << name << "' not found" << std::endl;
        return false;
    }
    
    // Shutdown the module before removing
    shutdown_module(name);
    modules.erase(it);
    std::cout << "ModuleRegistry: Unregistered module '" << name << "'" << std::endl;
    return true;
}

Module* ModuleRegistry::get_module(const std::string& name) {
    auto it = modules.find(name);
    if (it != modules.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<Module*> ModuleRegistry::get_all_modules() {
    std::vector<Module*> result;
    for (auto& [name, module] : modules) {
        result.push_back(module.get());
    }
    return result;
}

bool ModuleRegistry::initialize_module(const std::string& name) {
    Module* module = get_module(name);
    if (!module) {
        std::cerr << "ModuleRegistry: Cannot initialize unknown module '" << name << "'" << std::endl;
        return false;
    }
    
    return module->initialize();
}

bool ModuleRegistry::shutdown_module(const std::string& name) {
    Module* module = get_module(name);
    if (!module) {
        std::cerr << "ModuleRegistry: Cannot shutdown unknown module '" << name << "'" << std::endl;
        return false;
    }
    
    module->shutdown();
    return true;
}

void ModuleRegistry::shutdown_all() {
    for (auto& [name, module] : modules) {
        module->shutdown();
    }
}

size_t ModuleRegistry::get_initialized_count() const {
    // For now, we'll assume all registered modules are initialized
    // In a real implementation, we'd track initialization state
    return modules.size();
}

std::vector<Module*> ModuleRegistry::get_modules_by_trust_level(ModuleTrustLevel level) {
    std::vector<Module*> result;
    for (auto& [name, module] : modules) {
        if (module->get_trust_level() == level) {
            result.push_back(module.get());
        }
    }
    return result;
}

void ModuleRegistry::_bind_methods() {
    // Will implement later for scripting
}

} // namespace godnux
