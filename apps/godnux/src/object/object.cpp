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
#include "godnux/object.hpp"
#include <iostream>
#include <unordered_map>

namespace godnux {

// ClassDB implementation
std::unordered_map<String, std::function<RefCounted()>>& ClassDB::get_registry() {
    static std::unordered_map<String, std::function<RefCounted()>> registry;
    return registry;
}

void ClassDB::register_class(const String& class_name, std::function<RefCounted()> constructor) {
    auto& registry = get_registry();
    registry[class_name] = constructor;
    std::cout << "ClassDB: Registered class '" << class_name << "'" << std::endl;
}

RefCounted ClassDB::create_instance(const String& class_name) {
    auto& registry = get_registry();
    auto it = registry.find(class_name);
    if (it != registry.end()) {
        return it->second();
    }
    std::cerr << "ClassDB: Unknown class '" << class_name << "'" << std::endl;
    return nullptr;
}

bool ClassDB::class_exists(const String& class_name) {
    auto& registry = get_registry();
    return registry.find(class_name) != registry.end();
}

// Object implementation
Object::Object() : reference_count(1) {
    std::cout << "Object: Created" << std::endl;
}

Object::~Object() {
    std::cout << "Object: Destroyed" << std::endl;
}

void Object::reference() {
    reference_count++;
}

void Object::unreference() {
    reference_count--;
    if (reference_count <= 0) {
        delete this;
    }
}

int Object::get_reference_count() const {
    return reference_count;
}

bool Object::is_class(const String& class_name) const {
    return get_class() == class_name;
}

} // namespace godnux