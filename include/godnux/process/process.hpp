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
#ifndef GODNUX_PROCESS_HPP
#define GODNUX_PROCESS_HPP

#include "../object.hpp"
#include "../security/capabilities.hpp"
#include <string>
#include <memory>
#include <vector>
#include <chrono>

namespace godnux {

enum ProcessState {
    CREATED,
    RUNNING,
    SUSPENDED,
    TERMINATED
};

class Process : public Object {
    GDCLASS(Process, Object);
    
public:
    Process();
    ~Process();

    void _bind_methods();

    // Core lifecycle methods
    bool start();
    bool terminate();
    bool suspend();
    bool resume();

    // Getters
    int get_pid() const { return pid; }
    const std::string& get_name() const { return name; }
    ProcessState get_state() const { return state; }

    // Setters
    void set_pid(int p) { pid = p; }
    void set_name(const std::string& n) { name = n; }

    // Parent/child relationships
    void set_parent(Process* p) { parent = p; }
    Process* get_parent() const { return parent; }

    // Security methods
    bool has_capability(Capability cap) const {
        return godnux::has_capability(capabilities, cap);
    }
    
    CapabilityMask get_capabilities() const { return capabilities; }
    ProcessPrivilege get_privilege() const { return privilege; }
    
    void set_capabilities(CapabilityMask caps) { capabilities = caps; }
    void set_privilege(ProcessPrivilege priv) { privilege = priv; }
    
    std::string get_privilege_string() const {
        switch (privilege) {
            case PRIV_KERNEL: return "KERNEL";
            case PRIV_SYSTEM: return "SYSTEM";
            case PRIV_USER: return "USER";
            case PRIV_SANDBOXED: return "SANDBOXED";
            default: return "UNKNOWN";
        }
    }

private:
    int pid;
    std::string name;
    ProcessState state;
    Process* parent;
    
    // Security fields
    CapabilityMask capabilities = CAP_BASIC;
    ProcessPrivilege privilege = PRIV_USER;
};

} // namespace godnux

#endif // GODNUX_PROCESS_HPP
