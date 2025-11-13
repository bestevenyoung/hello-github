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
#include "godnux/test_module.hpp"
#include <iostream>

namespace godnux {

// Base Module implementation
Module::Module() {
    std::cout << "Module base constructor" << std::endl;
}

Module::~Module() {
    std::cout << "Module base destructor" << std::endl;
}

// TestModule implementation
TestModule::TestModule() {
    std::cout << "TestModule created!" << std::endl;
}

TestModule::~TestModule() {
    if (m_loaded) {
        unload();
    }
    std::cout << "TestModule destroyed" << std::endl;
}

bool TestModule::load() {
    if (m_loaded) {
        std::cout << "TestModule already loaded" << std::endl;
        return true;
    }
    
    std::cout << "Loading TestModule..." << std::endl;
    m_loaded = true;
    
    std::cout << "TestModule loaded successfully!" << std::endl;
    return true;
}

void TestModule::unload() {
    if (!m_loaded) {
        return;
    }
    
    std::cout << "Unloading TestModule..." << std::endl;
    m_loaded = false;
    
    std::cout << "TestModule unloaded" << std::endl;
}

void TestModule::test_function() {
    std::cout << "TestModule::test_function() called!" << std::endl;
    std::cout << "Module name: " << get_name() << std::endl;
    std::cout << "Module version: " << get_version() << std::endl;
    std::cout << "Module description: " << get_description() << std::endl;
    std::cout << "Provides editor: " << (provides_editor() ? "Yes" : "No") << std::endl;
}

} // namespace godnux