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
#ifndef GODNUX_MEMORY_MANAGER_HPP
#define GODNUX_MEMORY_MANAGER_HPP

#include "../object.hpp"
#include <cstddef>
#include <cstdint>
#include <map>
#include <mutex>

namespace godnux {

class MemoryManager : public Object {
    GDCLASS(MemoryManager, Object);
    
public:
    struct MemoryRegion {
        void* address;
        size_t size;
        int permissions;
        int process_id;
    };
    
private:
    std::map<void*, MemoryRegion> allocated_regions;
    size_t total_memory = 0;
    size_t used_memory = 0;
    std::mutex memory_mutex;
    
    static const int PROT_READ = 0x1;
    static const int PROT_WRITE = 0x2;
    static const int PROT_EXEC = 0x4;
    
public:
    MemoryManager();
    ~MemoryManager();
    
    // Core allocation
    void* allocate(size_t size, int process_id = 0);
    void deallocate(void* ptr);
    
    // Memory protection
    bool protect_region(void* address, size_t size, int permissions);
    int get_region_permissions(void* address) const;
    
    // System memory info
    size_t get_total_memory() const { return total_memory; }
    size_t get_used_memory() const { return used_memory; }
    size_t get_free_memory() const { return total_memory - used_memory; }
    
    // Memory statistics
    size_t get_allocation_count() const { return allocated_regions.size(); }
};

} // namespace godnux

#endif // GODNUX_MEMORY_MANAGER_HPP