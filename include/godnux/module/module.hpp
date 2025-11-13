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
#ifndef GODNUX_MODULE_HPP
#define GODNUX_MODULE_HPP

#include "../object.hpp"
#include "../security/capabilities.hpp"
#include <string>
#include <map>

namespace godnux {

enum ModuleTrustLevel {
    TRUST_CORE,        // Built-in, audited modules
    TRUST_OFFICIAL,    // Signed/endorsed third-party
    TRUST_COMMUNITY,   // Open-source contributions
    TRUST_EXPERIMENTAL // Requires explicit user approval
};

class Module : public Object {
    GDCLASS(Module, Object);
    
public:
    Module();
    ~Module();

    void _bind_methods();

    // Lifecycle methods
    virtual bool initialize();
    virtual void shutdown();
    
    // Getters
    const std::string& get_name() const { return name; }
    const std::string& get_version() const { return version; }
    ModuleTrustLevel get_trust_level() const { return trust_level; }
    CapabilityMask get_capabilities() const { return capabilities; }
    
    // Setters
    void set_name(const std::string& n) { name = n; }
    void set_version(const std::string& v) { version = v; }
    void set_trust_level(ModuleTrustLevel level) { trust_level = level; }
    void set_capabilities(CapabilityMask caps) { capabilities = caps; }
    
    std::string get_trust_level_string() const {
        switch (trust_level) {
            case TRUST_CORE: return "CORE";
            case TRUST_OFFICIAL: return "OFFICIAL";
            case TRUST_COMMUNITY: return "COMMUNITY";
            case TRUST_EXPERIMENTAL: return "EXPERIMENTAL";
            default: return "UNKNOWN";
        }
    }

private:
    std::string name;
    std::string version;
    ModuleTrustLevel trust_level;
    CapabilityMask capabilities;
};

} // namespace godnux

#endif // GODNUX_MODULE_HPP
