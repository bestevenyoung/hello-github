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
#include "godnux/module.hpp"

namespace godnux {

Module::Module() {
    std::cout << "Module base constructor" << std::endl;
}

Module::~Module() {
    std::cout << "Module base destructor: " << get_class() << std::endl;
}

} // namespace godnux