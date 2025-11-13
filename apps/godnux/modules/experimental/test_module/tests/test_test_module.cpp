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
#include "core/error/error_macros.h"

void test_module_creation() {
    print_line("🧪 Running TestModule tests...");
    
    // Test module creation
    Ref<TestModule> module = memnew(TestModule);
    ERR_FAIL_COND(module.is_null());
    
    // Test method calls
    module->say_hello();
    
    String name = module->get_module_name();
    ERR_FAIL_COND(name != "TestModule");
    
    print_line("✅ All TestModule tests passed!");
}