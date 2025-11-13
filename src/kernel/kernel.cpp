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
#include "godnux/module/test_module.hpp"
#include <iostream>
#include <chrono>

namespace godnux {

Kernel* Kernel::singleton = nullptr;

Kernel* Kernel::get_singleton() {
    if (!singleton) {
        singleton = new Kernel();
    }
    return singleton;
}

Kernel::Kernel() {
    startup_time = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    std::cout << "Kernel: Constructor called" << std::endl;
}

Kernel::~Kernel() {
    shutdown();
    std::cout << "Kernel: Destructor called" << std::endl;
}

bool Kernel::initialize() {
    if (initialized) {
        std::cout << "Kernel: Already initialized" << std::endl;
        return true;
    }
    
    std::cout << "🔧 Initializing Godnux Kernel..." << std::endl;
    
    try {
        // Register core classes
        Kernel::_register_class();
        MemoryManager::_register_class();
        ProcessManager::_register_class();
        Process::_register_class();
        SecurityManager::_register_class();
        IPCManager::_register_class();
        ModuleRegistry::_register_class();
        Module::_register_class();
        TestModule::_register_class();
        
        // Initialize subsystems
        memory_manager = std::make_unique<MemoryManager>();
        std::cout << "✅ Memory Manager initialized" << std::endl;
        
        process_manager = std::make_unique<ProcessManager>();
        std::cout << "✅ Process Manager initialized" << std::endl;
        
        security_manager = std::make_unique<SecurityManager>();
        std::cout << "✅ Security Manager initialized" << std::endl;
        
        ipc_manager = std::make_unique<IPCManager>();
        std::cout << "✅ IPC Manager initialized" << std::endl;
        
        module_registry = std::make_unique<ModuleRegistry>();
        std::cout << "✅ Module Registry initialized" << std::endl;
        
        // Connect managers
        security_manager->set_process_manager(process_manager.get());
        std::cout << "✅ Managers connected" << std::endl;
        
        // Register test module
        auto test_module = std::make_unique<TestModule>();
        module_registry->register_module(std::move(test_module));
        std::cout << "✅ Test module registered" << std::endl;
        
        // Initialize test module
        module_registry->initialize_module("test_module");
        std::cout << "✅ Test module initialized" << std::endl;
        
        initialized = true;
        std::cout << "🎉 Godnux Kernel initialized successfully!" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "❌ Kernel initialization failed: " << e.what() << std::endl;
        return false;
    }
}

void Kernel::shutdown() {
    if (!initialized) return;
    
    std::cout << "🔧 Shutting down Godnux Kernel..." << std::endl;
    
    // Shutdown subsystems
    if (module_registry) {
        module_registry->shutdown_all();
        module_registry.reset();
        std::cout << "✅ Module Registry shutdown" << std::endl;
    }
    
    if (process_manager) {
        process_manager.reset();
        std::cout << "✅ Process Manager shutdown" << std::endl;
    }
    
    if (memory_manager) {
        memory_manager.reset();
        std::cout << "✅ Memory Manager shutdown" << std::endl;
    }
    
    if (security_manager) {
        security_manager.reset();
        std::cout << "✅ Security Manager shutdown" << std::endl;
    }
    
    if (ipc_manager) {
        ipc_manager.reset();
        std::cout << "✅ IPC Manager shutdown" << std::endl;
    }
    
    initialized = false;
    std::cout << "✅ Godnux Kernel shutdown complete" << std::endl;
}

std::string Kernel::get_version() const {
    return "0.6.0";  // Update version for Phase 6
}

uint64_t Kernel::get_uptime() const {
    auto current_time = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    return current_time - startup_time;
}

void Kernel::print_system_info() const {
    std::cout << "Godnux Kernel " << get_version() << std::endl;
    std::cout << "Uptime: " << get_uptime() << " seconds" << std::endl;
    std::cout << "Initialized: " << (initialized ? "Yes" : "No") << std::endl;
    std::cout << "Object system: Active" << std::endl;
    
    if (memory_manager) {
        std::cout << "Memory: " << memory_manager->get_used_memory() << "/" 
                  << memory_manager->get_total_memory() << " bytes used" << std::endl;
        std::cout << "Allocations: " << memory_manager->get_allocation_count() << std::endl;
    }
    
    if (process_manager) {
        std::cout << "Processes: " << process_manager->get_process_count() << " total" << std::endl;
        std::cout << "Running: " << process_manager->get_running_count() << std::endl;
        std::cout << "Blocked: " << process_manager->get_blocked_count() << std::endl;
    }
    
    if (security_manager) {
        std::cout << "Security: Active" << std::endl;
    }
    
    if (ipc_manager) {
        std::cout << "IPC: Active" << std::endl;
    }
    
    if (module_registry) {
        std::cout << "Modules: " << module_registry->get_module_count() << " loaded" << std::endl;
    }
}

void Kernel::_bind_methods() {
    // Will implement later for scripting
}

} // namespace godnux
