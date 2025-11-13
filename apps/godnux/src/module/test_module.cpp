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
#include "godnux/module/test_module.hpp"
#include <iostream>

namespace godnux {

void TestModule::_bind_methods() {
    // Will implement later for scripting
}

TestModule::TestModule() {
    set_name("test_module");
    set_version("1.0.0");
    set_trust_level(TRUST_CORE);
    set_capabilities(CAP_BASIC);
    std::cout << "TestModule: Created" << std::endl;
}

TestModule::~TestModule() {
    std::cout << "TestModule: Destroyed" << std::endl;
}

bool TestModule::initialize() {
    std::cout << "TestModule: Initializing test module" << std::endl;
    // Call parent implementation
    return Module::initialize();
}

void TestModule::shutdown() {
    std::cout << "TestModule: Shutting down test module" << std::endl;
    Module::shutdown();
}

void TestModule::test_function() {
    std::cout << "TestModule: test_function() called" << std::endl;
}

} // namespace godnux
