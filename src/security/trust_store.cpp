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
#include "godnux/security/trust_store.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace godnux {

TrustStore::TrustStore() {
    load_trusted_modules();
}

void TrustStore::load_trusted_modules() {
    // Load core modules as always trusted
    trusted_modules["kernel"] = TRUST_CORE;
    trusted_modules["memory_manager"] = TRUST_CORE;
    trusted_modules["security_manager"] = TRUST_CORE;
    
    std::cout << "TrustStore: Loaded " << trusted_modules.size() << " core trusted modules" << std::endl;
}

bool TrustStore::is_module_trusted(const std::string& module_name, ModuleTrustLevel required_level) {
    auto it = trusted_modules.find(module_name);
    if (it == trusted_modules.end()) {
        std::cout << "TrustStore: Module '" << module_name << "' not in trust store" << std::endl;
        return false;
    }
    
    ModuleTrustLevel actual_level = it->second;
    bool trusted = (actual_level >= required_level);
    
    std::cout << "TrustStore: Module '" << module_name << "' trust check: " 
              << trust_level_to_string(actual_level) << " >= " 
              << trust_level_to_string(required_level) << " = " 
              << (trusted ? "TRUSTED" : "UNTRUSTED") << std::endl;
    
    return trusted;
}

void TrustStore::add_trusted_module(const std::string& module_name, ModuleTrustLevel level) {
    trusted_modules[module_name] = level;
    std::cout << "TrustStore: Added '" << module_name << "' as " << trust_level_to_string(level) << std::endl;
}

void TrustStore::remove_trusted_module(const std::string& module_name) {
    trusted_modules.erase(module_name);
    std::cout << "TrustStore: Removed '" << module_name << "' from trust store" << std::endl;
}

std::string TrustStore::trust_level_to_string(ModuleTrustLevel level) {
    switch (level) {
        case TRUST_CORE: return "CORE";
        case TRUST_OFFICIAL: return "OFFICIAL";
        case TRUST_COMMUNITY: return "COMMUNITY";
        case TRUST_EXPERIMENTAL: return "EXPERIMENTAL";
        default: return "UNKNOWN";
    }
}

} // namespace godnux