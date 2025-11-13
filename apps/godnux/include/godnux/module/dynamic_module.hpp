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
#ifndef GODNUX_DYNAMIC_MODULE_HPP
#define GODNUX_DYNAMIC_MODULE_HPP

#include "module.hpp"
#include "module_manifest.hpp"
#include "shared_library_loader.hpp"
#include <memory>

namespace godnux {

class DynamicModule : public Module {
    GDCLASS(DynamicModule, Module);
    
private:
    ModuleManifest manifest;
    std::unique_ptr<SharedLibraryLoader> library_loader;
    
    // Function pointers for module entry points
    using ModuleInitFunc = Module*(*)();
    using ModuleShutdownFunc = void(*)();
    
    ModuleInitFunc init_func = nullptr;
    ModuleShutdownFunc shutdown_func = nullptr;
    
protected:
    static void _bind_methods();
    
public:
    DynamicModule();
    DynamicModule(const ModuleManifest& manifest);
    ~DynamicModule();
    
    bool load_library();
    bool unload_library();
    
    // Override Module methods
    bool initialize() override;
    void shutdown() override;
    
    // Getters
    const ModuleManifest& get_manifest() const { return manifest; }
    bool is_library_loaded() const;
    
    // Manifest operations
    void set_manifest(const ModuleManifest& manifest);
    bool validate_manifest() const;
    
private:
    bool resolve_entry_points();
};

} // namespace godnux

#endif // GODNUX_DYNAMIC_MODULE_HPP
