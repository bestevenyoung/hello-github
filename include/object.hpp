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
#include <vector>

namespace godnux {

// Forward declarations
class Object;
class ClassDB;

// Smart pointer for Object
using RefCounted = std::shared_ptr<Object>;

// Basic types we'll use
using String = std::string;
using Variant = std::any; // We'll replace with proper Variant later

// Signal connection - like Godot's signals
using Signal = std::string;
using Slot = std::function<void(const Variant&)>;

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
    
    // Signal system (simplified version of Godot's signals)
    void connect(const Signal& signal, const Slot& slot);
    void disconnect(const Signal& signal, const Slot& slot);
    void emit_signal(const Signal& signal, const Variant& data = Variant{});

    // Memory management
    static void cleanup();

protected:
    // For derived classes to register signals
    void register_signal(const Signal& signal);

private:
    int reference_count = 0;
    std::unordered_map<Signal, std::vector<Slot>> signal_slots;
};

// Class Database - like Godot's ClassDB
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

// Macro to simplify class registration (like Godot's GDCLASS)
#define GODNUX_CLASS(type, base) \
public: \
    String get_class() const override { return #type; } \
    static void _register_class() { ClassDB::register_class<type>(#type); }

} // namespace godnux

#endif // GODNUX_OBJECT_HPP