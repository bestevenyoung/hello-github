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

#include "memory/memory_manager.hpp"
#include "process/process_manager.hpp"
#include "security/security_manager.hpp"
#include "ipc/ipc_manager.hpp"
#include "module/module_registry.hpp"
#include <memory>
#include <chrono>

namespace godnux {

class Kernel : public Object {
    GDCLASS(Kernel, Object);
    
private:
    static Kernel* singleton;
    bool initialized = false;
    uint64_t startup_time;
    
    std::unique_ptr<MemoryManager> memory_manager;
    std::unique_ptr<ProcessManager> process_manager;
    std::unique_ptr<SecurityManager> security_manager;
    std::unique_ptr<IPCManager> ipc_manager;
    std::unique_ptr<ModuleRegistry> module_registry;
    
protected:
    static void _bind_methods();
    
public:
    Kernel();
    ~Kernel();
    
    static Kernel* get_singleton();
    
    bool initialize();
    void shutdown();
    
    std::string get_version() const;
    uint64_t get_uptime() const;
    void print_system_info() const;
    
    // Public getters for subsystems
    MemoryManager* get_memory_manager() { return memory_manager.get(); }
    ProcessManager* get_process_manager() { return process_manager.get(); }
    SecurityManager* get_security_manager() { return security_manager.get(); }
    IPCManager* get_ipc_manager() { return ipc_manager.get(); }
    ModuleRegistry* get_module_registry() { return module_registry.get(); }
};

} // namespace godnux

#endif // GODNUX_KERNEL_HPP
