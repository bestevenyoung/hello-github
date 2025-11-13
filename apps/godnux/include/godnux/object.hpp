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
#ifndef GODNUX_OBJECT_HPP
#define GODNUX_OBJECT_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

namespace godnux {

// Forward declarations
class Object;
class ClassDB;

// Basic types
using String = std::string;
using RefCounted = std::shared_ptr<Object>;

class Object {
public:
    Object();
    virtual ~Object();

    // Reference counting
    void reference();
    void unreference();
    int get_reference_count() const;

    // Class information
    virtual String get_class() const = 0;
    bool is_class(const String& class_name) const;

private:
    int reference_count = 0;
};

// Class Database
class ClassDB {
public:
    static void register_class(const String& class_name, std::function<RefCounted()> constructor);
    static RefCounted create_instance(const String& class_name);
    static bool class_exists(const String& class_name);
    
    template<typename T>
    static void register_class(const String& class_name) {
        register_class(class_name, []() -> RefCounted {
            return std::make_shared<T>();
        });
    }

private:
    static std::unordered_map<String, std::function<RefCounted()>>& get_registry();
};

// Macro for class registration - FIXED to use fully qualified names
#define GDCLASS(className, baseClass) \
public: \
    ::godnux::String get_class() const override { return #className; } \
    static void _register_class() { ::godnux::ClassDB::register_class<className>(#className); }

} // namespace godnux

#endif // GODNUX_OBJECT_HPP