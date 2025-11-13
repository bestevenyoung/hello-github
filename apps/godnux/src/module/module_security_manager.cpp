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
#include "godnux/module/module_security_manager.hpp"
#include <iostream>

namespace godnux {

ModuleSecurityManager::ModuleSecurityManager(SecurityManager* sec_mgr) 
    : security_manager(sec_mgr) {
    std::cout << "ModuleSecurityManager: Initialized" << std::endl;
}

bool ModuleSecurityManager::can_load_module(const ModuleManifest& manifest, int loading_pid) {
    if (!security_manager) {
        std::cerr << "ModuleSecurityManager: No security manager available" << std::endl;
        return false;
    }
    
    // Check if loader has CAP_MODULE capability
    if (!security_manager->check_capability(loading_pid, CAP_MODULE)) {
        log_module_escape_attempt(manifest.name, "LOAD_NO_CAPABILITY", false);
        return false;
    }
    
    // Check trust level restrictions
    if (manifest.trust_level == TRUST_EXPERIMENTAL) {
        // Experimental modules require additional checks
        audit_module_event(manifest.name, "EXPERIMENTAL_LOAD_ATTEMPT", true);
        std::cout << "ModuleSecurityManager: Experimental module '" << manifest.name 
                  << "' - user approval required" << std::endl;
    }
    
    // Check capability validity
    if (!manifest.has_valid_capabilities()) {
        log_module_escape_attempt(manifest.name, "INVALID_CAPABILITIES", false);
        return false;
    }
    
    audit_module_event(manifest.name, "LOAD_ALLOWED", true);
    return true;
}

bool ModuleSecurityManager::can_initialize_module(const ModuleManifest& manifest, int initializing_pid) {
    if (!security_manager) return false;
    
    // For now, use same checks as loading
    // In a real implementation, we might have different checks for initialization
    return can_load_module(manifest, initializing_pid);
}

bool ModuleSecurityManager::can_unload_module(const std::string& module_name, int unloading_pid) {
    if (!security_manager) return false;
    
    // Check if unloader has CAP_MODULE capability
    if (!security_manager->check_capability(unloading_pid, CAP_MODULE)) {
        log_module_escape_attempt(module_name, "UNLOAD_NO_CAPABILITY", false);
        return false;
    }
    
    audit_module_event(module_name, "UNLOAD_ALLOWED", true);
    return true;
}

bool ModuleSecurityManager::check_resource_quota(const std::string& module_name, size_t requested_memory) {
    // Simple resource quota system
    const size_t MAX_MEMORY_QUOTA = 100 * 1024 * 1024; // 100MB
    
    if (requested_memory > MAX_MEMORY_QUOTA) {
        log_module_escape_attempt(module_name, "RESOURCE_QUOTA_EXCEEDED", false);
        return false;
    }
    
    return true;
}

bool ModuleSecurityManager::check_capability_escalation(const ModuleManifest& old_manifest, const ModuleManifest& new_manifest) {
    // Prevent modules from escalating capabilities in updates
    if ((new_manifest.requested_capabilities & ~old_manifest.requested_capabilities) != 0) {
        log_module_escape_attempt(old_manifest.name, "CAPABILITY_ESCALATION", false);
        return false;
    }
    
    return true;
}

void ModuleSecurityManager::log_module_escape_attempt(const std::string& module_name, const std::string& escape_vector, bool allowed) {
    std::cout << "MODULE_SECURITY: " << (allowed ? "ALLOWED" : "DENIED") 
              << " escape attempt by '" << module_name 
              << "' - vector: " << escape_vector << std::endl;
}

void ModuleSecurityManager::log_module_trust_change(const std::string& module_name, ModuleTrustLevel old_level, ModuleTrustLevel new_level) {
    std::cout << "MODULE_TRUST: '" << module_name 
              << "' trust level changed from " << static_cast<int>(old_level)
              << " to " << static_cast<int>(new_level) << std::endl;
}

void ModuleSecurityManager::audit_module_event(const std::string& module_name, const std::string& event, bool success) {
    if (security_manager) {
        security_manager->log_security_event("module_" + event + " " + module_name, 0, success);
    }
}

} // namespace godnux
