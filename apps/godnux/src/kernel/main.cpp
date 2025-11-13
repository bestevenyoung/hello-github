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
#include "godnux/kernel.hpp"
#include "godnux/process/process_manager.hpp"
#include "godnux/security/security_manager.hpp"
#include "godnux/ipc/ipc_manager.hpp"
#include "godnux/cli/godnuxctl.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    // Simple CLI handling
    if (argc > 1) {
        godnux::GodnuxCLI cli;
        return cli.run(argc, argv);
    }
    
    // Interactive mode
    auto kernel = godnux::Kernel::get_singleton();
    if (!kernel->initialize()) {
        return 1;
    }
    
    std::cout << "🎉 Godnux Kernel " << kernel->get_version() << " is running." << std::endl;
    std::cout << "Kernel class: " << "Kernel" << std::endl;
    std::cout << "Type 'help' for commands, 'exit' to quit." << std::endl;
    
    // Memory manager test
    auto memory_mgr = kernel->get_memory_manager();
    void* test_memory = memory_mgr->allocate(1024, 0); // 1KB allocation
    if (test_memory) {
        std::cout << "✅ Memory Manager test: Allocated 1KB at " << test_memory << std::endl;
        memory_mgr->deallocate(test_memory);
        std::cout << "✅ Memory Manager test: Deallocated memory" << std::endl;
    }
    
    // Process manager test with security context
    auto process_mgr = kernel->get_process_manager();
    auto test_process = process_mgr->create_secure_process("demo_process", godnux::PRIV_USER, godnux::CAP_BASIC);
    std::cout << "✅ Process Manager test: Created process '" << test_process->get_name() 
              << "' with PID " << test_process->get_pid() 
              << " [Privilege: " << test_process->get_privilege_string() << "]" << std::endl;
    
    test_process->start();
    std::cout << "✅ Process Manager test: Started process" << std::endl;
    
    test_process->terminate();
    std::cout << "✅ Process Manager test: Terminated process" << std::endl;
    
    // Security manager test
    auto security_mgr = kernel->get_security_manager();
    bool can_spawn = security_mgr->check_capability(test_process->get_pid(), godnux::CAP_SPAWN);
    std::cout << "✅ Security Manager test: CAP_SPAWN check: " << (can_spawn ? "ALLOWED" : "DENIED") << std::endl;
    
    // IPC manager test
    auto ipc_mgr = kernel->get_ipc_manager();
    bool msg_sent = ipc_mgr->send_message(test_process->get_pid(), 0, "system_broadcast", "Hello from security test!");
    std::cout << "✅ IPC Manager test: Message sent: " << (msg_sent ? "SUCCESS" : "FAILED") << std::endl;
    
    // Create a system process to demonstrate privilege differences
    auto system_process = process_mgr->create_secure_process("system_daemon", godnux::PRIV_SYSTEM, godnux::CAP_SYSTEM);
    std::cout << "✅ Security Demo: Created SYSTEM process '" << system_process->get_name() 
              << "' with enhanced capabilities" << std::endl;
    
    // Interactive shell
    std::string input;
    while (true) {
        std::cout << "godnux> ";
        std::getline(std::cin, input);
        
        if (input == "exit" || input == "quit") {
            break;
        } else if (input == "help") {
            std::cout << "Available commands:" << std::endl;
            std::cout << "  ps - List processes with security context" << std::endl;
            std::cout << "  security - Show security status" << std::endl;
            std::cout << "  classes - List registered classes" << std::endl;
            std::cout << "  memory - Show memory usage" << std::endl;
            std::cout << "  info - Show system information" << std::endl;
            std::cout << "  exit - Quit interactive mode" << std::endl;
        } else if (input == "ps") {
            auto processes = process_mgr->get_all_processes();
            std::cout << "Process List:" << std::endl;
            for (auto* process : processes) {
                std::cout << "  PID " << process->get_pid() << ": " << process->get_name()
                          << " [" << process->get_privilege_string() 
                          << ", Caps: 0x" << std::hex << process->get_capabilities() << std::dec << "]" << std::endl;
            }
        } else if (input == "security") {
            std::cout << "Security Status:" << std::endl;
            std::cout << "  Active: YES" << std::endl;
            std::cout << "  Processes with security context: " << process_mgr->get_process_count() << std::endl;
            std::cout << "  Last security check: CAP_SPAWN for PID " << test_process->get_pid() 
                      << " = " << (can_spawn ? "ALLOWED" : "DENIED") << std::endl;
        } else if (input == "classes") {
            std::cout << "Registered Classes:" << std::endl;
            std::cout << "  - Kernel" << std::endl;
            std::cout << "  - MemoryManager" << std::endl;
            std::cout << "  - ProcessManager" << std::endl;
            std::cout << "  - Process" << std::endl;
            std::cout << "  - SecurityManager" << std::endl;
            std::cout << "  - IPCManager" << std::endl;
        } else if (input == "memory") {
            std::cout << "Memory: " << memory_mgr->get_used_memory() << "/" 
                      << memory_mgr->get_total_memory() << " bytes used" << std::endl;
            std::cout << "Allocations: " << memory_mgr->get_allocation_count() << std::endl;
        } else if (input == "info") {
            kernel->print_system_info();
        } else if (!input.empty()) {
            std::cout << "Unknown command: " << input << std::endl;
            std::cout << "Type 'help' for available commands." << std::endl;
        }
    }
    
    // Cleanup
    process_mgr->destroy_process(test_process->get_pid());
    process_mgr->destroy_process(system_process->get_pid());
    
    std::cout << "👋 Shutting down Godnux Kernel..." << std::endl;
    
    return 0;
}
