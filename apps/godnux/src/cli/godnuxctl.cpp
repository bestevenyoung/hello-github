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
#include "godnux/module/shared_library_loader.hpp"
#include "godnux/cli/godnuxctl.hpp"
#include "godnux/kernel.hpp"
#include "godnux/process/process_manager.hpp"
#include "godnux/security/capabilities.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace godnux {

GodnuxCLI::GodnuxCLI() {
    // Constructor
}

GodnuxCLI::~GodnuxCLI() {
    // Destructor
}

int GodnuxCLI::run(int argc, char** argv) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string command = argv[1];

    if (command == "--version" || command == "-v") {
        return cmd_version();
    } else if (command == "info") {
        return cmd_info();
    } else if (command == "classes") {
        return cmd_classes();
    } else if (command == "memory") {
        return cmd_memory();
    } else if (command == "processes" || command == "ps") {
        return cmd_processes();
    } else if (command == "security" || command == "sec") {
        return cmd_security();
    } else if (command == "security-test" || command == "sectest") {
        return cmd_security_test();
    } else if (command == "modules" || command == "mod") {
        return cmd_modules(argc, argv);
    } else if (command == "module-list" || command == "modlist") {
        return cmd_module_list(argc, argv);
    } else if (command == "module-load" || command == "modload") {
        return cmd_module_load(argc, argv);
    } else if (command == "module-unload" || command == "modunload") {
        return cmd_module_unload(argc, argv);
    } else if (command == "help" || command == "--help" || command == "-h") {
        print_usage();
        return 0;
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        print_usage();
        return 1;
    }
}

void GodnuxCLI::print_usage() {
    std::cout << "Godnux Kernel CLI - Version 0.6.0" << std::endl;
    std::cout << "Usage: godnux <command>" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  --version, -v      Show kernel version" << std::endl;
    std::cout << "  info               Show system information" << std::endl;
    std::cout << "  classes            List registered classes" << std::endl;
    std::cout << "  memory             Show memory usage" << std::endl;
    std::cout << "  processes, ps      List processes with security context" << std::endl;
    std::cout << "  security, sec      Show security status" << std::endl;
    std::cout << "  security-test, sectest  Run security tests" << std::endl;
    std::cout << "  modules, mod       Module management commands" << std::endl;
    std::cout << "  module-list        List available modules" << std::endl;
    std::cout << "  module-load        Load a module" << std::endl;
    std::cout << "  module-unload      Unload a module" << std::endl;
    std::cout << "  help, --help, -h   Show this help" << std::endl;
}

int GodnuxCLI::cmd_version() {
    std::cout << "Godnux 0.6.0" << std::endl;
    return 0;
}

int GodnuxCLI::cmd_info() {
    auto kernel = Kernel::get_singleton();
    if (!kernel->initialize()) {
        return 1;
    }
    kernel->print_system_info();
    return 0;
}

int GodnuxCLI::cmd_classes() {
    auto kernel = Kernel::get_singleton();
    if (!kernel->initialize()) {
        return 1;
    }
    
    std::cout << "Godnux Class System" << std::endl;
    std::cout << "Registered classes:" << std::endl;
    std::cout << "  - Kernel" << std::endl;
    std::cout << "  - MemoryManager" << std::endl;
    std::cout << "  - ProcessManager" << std::endl;
    std::cout << "  - Process" << std::endl;
    std::cout << "  - SecurityManager" << std::endl;
    std::cout << "  - IPCManager" << std::endl;
    std::cout << "  - ModuleRegistry" << std::endl;
    std::cout << "  - Module" << std::endl;
    std::cout << "  - TestModule" << std::endl;
    
    return 0;
}

int GodnuxCLI::cmd_memory() {
    auto kernel = Kernel::get_singleton();
    if (!kernel->initialize()) {
        return 1;
    }
    
    auto memory_mgr = kernel->get_memory_manager();
    if (!memory_mgr) {
        std::cerr << "Memory Manager not available" << std::endl;
        return 1;
    }
    
    std::cout << "Memory Usage:" << std::endl;
    std::cout << "  Total: " << memory_mgr->get_total_memory() << " bytes" << std::endl;
    std::cout << "  Used: " << memory_mgr->get_used_memory() << " bytes" << std::endl;
    std::cout << "  Free: " << memory_mgr->get_free_memory() << " bytes" << std::endl;
    std::cout << "  Allocations: " << memory_mgr->get_allocation_count() << std::endl;
    
    return 0;
}

int GodnuxCLI::cmd_processes() {
    auto kernel = Kernel::get_singleton();
    if (!kernel->initialize()) {
        return 1;
    }
    
    auto process_mgr = kernel->get_process_manager();
    if (!process_mgr) {
        std::cerr << "Process Manager not available" << std::endl;
        return 1;
    }
    
    auto processes = process_mgr->get_all_processes();
    
    std::cout << "Process List (Security Context):" << std::endl;
    std::cout << std::left 
              << std::setw(8) << "PID" 
              << std::setw(20) << "Name" 
              << std::setw(12) << "State" 
              << std::setw(10) << "Privilege" 
              << std::setw(16) << "Capabilities" 
              << std::endl;
    std::cout << std::string(66, '-') << std::endl;
    
    for (auto* process : processes) {
        std::string state_str;
        switch (process->get_state()) {
            case CREATED: state_str = "CREATED"; break;
            case RUNNING: state_str = "RUNNING"; break;
            case SUSPENDED: state_str = "SUSPENDED"; break;
            case TERMINATED: state_str = "TERMINATED"; break;
        }
        
        // Format capabilities as hex
        std::stringstream caps_ss;
        caps_ss << "0x" << std::hex << process->get_capabilities();
        
        std::cout << std::left 
                  << std::setw(8) << process->get_pid()
                  << std::setw(20) << process->get_name()
                  << std::setw(12) << state_str
                  << std::setw(10) << process->get_privilege_string()
                  << std::setw(16) << caps_ss.str()
                  << std::endl;
    }
    
    std::cout << "Total: " << processes.size() << " processes" << std::endl;
    
    return 0;
}

int GodnuxCLI::cmd_security() {
    auto kernel = Kernel::get_singleton();
    if (!kernel->initialize()) {
        return 1;
    }
    
    auto security_mgr = kernel->get_security_manager();
    if (!security_mgr) {
        std::cerr << "Security Manager not available" << std::endl;
        return 1;
    }
    
    std::cout << "Security Status:" << std::endl;
    std::cout << "  Security Manager: ACTIVE" << std::endl;
    std::cout << "  Capability System: ENABLED" << std::endl;
    std::cout << "  Privilege Levels: KERNEL, SYSTEM, USER, SANDBOXED" << std::endl;
    std::cout << "  Audit Logging: ACTIVE" << std::endl;
    
    return 0;
}

int GodnuxCLI::cmd_security_test() {
    auto kernel = Kernel::get_singleton();
    if (!kernel->initialize()) {
        return 1;
    }
    
    auto process_mgr = kernel->get_process_manager();
    auto security_mgr = kernel->get_security_manager();
    
    if (!process_mgr || !security_mgr) {
        std::cerr << "Required managers not available" << std::endl;
        return 1;
    }
    
    std::cout << "Running Security Tests..." << std::endl;
    std::cout << std::string(40, '=') << std::endl;
    
    // Test 1: Create different privilege level processes
    std::cout << "Test 1: Process Creation with Different Privileges" << std::endl;
    auto user_process = process_mgr->create_secure_process("test_user", PRIV_USER, CAP_BASIC);
    auto system_process = process_mgr->create_secure_process("test_system", PRIV_SYSTEM, CAP_SYSTEM);
    
    std::cout << "✓ Created USER process with basic capabilities" << std::endl;
    std::cout << "✓ Created SYSTEM process with system capabilities" << std::endl;
    
    // Test 2: Test capability checks
    std::cout << "\nTest 2: Capability Checking" << std::endl;
    bool user_has_spawn = security_mgr->check_capability(user_process->get_pid(), CAP_SPAWN);
    bool system_has_spawn = security_mgr->check_capability(system_process->get_pid(), CAP_SPAWN);
    
    std::cout << "USER process CAP_SPAWN: " << (user_has_spawn ? "ALLOWED" : "DENIED") << std::endl;
    std::cout << "SYSTEM process CAP_SPAWN: " << (system_has_spawn ? "ALLOWED" : "DENIED") << std::endl;
    
    // Test 3: Test privilege escalation prevention
    std::cout << "\nTest 3: Privilege Escalation Prevention" << std::endl;
    bool can_create_kernel = security_mgr->can_create_process(user_process->get_pid(), PRIV_KERNEL, CAP_KERNEL);
    std::cout << "USER creating KERNEL process: " << (can_create_kernel ? "ALLOWED" : "DENIED") << std::endl;
    
    // Test 4: Test scheduling security
    std::cout << "\nTest 4: Scheduling Security" << std::endl;
    bool user_high_priority = security_mgr->can_schedule(user_process->get_pid(), 0); // Highest priority
    bool system_high_priority = security_mgr->can_schedule(system_process->get_pid(), 0);
    
    std::cout << "USER high priority scheduling: " << (user_high_priority ? "ALLOWED" : "DENIED") << std::endl;
    std::cout << "SYSTEM high priority scheduling: " << (system_high_priority ? "ALLOWED" : "DENIED") << std::endl;
    
    // Cleanup test processes
    process_mgr->destroy_process(user_process->get_pid());
    process_mgr->destroy_process(system_process->get_pid());
    
    std::cout << std::string(40, '=') << std::endl;
    std::cout << "Security Tests Completed!" << std::endl;
    
    return 0;
}

// New module management methods
int GodnuxCLI::cmd_modules(int argc, char** argv) {
    std::cout << "Module Management Commands:" << std::endl;
    std::cout << "  module-list, modlist     List all available modules" << std::endl;
    std::cout << "  module-load <name>       Load a module by name" << std::endl;
    std::cout << "  module-unload <name>     Unload a module by name" << std::endl;
    std::cout << "  module-info <name>       Show module information" << std::endl;
    return 0;
}

int GodnuxCLI::cmd_module_list(int argc, char** argv) {
    auto kernel = Kernel::get_singleton();
    if (!kernel->initialize()) {
        return 1;
    }
    
    auto module_registry = kernel->get_module_registry();
    if (!module_registry) {
        std::cerr << "Module Registry not available" << std::endl;
        return 1;
    }
    
    std::cout << "Available Modules:" << std::endl;
    std::cout << "  [CORE] test_module - Test module for verification" << std::endl;
    std::cout << "  [EXPERIMENTAL] untrusted_module - Untrusted test module" << std::endl;
    std::cout << std::endl;
    std::cout << "Development Mode: All modules are currently trusted" << std::endl;
    
    return 0;
}

int GodnuxCLI::cmd_module_load(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: godnux module-load <module_name>" << std::endl;
        return 1;
    }
    
    std::string module_name = argv[2];
    auto kernel = Kernel::get_singleton();
    if (!kernel->initialize()) {
        return 1;
    }
    
    auto module_registry = kernel->get_module_registry();
    if (!module_registry) {
        std::cerr << "Module Registry not available" << std::endl;
        return 1;
    }
    
    // Try to load the module - FIXED PATH
    std::string library_path = "../modules/minimal/libminimal.so";  // Changed this line
    
    // Use SharedLibraryLoader to load it
    godnux::SharedLibraryLoader loader;
    if (loader.load_library(library_path)) {
        std::cout << "✓ Successfully loaded: " << library_path << std::endl;
        
        // Get the module creation function
        void* create_func = loader.get_symbol("create_minimal_module");
        if (create_func) {
            std::cout << "✓ Found module entry point" << std::endl;
            // For now, just call it to prove it works
            void* (*create_module)() = (void* (*)())create_func;
            void* module_instance = create_module();
            std::cout << "✓ Module instance created: " << module_instance << std::endl;
        } else {
            std::cerr << "✗ Could not find module entry point" << std::endl;
        }
    } else {
        std::cerr << "✗ Failed to load: " << library_path << std::endl;
    }
    
    return 0;
}


int GodnuxCLI::cmd_module_unload(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: godnux module-unload <module_name>" << std::endl;
        return 1;
    }
    
    std::string module_name = argv[2];
    std::cout << "Unloading module: " << module_name << std::endl;
    std::cout << "Development Mode: Module unloading not fully implemented yet" << std::endl;
    
    return 0;
}

} // namespace godnux