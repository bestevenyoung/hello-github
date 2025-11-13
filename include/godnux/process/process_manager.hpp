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
#ifndef GODNUX_PROCESS_MANAGER_HPP
#define GODNUX_PROCESS_MANAGER_HPP

#include "../object.hpp"
#include "../security/capabilities.hpp"
#include "process.hpp"
#include <vector>
#include <memory>
#include <map>
#include <queue>

namespace godnux {

class ProcessManager : public Object {
    GDCLASS(ProcessManager, Object);
    
public:
    enum SchedulerType {
        GODNUX_SCHED_FIFO,
        GODNUX_SCHED_ROUND_ROBIN,
        GODNUX_SCHED_PRIORITY
    };
    
private:
    std::map<int, std::unique_ptr<Process>> processes;
    std::queue<Process*> ready_queue;
    std::vector<Process*> blocked_processes;
    Process* current_process = nullptr;
    int next_pid = 1;
    SchedulerType scheduler_type = GODNUX_SCHED_ROUND_ROBIN;
    
protected:
    static void _bind_methods();
    
public:
    ProcessManager();
    ~ProcessManager();
    
    // Process lifecycle - original method for backward compatibility
    Process* create_process(const std::string& name, int priority = 0);
    
    // New secure process creation method
    Process* create_secure_process(const std::string& name, 
                                  ProcessPrivilege privilege = PRIV_USER,
                                  CapabilityMask capabilities = CAP_BASIC);
    
    bool destroy_process(int pid);
    Process* get_process(int pid);
    std::vector<Process*> get_all_processes();
    
    // Process states
    bool suspend_process(int pid);
    bool resume_process(int pid);
    bool terminate_process(int pid);
    
    // Scheduling
    void set_scheduler_type(SchedulerType type);
    void schedule();
    void yield();
    Process* get_current_process() const { return current_process; }
    
    // Statistics
    int get_process_count() const { return processes.size(); }
    int get_running_count() const;
    int get_blocked_count() const { return blocked_processes.size(); }
};

} // namespace godnux

#endif // GODNUX_PROCESS_MANAGER_HPP
