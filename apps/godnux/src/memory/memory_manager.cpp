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
#include "godnux/memory/memory_manager.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>

namespace godnux {

MemoryManager::MemoryManager() {
    total_memory = 1024 * 1024 * 1024; // 1 GB simulated
    used_memory = 0;
    std::cout << "MemoryManager: Initialized with " << total_memory << " bytes" << std::endl;
}

MemoryManager::~MemoryManager() {
    // Clean up any remaining allocations
    for (auto& [address, region] : allocated_regions) {
        std::free(address);
    }
    allocated_regions.clear();
    std::cout << "MemoryManager: Shutdown complete" << std::endl;
}

void* MemoryManager::allocate(size_t size, int process_id) {
    std::lock_guard<std::mutex> lock(memory_mutex);
    
    if (used_memory + size > total_memory) {
        std::cerr << "MemoryManager: Out of memory!" << std::endl;
        return nullptr;
    }
    
    void* memory = std::malloc(size);
    if (!memory) {
        std::cerr << "MemoryManager: Allocation failed for " << size << " bytes" << std::endl;
        return nullptr;
    }
    
    // Initialize to zero for security
    std::memset(memory, 0, size);
    
    MemoryRegion region;
    region.address = memory;
    region.size = size;
    region.permissions = PROT_READ | PROT_WRITE;
    region.process_id = process_id;
    
    allocated_regions[memory] = region;
    used_memory += size;
    
    std::cout << "MemoryManager: Allocated " << size << " bytes at " << memory 
              << " for process " << process_id << std::endl;
    
    return memory;
}

void MemoryManager::deallocate(void* ptr) {
    std::lock_guard<std::mutex> lock(memory_mutex);
    
    auto it = allocated_regions.find(ptr);
    if (it == allocated_regions.end()) {
        std::cerr << "MemoryManager: Attempt to free unknown memory " << ptr << std::endl;
        return;
    }
    
    size_t size = it->second.size;
    std::free(ptr);
    allocated_regions.erase(it);
    used_memory -= size;
    
    std::cout << "MemoryManager: Freed " << size << " bytes at " << ptr << std::endl;
}

bool MemoryManager::protect_region(void* address, size_t size, int permissions) {
    (void)size; // We're not actually implementing memory protection yet
    
    std::lock_guard<std::mutex> lock(memory_mutex);
    
    auto it = allocated_regions.find(address);
    if (it == allocated_regions.end()) {
        std::cerr << "MemoryManager: Unknown memory region " << address << std::endl;
        return false;
    }
    
    it->second.permissions = permissions;
    std::cout << "MemoryManager: Protected region " << address 
              << " with permissions " << permissions << std::endl;
    
    return true;
}

int MemoryManager::get_region_permissions(void* address) const {
    auto it = allocated_regions.find(address);
    if (it != allocated_regions.end()) {
        return it->second.permissions;
    }
    return 0;
}

} // namespace godnux