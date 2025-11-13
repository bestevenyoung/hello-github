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
#include "godnux/ipc/ipc_manager.hpp"
#include "godnux/security/security_manager.hpp"
#include <iostream>

namespace godnux {

IPCManager* IPCManager::singleton = nullptr;

IPCManager* IPCManager::get_singleton() {
    if (!singleton) {
        singleton = new IPCManager();
    }
    return singleton;
}

IPCManager::IPCManager() {
    std::cout << "IPCManager: Initialized" << std::endl;
    
    // Create some default system channels
    create_channel("system_broadcast", CAP_NONE);  // Anyone can listen
    create_channel("kernel_events", CAP_KERNEL);   // Kernel only
}

IPCManager::~IPCManager() {
    message_queues.clear();
    message_handlers.clear();
    std::cout << "IPCManager: Shutdown complete" << std::endl;
}

bool IPCManager::send_message(int from_pid, int to_pid, const std::string& channel, const std::string& data, CapabilityMask required_cap) {
    // Security check: does sender have the required capability?
    auto security_mgr = SecurityManager::get_singleton();
    if (security_mgr && !security_mgr->check_capability(from_pid, static_cast<Capability>(required_cap))) {
        std::cerr << "IPCManager: Permission denied for PID " << from_pid 
                  << " to send on channel '" << channel << "'" << std::endl;
        return false;
    }
    
    // Check if channel exists
    if (message_queues.find(channel) == message_queues.end()) {
        std::cerr << "IPCManager: Channel '" << channel << "' does not exist" << std::endl;
        return false;
    }
    
    IPCMessage msg(from_pid, to_pid, channel, data, required_cap);
    message_queues[channel].push(msg);
    
    std::cout << "IPCManager: Message sent from PID " << from_pid 
              << " to PID " << to_pid << " on channel '" << channel << "'" << std::endl;
    return true;
}

bool IPCManager::has_messages(int pid, const std::string& channel) {
    if (message_queues.find(channel) == message_queues.end()) {
        return false;
    }
    
    // In a real implementation, we'd check if this PID has access to the channel
    // For now, just check if there are messages
    return !message_queues[channel].empty();
}

IPCMessage IPCManager::receive_message(int pid, const std::string& channel) {
    if (message_queues.find(channel) == message_queues.end() || message_queues[channel].empty()) {
        return IPCMessage(-1, -1, "", "");
    }
    
    IPCMessage msg = message_queues[channel].front();
    message_queues[channel].pop();
    
    std::cout << "IPCManager: Message received by PID " << pid 
              << " on channel '" << channel << "'" << std::endl;
    return msg;
}

bool IPCManager::create_channel(const std::string& channel, CapabilityMask required_cap) {
    if (message_queues.find(channel) != message_queues.end()) {
        std::cerr << "IPCManager: Channel '" << channel << "' already exists" << std::endl;
        return false;
    }
    
    message_queues[channel] = std::queue<IPCMessage>();
    std::cout << "IPCManager: Created channel '" << channel 
              << "' with required capability 0x" << std::hex << required_cap << std::dec << std::endl;
    return true;
}

bool IPCManager::destroy_channel(const std::string& channel) {
    auto it = message_queues.find(channel);
    if (it == message_queues.end()) {
        std::cerr << "IPCManager: Channel '" << channel << "' does not exist" << std::endl;
        return false;
    }
    
    message_queues.erase(it);
    std::cout << "IPCManager: Destroyed channel '" << channel << "'" << std::endl;
    return true;
}

void IPCManager::_bind_methods() {
    // Will implement later for scripting
}

} // namespace godnux
