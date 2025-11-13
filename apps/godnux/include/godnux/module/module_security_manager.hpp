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
#ifndef GODNUX_MODULE_SECURITY_MANAGER_HPP
#define GODNUX_MODULE_SECURITY_MANAGER_HPP

#include "../security/security_manager.hpp"
#include "module_manifest.hpp"
#include <string>

namespace godnux {

class ModuleSecurityManager {
private:
    SecurityManager* security_manager;
    
public:
    ModuleSecurityManager(SecurityManager* sec_mgr);
    
    // Module security checks
    bool can_load_module(const ModuleManifest& manifest, int loading_pid);
    bool can_initialize_module(const ModuleManifest& manifest, int initializing_pid);
    bool can_unload_module(const std::string& module_name, int unloading_pid);
    
    // Resource quota checks
    bool check_resource_quota(const std::string& module_name, size_t requested_memory);
    bool check_capability_escalation(const ModuleManifest& old_manifest, const ModuleManifest& new_manifest);
    
    // Escape visibility logging
    void log_module_escape_attempt(const std::string& module_name, const std::string& escape_vector, bool allowed);
    void log_module_trust_change(const std::string& module_name, ModuleTrustLevel old_level, ModuleTrustLevel new_level);
    
    // Audit integration
    void audit_module_event(const std::string& module_name, const std::string& event, bool success);
};

} // namespace godnux

#endif // GODNUX_MODULE_SECURITY_MANAGER_HPP
