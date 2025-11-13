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
#ifndef GODNUX_KERNEL_HPP
#define GODNUX_KERNEL_HPP

#include <string>
#include <vector>
#include <memory>

namespace godnux {

class Kernel {
public:
    Kernel();
    ~Kernel();
    
    // Initialize the kernel
    bool initialize(int argc, char** argv);
    
    // Main loop
    int run();
    
    // Shutdown
    void shutdown();

private:
    bool parse_arguments(int argc, char** argv);
    void print_help();
    void print_version();
    
    bool m_initialized = false;
    bool m_running = false;
};

} // namespace godnux

#endif // GODNUX_KERNEL_HPP