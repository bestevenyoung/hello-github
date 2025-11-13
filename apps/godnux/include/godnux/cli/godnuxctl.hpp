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
#ifndef GODNUX_CLI_GODNUXCTL_HPP
#define GODNUX_CLI_GODNUXCTL_HPP

namespace godnux {

class GodnuxCLI {
public:
    GodnuxCLI();
    ~GodnuxCLI();
    
    int run(int argc, char** argv);

private:
    void print_usage();
    int cmd_version();
    int cmd_info();
    int cmd_classes();
    int cmd_memory();
    int cmd_processes();
    int cmd_security();
    int cmd_security_test();
    
    // Add these new module commands:
    int cmd_modules(int argc, char** argv);
    int cmd_module_list(int argc, char** argv);
    int cmd_module_load(int argc, char** argv);
    int cmd_module_unload(int argc, char** argv);
};

} // namespace godnux

#endif // GODNUX_CLI_GODNUXCTL_HPP