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
#ifndef GODNUX_MODULE_MANIFEST_HPP
#define GODNUX_MODULE_MANIFEST_HPP

#include "module.hpp"  // This defines ModuleTrustLevel
#include "../security/capabilities.hpp"
#include <string>
#include <vector>
#include <map>

namespace godnux {

struct ModuleManifest {
    std::string name;
    std::string version;
    std::string description;
    ModuleTrustLevel trust_level;
    CapabilityMask required_capabilities;
    CapabilityMask requested_capabilities;
    std::vector<std::string> dependencies;
    std::string entry_point;
    std::string library_path;
    
    // Validation
    bool is_valid() const {
        return !name.empty() && !version.empty() && !entry_point.empty();
    }
    
    // Security: Check if requested capabilities are within required
    bool has_valid_capabilities() const {
        return (requested_capabilities & ~required_capabilities) == 0;
    }
    
    // Voluntary restriction: Module requests less than it could
    bool has_voluntary_restrictions() const {
        return requested_capabilities != required_capabilities;
    }
};

} // namespace godnux

#endif // GODNUX_MODULE_MANIFEST_HPP
