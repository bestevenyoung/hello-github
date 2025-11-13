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
#include "godnux/module/module_discovery.hpp"
#include "godnux/module/module_manifest.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace godnux {

ModuleDiscovery::ModuleDiscovery() {
    // Add default search paths
    search_paths.push_back("modules/core");
    search_paths.push_back("modules/official");
    search_paths.push_back("modules/community");
    search_paths.push_back("modules/experimental");
}

void ModuleDiscovery::add_search_path(const std::string& path) {
    search_paths.push_back(path);
}

void ModuleDiscovery::set_search_paths(const std::vector<std::string>& paths) {
    search_paths = paths;
}

std::vector<ModuleManifest> ModuleDiscovery::discover_modules() {
    std::vector<ModuleManifest> manifests;
    
    for (const auto& search_path : search_paths) {
        if (!std::filesystem::exists(search_path)) {
            std::cout << "ModuleDiscovery: Path does not exist: " << search_path << std::endl;
            continue;
        }
        
        try {
            for (const auto& entry : std::filesystem::directory_iterator(search_path)) {
                if (entry.is_regular_file() && entry.path().extension() == ".json") {
                    auto manifest = load_manifest_from_file(entry.path());
                    if (manifest.is_valid()) {
                        manifests.push_back(manifest);
                        std::cout << "ModuleDiscovery: Found module '" << manifest.name 
                                  << "' in " << search_path << std::endl;
                    }
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "ModuleDiscovery: Error scanning path " << search_path << ": " << e.what() << std::endl;
        }
    }
    
    return manifests;
}

ModuleManifest ModuleDiscovery::discover_module(const std::string& module_name) {
    for (const auto& search_path : search_paths) {
        std::filesystem::path manifest_path = search_path + "/" + module_name + ".json";
        if (std::filesystem::exists(manifest_path)) {
            return load_manifest_from_file(manifest_path);
        }
    }
    
    return ModuleManifest(); // Return invalid manifest
}

bool ModuleDiscovery::validate_module_path(const std::string& path) {
    return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
}

std::string ModuleDiscovery::find_module_library(const std::string& module_name, const std::vector<std::string>& search_paths) {
    // Simple implementation - look for .so files
    for (const auto& path : search_paths) {
        std::string library_path = path + "/lib" + module_name + ".so";
        if (validate_module_path(library_path)) {
            return library_path;
        }
    }
    return "";
}

ModuleManifest ModuleDiscovery::load_manifest_from_file(const std::filesystem::path& manifest_path) {
    ModuleManifest manifest;
    
    try {
        std::ifstream file(manifest_path);
        if (!file.is_open()) {
            std::cerr << "ModuleDiscovery: Cannot open manifest file: " << manifest_path << std::endl;
            return manifest;
        }
        
        // For now, we'll create a simple manifest - in a real implementation
        // we would parse JSON and populate all fields
        std::string module_name = manifest_path.stem().string();
        manifest.name = module_name;
        manifest.version = "1.0.0";
        manifest.description = "Discovered module: " + module_name;
        
        // Set trust level based on directory
        std::string parent_dir = manifest_path.parent_path().filename().string();
        if (parent_dir == "core") manifest.trust_level = TRUST_CORE;
        else if (parent_dir == "official") manifest.trust_level = TRUST_OFFICIAL;
        else if (parent_dir == "community") manifest.trust_level = TRUST_COMMUNITY;
        else manifest.trust_level = TRUST_EXPERIMENTAL;
        
        // Set capabilities based on trust level
        switch (manifest.trust_level) {
            case TRUST_CORE:
                manifest.required_capabilities = CAP_KERNEL;
                manifest.requested_capabilities = CAP_KERNEL;
                break;
            case TRUST_OFFICIAL:
                manifest.required_capabilities = CAP_SYSTEM;
                manifest.requested_capabilities = CAP_SYSTEM;
                break;
            case TRUST_COMMUNITY:
                manifest.required_capabilities = CAP_BASIC;
                manifest.requested_capabilities = CAP_BASIC;
                break;
            case TRUST_EXPERIMENTAL:
                manifest.required_capabilities = CAP_BASIC;
                manifest.requested_capabilities = CAP_BASIC; // Start with minimal
                break;
        }
        
        manifest.entry_point = module_name + "_init";
        manifest.library_path = find_module_library(module_name, search_paths);
        
    } catch (const std::exception& e) {
        std::cerr << "ModuleDiscovery: Error loading manifest from " << manifest_path << ": " << e.what() << std::endl;
    }
    
    return manifest;
}

std::vector<std::filesystem::path> ModuleDiscovery::find_manifest_files() {
    std::vector<std::filesystem::path> manifest_files;
    
    for (const auto& search_path : search_paths) {
        if (!std::filesystem::exists(search_path)) continue;
        
        try {
            for (const auto& entry : std::filesystem::directory_iterator(search_path)) {
                if (entry.is_regular_file() && entry.path().extension() == ".json") {
                    manifest_files.push_back(entry.path());
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "ModuleDiscovery: Error scanning " << search_path << ": " << e.what() << std::endl;
        }
    }
    
    return manifest_files;
}

} // namespace godnux
