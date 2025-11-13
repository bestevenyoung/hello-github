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
#ifndef GODNUX_TEST_MODULE_HPP
#define GODNUX_TEST_MODULE_HPP

#include "module.hpp"

namespace godnux {

class TestModule : public Module {
    GDCLASS(TestModule, Module);
    
public:
    TestModule();
    ~TestModule();
    
    void _bind_methods();
    
    bool initialize() override;
    void shutdown() override;
    
    // Test method
    void test_function();
};

} // namespace godnux

#endif // GODNUX_TEST_MODULE_HPP
