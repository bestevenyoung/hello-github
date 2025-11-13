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
#include "godnux/module/dynamic_module.hpp"
#include <iostream>

namespace godnux {

void DynamicModule::_bind_methods() {
    // Will implement later for scripting
}

DynamicModule::DynamicModule() 
    : library_loader(std::make_unique<SharedLibraryLoader>()) {
    std::cout << "DynamicModule: Created (empty)" << std::endl;
}

DynamicModule::DynamicModule(const ModuleManifest& manifest) 
    : manifest(manifest), library_loader(std::make_unique<SharedLibraryLoader>()) {
    
    // Apply manifest settings to base module
    set_name(manifest.name);
    set_version(manifest.version);
    set_trust_level(manifest.trust_level);
    set_capabilities(manifest.requested_capabilities);
    
    std::cout << "DynamicModule: Created '" << manifest.name << "'" << std::endl;
}

DynamicModule::~DynamicModule() {
    unload_library();
    std::cout << "DynamicModule: Destroyed '" << get_name() << "'" << std::endl;
}

bool DynamicModule::load_library() {
    if (!manifest.is_valid()) {
        std::cerr << "DynamicModule: Cannot load library - invalid manifest" << std::endl;
        return false;
    }
    
    if (is_library_loaded()) {
        std::cout << "DynamicModule: Library already loaded for '" << get_name() << "'" << std::endl;
        return true;
    }
    
    if (!library_loader->load_library(manifest.library_path)) {
        std::cerr << "DynamicModule: Failed to load library for '" << get_name() << "'" << std::endl;
        return false;
    }
    
    if (!resolve_entry_points()) {
        std::cerr << "DynamicModule: Failed to resolve entry points for '" << get_name() << "'" << std::endl;
        unload_library();
        return false;
    }
    
    std::cout << "DynamicModule: Successfully loaded library for '" << get_name() << "'" << std::endl;
    return true;
}

bool DynamicModule::unload_library() {
    if (!is_library_loaded()) {
        return true;
    }
    
    // Call shutdown if module was initialized
    if (init_func) {
        shutdown();
    }
    
    bool success = library_loader->unload_library();
    if (success) {
        init_func = nullptr;
        shutdown_func = nullptr;
    }
    
    return success;
}

bool DynamicModule::initialize() {
    if (!is_library_loaded() && !load_library()) {
        std::cerr << "DynamicModule: Cannot initialize - library not loaded" << std::endl;
        return false;
    }
    
    if (!init_func) {
        std::cerr << "DynamicModule: No init function for '" << get_name() << "'" << std::endl;
        return false;
    }
    
    std::cout << "DynamicModule: Initializing '" << get_name() << "'" << std::endl;
    
    // Call the module's initialization function
    Module* module_instance = init_func();
    if (!module_instance) {
        std::cerr << "DynamicModule: Init function returned null for '" << get_name() << "'" << std::endl;
        return false;
    }
    
    // Call base class initialize for logging
    return Module::initialize();
}

void DynamicModule::shutdown() {
    if (!is_library_loaded()) {
        std::cout << "DynamicModule: Library not loaded for '" << get_name() << "'" << std::endl;
        return;
    }
    
    if (shutdown_func) {
        std::cout << "DynamicModule: Shutting down '" << get_name() << "'" << std::endl;
        shutdown_func();
    } else {
        std::cout << "DynamicModule: No shutdown function for '" << get_name() << "'" << std::endl;
    }
    
    // Call base class shutdown for logging
    Module::shutdown();
}

bool DynamicModule::is_library_loaded() const {
    return library_loader && library_loader->is_loaded();
}

void DynamicModule::set_manifest(const ModuleManifest& new_manifest) {
    manifest = new_manifest;
    
    // Update base module properties
    set_name(manifest.name);
    set_version(manifest.version);
    set_trust_level(manifest.trust_level);
    set_capabilities(manifest.requested_capabilities);
}

bool DynamicModule::validate_manifest() const {
    if (!manifest.is_valid()) {
        std::cerr << "DynamicModule: Manifest invalid for '" << get_name() << "'" << std::endl;
        return false;
    }
    
    if (!manifest.has_valid_capabilities()) {
        std::cerr << "DynamicModule: Invalid capabilities in manifest for '" << get_name() << "'" << std::endl;
        return false;
    }
    
    return true;
}

bool DynamicModule::resolve_entry_points() {
    if (!is_library_loaded()) {
        std::cerr << "DynamicModule: Cannot resolve entry points - library not loaded" << std::endl;
        return false;
    }
    
    // Resolve initialization function using the fixed template
    init_func = library_loader->get_function<Module*(*)()>(manifest.entry_point + "_init");
    if (!init_func) {
        std::cerr << "DynamicModule: Failed to resolve init function '" 
                  << manifest.entry_point + "_init' for '" << get_name() << "'" << std::endl;
        return false;
    }
    
    // Resolve shutdown function (optional)
    shutdown_func = library_loader->get_function<void(*)()>(manifest.entry_point + "_shutdown");
    if (!shutdown_func) {
        std::cout << "DynamicModule: No shutdown function found for '" << get_name() 
                  << "' (this is optional)" << std::endl;
    }
    
    return true;
}

} // namespace godnux
