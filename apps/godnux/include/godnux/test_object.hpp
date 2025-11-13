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
#ifndef GODNUX_TEST_OBJECT_HPP
#define GODNUX_TEST_OBJECT_HPP

#include "object.hpp"

namespace godnux {

class TestObject : public Object {
    GODNUX_CLASS(TestObject, Object)
    
public:
    TestObject();
    ~TestObject() override;
    
    void test_method();
    void set_name(const String& name);
    String get_name() const;
    
private:
    String object_name = "Unnamed";
};

} // namespace godnux

#endif // GODNUX_TEST_OBJECT_HPP