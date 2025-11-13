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
#include "godnux/process/process.hpp"
#include <iostream>

namespace godnux {

void Process::_bind_methods() {
    // Empty for now - satisfies GDCLASS requirement
}

// Rest of your existing implementation stays the same
Process::Process() : pid(0), state(CREATED), parent(nullptr) {
    std::cout << "Process: Created" << std::endl;
}

Process::~Process() {
    std::cout << "Process: Destroyed (PID: " << pid << ")" << std::endl;
}

bool Process::start() {
    if (state != CREATED) {
        std::cerr << "Process: Cannot start - already in state " << state << std::endl;
        return false;
    }
    
    state = RUNNING;
    std::cout << "Process: Started (PID: " << pid << ")" << std::endl;
    return true;
}

bool Process::terminate() {
    state = TERMINATED;
    std::cout << "Process: Terminated (PID: " << pid << ")" << std::endl;
    return true;
}

bool Process::suspend() {
    if (state != RUNNING) {
        return false;
    }
    state = SUSPENDED;
    return true;
}

bool Process::resume() {
    if (state != SUSPENDED) {
        return false;
    }
    state = RUNNING;
    return true;
}

} // namespace godnux