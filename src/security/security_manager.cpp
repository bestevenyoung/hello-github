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
#include "godnux/security/security_manager.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <ctime>

namespace godnux {

SecurityManager* SecurityManager::singleton = nullptr;

SecurityManager::SecurityManager() {
    singleton = this;
    current_user = std::to_string(getuid());
    authenticated = true;
    
    granted_capabilities.clear();
    granted_capabilities.insert(CAP_FS);
    granted_capabilities.insert(CAP_SPAWN);
    
    std::cout << "SecurityManager: Initialized for user " << current_user << std::endl;
}

SecurityManager::~SecurityManager() {
    std::cout << "SecurityManager: Shutdown complete" << std::endl;
}

SecurityManager* SecurityManager::get_singleton() {
    return singleton;
}

bool SecurityManager::check_capability(int pid, Capability cap) {
    if (!process_manager) {
        std::cout << "SECURITY: DENIED [PID: " << pid << "] check_capability " << cap << " - no process manager" << std::endl;
        return false;
    }
    
    Process* process = process_manager->get_process(pid);
    if (!process) {
        std::cout << "SECURITY: DENIED [PID: " << pid << "] check_capability " << cap << " - process not found" << std::endl;
        return false;
    }
    
    CapabilityMask process_caps = process->get_capabilities();
    bool allowed = has_capability(process_caps, cap);
    
    std::cout << "SECURITY: " << (allowed ? "ALLOWED" : "DENIED") 
              << " [PID: " << pid << "] check_capability " << cap << std::endl;
    
    log_security_event("check_capability " + std::to_string(cap), pid, allowed);
    
    return allowed;
}

bool SecurityManager::check_privilege(int pid, ProcessPrivilege min_privilege) {
    if (!process_manager) return false;
    
    Process* process = process_manager->get_process(pid);
    if (!process) return false;
    
    ProcessPrivilege process_priv = process->get_privilege();
    bool allowed = (process_priv <= min_privilege); // Lower value means higher privilege
    
    std::cout << "SECURITY: " << (allowed ? "ALLOWED" : "DENIED") 
              << " [PID: " << pid << "] check_privilege " << static_cast<int>(min_privilege) 
              << " (process has " << static_cast<int>(process_priv) << ")" << std::endl;
    
    log_security_event("check_privilege " + std::to_string(static_cast<int>(min_privilege)), pid, allowed);
    
    return allowed;
}

bool SecurityManager::can_schedule(int pid, int requested_priority) {
    if (!process_manager) return false;
    
    Process* process = process_manager->get_process(pid);
    if (!process) return false;
    
    // Kernel can schedule at any priority
    if (process->get_privilege() == PRIV_KERNEL) {
        return true;
    }
    
    // User processes cannot schedule at high priorities
    bool allowed = (requested_priority >= 10); // 0-9 are reserved for system
    
    std::cout << "SECURITY: " << (allowed ? "ALLOWED" : "DENIED") 
              << " [PID: " << pid << "] can_schedule at priority " << requested_priority << std::endl;
    
    log_security_event("can_schedule at priority " + std::to_string(requested_priority), pid, allowed);
    
    return allowed;
}

bool SecurityManager::can_create_process(int parent_pid, ProcessPrivilege requested_privilege, CapabilityMask requested_caps) {
    if (!process_manager) return false;
    
    Process* parent = process_manager->get_process(parent_pid);
    if (!parent) return false;
    
    ProcessPrivilege parent_priv = parent->get_privilege();
    
    // Cannot create processes with higher privilege than parent
    bool allowed = (requested_privilege >= parent_priv);
    
    std::cout << "SECURITY: " << (allowed ? "ALLOWED" : "DENIED") 
              << " [PID: " << parent_pid << "] can_create_process with privilege " 
              << static_cast<int>(requested_privilege) << std::endl;
    
    log_security_event("can_create_process with privilege " + std::to_string(static_cast<int>(requested_privilege)), parent_pid, allowed);
    
    return allowed;
}

void SecurityManager::log_security_event(const std::string& event, int pid, bool allowed) {
    std::ofstream logfile("/tmp/godnux_security.log", std::ios_base::app);
    if (logfile.is_open()) {
        logfile << "[" << time(nullptr) << "] " << current_user << " [PID: " << pid << "] " << event 
                << " [Allowed: " << (allowed ? "YES" : "NO") << "]" << std::endl;
    }
}

// Add the missing methods here
bool SecurityManager::acquire_privileges(const std::string& password) {
    if (password == "root" || geteuid() == 0) {
        granted_capabilities.insert(CAP_KERNEL);  // Use CAP_KERNEL instead of CAP_ALL
        std::cout << "SecurityManager: Privileges escalated to root" << std::endl;
        log_security_event("Privilege escalation to root", 0, true);
        return true;
    }
    
    std::cout << "SecurityManager: Failed privilege escalation" << std::endl;
    log_security_event("Privilege escalation to root", 0, false);
    return false;
}

void SecurityManager::drop_privileges() {
    granted_capabilities.clear();
    granted_capabilities.insert(CAP_FS);
    granted_capabilities.insert(CAP_SPAWN);
    std::cout << "SecurityManager: Dropped to user privileges" << std::endl;
    log_security_event("Dropped privileges", 0, true);
}

bool SecurityManager::has_privileges() const {
    return granted_capabilities.count(CAP_KERNEL) > 0;  // Use CAP_KERNEL instead of CAP_ALL
}

bool SecurityManager::check_capability(Capability cap) {
    if (granted_capabilities.count(CAP_KERNEL) || granted_capabilities.count(cap)) {
        return true;
    }
    
    std::cout << "SecurityManager: Capability denied" << std::endl;
    return false;
}

bool SecurityManager::authenticate(const std::string& username, const std::string& password) {
    current_user = username;
    authenticated = true;
    return true;
}

void SecurityManager::logout() {
    authenticated = false;
    drop_privileges();
}

void SecurityManager::grant_capability(Capability cap) {
    granted_capabilities.insert(cap);
}

void SecurityManager::revoke_capability(Capability cap) {
    granted_capabilities.erase(cap);
}

bool SecurityManager::is_module_trusted(const std::string& module_path) {
    std::string module_name = extract_module_name(module_path);
    return trust_store.is_module_trusted(module_name, TRUST_COMMUNITY);
}

void SecurityManager::trust_module(const std::string& module_path) {
    std::string module_name = extract_module_name(module_path);
    trust_store.add_trusted_module(module_name, TRUST_COMMUNITY);
    log_security_event("Module trusted: " + module_name, 0, true);
}

void SecurityManager::untrust_module(const std::string& module_path) {
    std::string module_name = extract_module_name(module_path);
    trust_store.remove_trusted_module(module_name);
    log_security_event("Module untrusted: " + module_name, 0, true);
}

std::string SecurityManager::extract_module_name(const std::string& path) {
    std::string name = path;
    size_t last_slash = name.find_last_of('/');
    if (last_slash != std::string::npos) {
        name = name.substr(last_slash + 1);
    }
    
    size_t dot_pos = name.find_last_of('.');
    if (dot_pos != std::string::npos) {
        name = name.substr(0, dot_pos);
    }
    
    // Remove 'lib' prefix if present
    if (name.substr(0, 3) == "lib") {
        name = name.substr(3);
    }
    
    return name;
}

} // namespace godnux