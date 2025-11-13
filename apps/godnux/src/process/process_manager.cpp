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
#include "godnux/process/process_manager.hpp"
#include "godnux/security/security_manager.hpp"
#include <iostream>

namespace godnux {

ProcessManager::ProcessManager() {
    std::cout << "ProcessManager: Initialized" << std::endl;
}

ProcessManager::~ProcessManager() {
    processes.clear();
    std::cout << "ProcessManager: Shutdown complete" << std::endl;
}

Process* ProcessManager::create_process(const std::string& name, int priority) {
    // For backward compatibility, create a basic user process
    return create_secure_process(name, PRIV_USER, CAP_BASIC);
}

Process* ProcessManager::create_secure_process(const std::string& name, 
                                              ProcessPrivilege privilege,
                                              CapabilityMask capabilities) {
    auto process = std::make_unique<Process>();
    process->set_pid(next_pid++);
    process->set_name(name);
    process->set_privilege(privilege);
    process->set_capabilities(capabilities);
    
    Process* result = process.get();
    processes[process->get_pid()] = std::move(process);
    
    std::cout << "ProcessManager: Created secure process '" << name 
              << "' with PID " << result->get_pid()
              << " [Privilege: " << result->get_privilege_string()
              << ", Caps: 0x" << std::hex << result->get_capabilities() << std::dec << "]"
              << std::endl;
    return result;
}

bool ProcessManager::destroy_process(int pid) {
    auto it = processes.find(pid);
    if (it == processes.end()) {
        std::cerr << "ProcessManager: No process with PID " << pid << std::endl;
        return false;
    }
    
    processes.erase(it);
    std::cout << "ProcessManager: Destroyed process with PID " << pid << std::endl;
    return true;
}

Process* ProcessManager::get_process(int pid) {
    auto it = processes.find(pid);
    if (it != processes.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<Process*> ProcessManager::get_all_processes() {
    std::vector<Process*> result;
    for (auto& [pid, process] : processes) {
        result.push_back(process.get());
    }
    return result;
}

bool ProcessManager::suspend_process(int pid) {
    auto process = get_process(pid);
    if (!process) return false;
    
    // Security check: does the current process have CAP_KILL?
    auto security_mgr = SecurityManager::get_singleton();
    if (security_mgr && current_process) {
        if (!security_mgr->check_capability(current_process->get_pid(), CAP_KILL)) {
            std::cerr << "ProcessManager: Permission denied to suspend process " << pid << std::endl;
            return false;
        }
    }
    
    return process->suspend();
}

bool ProcessManager::resume_process(int pid) {
    auto process = get_process(pid);
    if (!process) return false;
    
    // Security check
    auto security_mgr = SecurityManager::get_singleton();
    if (security_mgr && current_process) {
        if (!security_mgr->check_capability(current_process->get_pid(), CAP_KILL)) {
            std::cerr << "ProcessManager: Permission denied to resume process " << pid << std::endl;
            return false;
        }
    }
    
    return process->resume();
}

bool ProcessManager::terminate_process(int pid) {
    auto process = get_process(pid);
    if (!process) return false;
    
    // Security check: CAP_KILL required to terminate processes
    auto security_mgr = SecurityManager::get_singleton();
    if (security_mgr && current_process) {
        if (!security_mgr->check_capability(current_process->get_pid(), CAP_KILL)) {
            std::cerr << "ProcessManager: Permission denied to terminate process " << pid << std::endl;
            return false;
        }
    }
    
    return process->terminate();
}

void ProcessManager::set_scheduler_type(SchedulerType type) {
    scheduler_type = type;
}

void ProcessManager::schedule() {
    // Security-aware scheduling: system processes get priority
    if (ready_queue.empty()) return;
    
    Process* next_process = ready_queue.front();
    ready_queue.pop();
    
    // Check if this process can be scheduled with its current priority
    auto security_mgr = SecurityManager::get_singleton();
    if (security_mgr) {
        int effective_priority = (next_process->get_privilege() == PRIV_KERNEL) ? 0 : 10;
        if (security_mgr->can_schedule(next_process->get_pid(), effective_priority)) {
            current_process = next_process;
            std::cout << "Scheduler: Running process PID " << next_process->get_pid() 
                      << " [" << next_process->get_privilege_string() << "]" << std::endl;
        } else {
            std::cerr << "Scheduler: Security check failed for PID " << next_process->get_pid() << std::endl;
            // Put it back in the queue?
            ready_queue.push(next_process);
        }
    } else {
        // Fallback without security
        current_process = next_process;
    }
}

void ProcessManager::yield() {
    if (current_process) {
        ready_queue.push(current_process);
        current_process = nullptr;
    }
    schedule();
}

int ProcessManager::get_running_count() const {
    int count = 0;
    for (const auto& [pid, process] : processes) {
        if (process->get_state() == RUNNING) {
            count++;
        }
    }
    return count;
}

void ProcessManager::_bind_methods() {
    // Will implement later for scripting
}

} // namespace godnux
