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
#include "godnux/test_object.hpp"
#include <iostream>

namespace godnux {

TestObject::TestObject() {
    std::cout << "TestObject created!" << std::endl;
    register_signal("test_signal");
}

TestObject::~TestObject() {
    std::cout << "TestObject destroyed: " << object_name << std::endl;
}

void TestObject::test_method() {
    std::cout << "TestObject::test_method() called on: " << object_name << std::endl;
    // Emit signal with const char* data
    emit_signal("test_signal", Variant{"Hello from test_signal!"});
}

void TestObject::set_name(const String& name) {
    object_name = name;
}

String TestObject::get_name() const {
    return object_name;
}

} // namespace godnux