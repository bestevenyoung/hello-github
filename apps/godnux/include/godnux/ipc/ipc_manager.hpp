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
#ifndef GODNUX_IPC_MANAGER_HPP
#define GODNUX_IPC_MANAGER_HPP

#include "../object.hpp"
#include "../security/capabilities.hpp"
#include <string>
#include <map>
#include <queue>
#include <functional>

namespace godnux {

class IPCMessage {
public:
    int sender_pid;
    int receiver_pid;
    std::string channel;
    std::string data;
    CapabilityMask required_capability;
    
    IPCMessage(int from, int to, const std::string& chan, const std::string& msg, CapabilityMask cap = CAP_IPC)
        : sender_pid(from), receiver_pid(to), channel(chan), data(msg), required_capability(cap) {}
};

class IPCManager : public Object {
    GDCLASS(IPCManager, Object);
    
private:
    static IPCManager* singleton;
    std::map<std::string, std::queue<IPCMessage>> message_queues;
    std::map<std::string, std::function<void(const IPCMessage&)>> message_handlers;
    
protected:
    static void _bind_methods();
    
public:
    IPCManager();
    ~IPCManager();
    
    static IPCManager* get_singleton();
    
    // Secure message sending
    bool send_message(int from_pid, int to_pid, const std::string& channel, const std::string& data, CapabilityMask required_cap = CAP_IPC);
    
    // Message receiving
    bool has_messages(int pid, const std::string& channel);
    IPCMessage receive_message(int pid, const std::string& channel);
    
    // Channel management
    bool create_channel(const std::string& channel, CapabilityMask required_cap = CAP_IPC);
    bool destroy_channel(const std::string& channel);
};

} // namespace godnux

#endif // GODNUX_IPC_MANAGER_HPP
