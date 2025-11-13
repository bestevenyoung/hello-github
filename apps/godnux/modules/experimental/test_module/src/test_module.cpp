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
#include "test_module.h"
#include <iostream>

namespace godnux {

TestModule::TestModule() {
    std::cout << "TestModule constructor called" << std::endl;
}

TestModule::~TestModule() {
    std::cout << "TestModule destructor called" << std::endl;
}

std::string TestModule::get_name() const {
    return "TestModule";
}

std::string TestModule::get_version() const {
    return "1.0.0";
}

std::string TestModule::get_description() const {
    return "A test module for Godnux dynamic loading verification";
}

bool TestModule::load() {
    std::cout << "🔧 TestModule::load() called" << std::endl;
    
    if (is_loaded) {
        std::cout << "⚠️  TestModule already loaded" << std::endl;
        return true;
    }
    
    // Perform module initialization here
    std::cout << "✅ TestModule loaded successfully!" << std::endl;
    is_loaded = true;
    
    // Test our functionality
    say_hello();
    int result = add_numbers(5, 3);
    std::cout << "🧪 Test calculation: 5 + 3 = " << result << std::endl;
    
    return true;
}

void TestModule::unload() {
    std::cout << "🔧 TestModule::unload() called" << std::endl;
    
    if (!is_loaded) {
        std::cout << "⚠️  TestModule not loaded" << std::endl;
        return;
    }
    
    // Perform cleanup here
    std::cout << "✅ TestModule unloaded successfully!" << std::endl;
    is_loaded = false;
}

void TestModule::say_hello() {
    std::cout << "🎉 Hello from TestModule! Dynamic loading works!" << std::endl;
}

int TestModule::add_numbers(int a, int b) {
    return a + b;
}

} // namespace godnux

// Module entry points - these are what the module loader looks for
extern "C" {
    
    // Create module instance
    godnux::Module* create_module() {
        std::cout << "🔄 create_module() called for TestModule" << std::endl;
        return new godnux::TestModule();
    }
    
    // Destroy module instance
    void destroy_module(godnux::Module* module) {
        std::cout << "🔄 destroy_module() called for TestModule" << std::endl;
        delete module;
    }
    
    // Optional: Get module info without creating instance
    const char* get_module_name() {
        return "TestModule";
    }
    
    const char* get_module_version() {
        return "1.0.0";
    }
}