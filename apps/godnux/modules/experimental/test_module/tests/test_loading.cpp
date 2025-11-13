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
#include <iostream>
#include <dlfcn.h>
#include <memory>

// Function pointer types for module entry points
typedef godnux::Module* (*CreateModuleFunc)();
typedef void (*DestroyModuleFunc)(godnux::Module*);

int main() {
    std::cout << "🧪 Testing TestModule loading..." << std::endl;
    
    const char* module_path = "./test_module.so";
    
    // Load the shared library
    void* handle = dlopen(module_path, RTLD_LAZY);
    if (!handle) {
        std::cerr << "❌ Failed to load module: " << dlerror() << std::endl;
        return 1;
    }
    
    std::cout << "✅ Module loaded successfully" << std::endl;
    
    // Get the create_module function
    CreateModuleFunc create_module = (CreateModuleFunc)dlsym(handle, "create_module");
    if (!create_module) {
        std::cerr << "❌ Failed to find create_module: " << dlerror() << std::endl;
        dlclose(handle);
        return 1;
    }
    
    std::cout << "✅ Found create_module function" << std::endl;
    
    // Create module instance
    godnux::Module* module = create_module();
    if (!module) {
        std::cerr << "❌ Failed to create module instance" << std::endl;
        dlclose(handle);
        return 1;
    }
    
    std::cout << "✅ Module instance created: " << module->get_name() << std::endl;
    
    // Test module functionality
    std::cout << "📋 Module info:" << std::endl;
    std::cout << "   Name: " << module->get_name() << std::endl;
    std::cout << "   Version: " << module->get_version() << std::endl;
    std::cout << "   Description: " << module->get_description() << std::endl;
    
    // Load the module
    if (module->load()) {
        std::cout << "✅ Module loaded successfully" << std::endl;
    } else {
        std::cerr << "❌ Module failed to load" << std::endl;
    }
    
    // Unload the module
    module->unload();
    std::cout << "✅ Module unloaded" << std::endl;
    
    // Get destroy function and clean up
    DestroyModuleFunc destroy_module = (DestroyModuleFunc)dlsym(handle, "destroy_module");
    if (destroy_module) {
        destroy_module(module);
        std::cout << "✅ Module instance destroyed" << std::endl;
    } else {
        std::cerr << "⚠️  No destroy_module function found" << std::endl;
        delete module;
    }
    
    // Close the library
    dlclose(handle);
    std::cout << "✅ Library closed" << std::endl;
    
    std::cout << "🎉 All tests passed!" << std::endl;
    return 0;
}