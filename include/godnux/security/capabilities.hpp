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
#ifndef GODNUX_CAPABILITIES_HPP
#define GODNUX_CAPABILITIES_HPP

#include <cstdint>

namespace godnux {

// Capability flags - inspired by Linux but tailored for our needs
enum Capability : uint32_t {
    CAP_NONE       = 0,
    CAP_SPAWN      = 1 << 0,   // Create new processes
    CAP_KILL       = 1 << 1,   // Terminate processes  
    CAP_ALLOC      = 1 << 2,   // Memory allocation
    CAP_IPC        = 1 << 3,   // Inter-process communication
    CAP_MODULE     = 1 << 4,   // Load/unload modules
    CAP_FS         = 1 << 5,   // Filesystem access
    CAP_NET        = 1 << 6,   // Network access
    CAP_HW         = 1 << 7,   // Hardware access (GPU, etc.)
    CAP_AUDIT      = 1 << 8,   // Read audit logs
    CAP_SUDO       = 1 << 9,   // Escalate privileges
    
    // Predefined capability sets
    CAP_BASIC      = CAP_ALLOC | CAP_IPC,
    CAP_SYSTEM     = CAP_BASIC | CAP_SPAWN | CAP_MODULE | CAP_FS,
    CAP_KERNEL     = 0xFFFFFFFF  // All capabilities
};

using CapabilityMask = uint32_t;

enum ProcessPrivilege {
    PRIV_KERNEL,     // Core kernel components
    PRIV_SYSTEM,     // System daemons/services  
    PRIV_USER,       // Regular user processes
    PRIV_SANDBOXED   // Untrusted/third-party code
};

// Helper functions
constexpr bool has_capability(CapabilityMask mask, Capability cap) {
    return (mask & static_cast<uint32_t>(cap)) != 0;
}

constexpr CapabilityMask add_capability(CapabilityMask mask, Capability cap) {
    return mask | static_cast<uint32_t>(cap);
}

} // namespace godnux

#endif // GODNUX_CAPABILITIES_HPP