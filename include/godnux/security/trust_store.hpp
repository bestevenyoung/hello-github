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
#ifndef GODNUX_TRUST_STORE_HPP
#define GODNUX_TRUST_STORE_HPP

#include "../module/module.hpp"
#include <string>
#include <unordered_map>

namespace godnux {

class TrustStore {
private:
    std::unordered_map<std::string, ModuleTrustLevel> trusted_modules;
    
    void load_trusted_modules();
    std::string trust_level_to_string(ModuleTrustLevel level);

public:
    TrustStore();
    
    bool is_module_trusted(const std::string& module_name, ModuleTrustLevel required_level);
    void add_trusted_module(const std::string& module_name, ModuleTrustLevel level);
    void remove_trusted_module(const std::string& module_name);
};

} // namespace godnux

#endif // GODNUX_TRUST_STORE_HPP