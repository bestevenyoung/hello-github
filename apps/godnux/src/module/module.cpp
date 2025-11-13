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
#include "godnux/module/module.hpp"
#include <iostream>

namespace godnux {

void Module::_bind_methods() {
    // Will implement later for scripting
}

Module::Module() 
    : name("unnamed"), version("1.0.0"), trust_level(TRUST_COMMUNITY), capabilities(CAP_NONE) {
    std::cout << "Module: Created '" << name << "'" << std::endl;
}

Module::~Module() {
    std::cout << "Module: Destroyed '" << name << "'" << std::endl;
}

bool Module::initialize() {
    std::cout << "Module: Initialized '" << name 
              << "' [Trust: " << get_trust_level_string() 
              << ", Caps: 0x" << std::hex << capabilities << std::dec << "]" << std::endl;
    return true;
}

void Module::shutdown() {
    std::cout << "Module: Shutdown '" << name << "'" << std::endl;
}

} // namespace godnux
