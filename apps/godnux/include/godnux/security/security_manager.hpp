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
#ifndef GODNUX_SECURITY_MANAGER_HPP
#define GODNUX_SECURITY_MANAGER_HPP

#include "../object.hpp"
#include "../process/process_manager.hpp"
#include "capabilities.hpp"
#include "trust_store.hpp"
#include <string>
#include <unordered_set>

namespace godnux {

class SecurityManager : public Object {
    GDCLASS(SecurityManager, Object);
    
private:
    static SecurityManager* singleton;
    ProcessManager* process_manager = nullptr;
    TrustStore trust_store;
    
    std::string current_user;
    bool authenticated;
    std::unordered_set<Capability> granted_capabilities;
    
    std::string extract_module_name(const std::string& path);
    
protected:
    static void _bind_methods();
    
public:
    SecurityManager();
    ~SecurityManager();
    
    static SecurityManager* get_singleton();
    
    void set_process_manager(ProcessManager* pm) { process_manager = pm; }
    
    // Basic security checks
    bool check_capability(int pid, Capability cap);
    bool check_privilege(int pid, ProcessPrivilege min_privilege);
    bool can_schedule(int pid, int requested_priority);
    
    // Process creation security
    bool can_create_process(int parent_pid, ProcessPrivilege requested_privilege, CapabilityMask requested_caps);
    
    // Audit logging
    void log_security_event(const std::string& event, int pid, bool allowed);
    
    // Module trust management
    bool is_module_trusted(const std::string& module_path);
    void trust_module(const std::string& module_path);
    void untrust_module(const std::string& module_path);
    
    // User and privilege management
    bool acquire_privileges(const std::string& password);
    void drop_privileges();
    bool has_privileges() const;
    bool check_capability(Capability cap);
    bool authenticate(const std::string& username, const std::string& password);
    void logout();
    void grant_capability(Capability cap);
    void revoke_capability(Capability cap);
};

} // namespace godnux

#endif // GODNUX_SECURITY_MANAGER_HPP