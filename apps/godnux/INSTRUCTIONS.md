# Godnux Module Development Guide

This document provides instructions for developers who want to build modules for the **Godnux kernel** using AI assistants such as **DeepSeek**, **DeepThink**, **Copilot**, or other compatible systems.  

By following these steps, you’ll be able to design, compile, and load modules into the Godnux engine, leveraging its Linux-inspired kernel and dynamic module infrastructure.

---

## 🔎 System Overview
Godnux is a development engine with a modular, Linux-inspired kernel that supports **dynamic module loading**.  
The system is actively evolving, with the following components already functional:

- ✅ Kernel core (memory, process, security managers)  
- ✅ Trust infrastructure (TrustStore, SecurityManager)  
- ✅ Module discovery and registry  
- ✅ **Dynamic loading proven working** with a minimal test module  
- ✅ CLI commands for module management  
- 🔄 Development Mode: All modules currently trusted (security enforcement pending)

---

## 📦 Module Requirements

### 1. Basic Structure
- Modules are **shared libraries (.so)** compiled for Linux x86-64.  
- Written in **C/C++** with **C linkage** for entry points.  
- Must export a `create_<module_name>` function.  
- Should export a `destroy_<module_name>` function (optional but recommended).

### 2. Entry Point Signature
```cpp
extern "C" {
    void* create_<module_name>() {
        // Module initialization logic
        return module_instance; // Can be nullptr for simple modules
    }
}

# Godnux Module Development Guide

## System Overview

Godnux is a development engine with a Linux-inspired kernel that supports dynamic module loading. The system is in active development with basic kernel + module infrastructure working.

## Current Status

- ✅ **Kernel core functional** (memory, process, security managers)
- ✅ **Trust infrastructure in place** (TrustStore, SecurityManager)  
- ✅ **Module discovery and registry** working
- ✅ **Dynamic loading proven working** with minimal test module
- ✅ **CLI module management commands** available
- 🔄 **Development Mode**: All modules currently trusted (enforcement pending)

## Module Requirements

### 1. Basic Structure

- Shared library (.so) compiled for Linux x86-64
- Standard C/C++ with C linkage for entry points
- Must export `create_<module_name>` function
- Should export `destroy_<module_name>` function (optional but recommended)

### 2. Entry Point Signature

```cpp
extern "C" {
    void* create_<module_name>() {
        // Module initialization logic
        return module_instance; // Can be nullptr for simple modules
    }
    
    // Optional but recommended
    void destroy_<module_name>(void* instance) {
        // Module cleanup logic
    }
}
```

### 3. Compilation

```bash
# From the godnux root directory (where CMakeLists.txt is located)
g++ -shared -fPIC -o modules/<category>/lib<module_name>.so src/<module_name>.cpp
```

### 4. Module Directory Structure

```
modules/
├── core/          # Trusted system modules (TRUST_CORE)
├── official/      # Officially supported modules (TRUST_OFFICIAL)  
├── community/     # User-contributed modules (TRUST_COMMUNITY)
├── experimental/  # Untrusted/development modules (TRUST_EXPERIMENTAL)
└── minimal/       # Working test module (proof of concept)
```

## Current Working Example

### Minimal Module Code

```cpp
// modules/minimal/minimal_module.cpp
#include <iostream>

extern "C" {
    void* create_minimal_module() {
        std::cout << "Minimal module created!" << std::endl;
        return nullptr;
    }
    
    void destroy_minimal_module(void* instance) {
        std::cout << "Minimal module destroyed!" << std::endl;
    }
}
```

### Compile the Module

```bash
# From godnux root directory
mkdir -p modules/minimal
g++ -shared -fPIC -o modules/minimal/libminimal.so modules/minimal/minimal_module.cpp
```

## Loading Modules

### CLI Commands

```bash
# From build directory
./godnux modules                  # Show module management help
./godnux module-list             # List available modules
./godnux module-load <name>      # Load a module by name
./godnux module-unload <name>    # Unload a module by name
```

### Example: Load Minimal Module

```bash
cd build
./godnux module-load minimal
```

**Expected Output:**
```
SharedLibraryLoader: Loaded library '../modules/minimal/libminimal.so'
✓ Successfully loaded: ../modules/minimal/libminimal.so
SharedLibraryLoader: Resolved symbol 'create_minimal_module'
✓ Found module entry point
Minimal module created!
✓ Module instance created: 0
SharedLibraryLoader: Unloaded library '../modules/minimal/libminimal.so'
```

## Available Infrastructure

### Core Classes

- **`SharedLibraryLoader`** - Cross-platform library loading
- **`ModuleRegistry`** - Module registration and tracking  
- **`ModuleSecurityManager`** - Trust and capability checks
- **`ModuleDiscovery`** - Directory scanning and manifest loading
- **`TrustStore`** - Trust level management

### Trust Levels

```cpp
enum ModuleTrustLevel {
    TRUST_CORE,        // Core system modules
    TRUST_OFFICIAL,    // Officially supported
    TRUST_COMMUNITY,   // User-contributed
    TRUST_EXPERIMENTAL // Untrusted/development
};
```

### Capability System

```cpp
enum Capability : uint32_t {
    CAP_NONE       = 0,
    CAP_SPAWN      = 1 << 0,   // Create new processes
    CAP_KILL       = 1 << 1,   // Terminate processes  
    CAP_ALLOC      = 1 << 2,   // Memory allocation
    CAP_IPC        = 1 << 3,   // Inter-process communication
    CAP_MODULE     = 1 << 4,   // Load/unload modules
    CAP_FS         = 1 << 5,   // Filesystem access
    CAP_NET        = 1 << 6,   // Network access
    CAP_HW         = 1 << 7,   // Hardware access
    CAP_AUDIT      = 1 << 8,   // Read audit logs
    CAP_SUDO       = 1 << 9,   // Escalate privileges
    
    // Predefined capability sets
    CAP_BASIC      = CAP_ALLOC | CAP_IPC,
    CAP_SYSTEM     = CAP_BASIC | CAP_SPAWN | CAP_MODULE | CAP_FS,
    CAP_KERNEL     = 0xFFFFFFFF  // All capabilities
};
```

## Development Mode Notes

### Current Behavior
- **All modules are trusted** (security checks bypassed)
- **No sandboxing** enforced
- **Resource quotas** not implemented
- **Trust level checks** in infrastructure but not enforced

### Future Enhancements
- Trust level enforcement (CORE > OFFICIAL > COMMUNITY > EXPERIMENTAL)
- Capability-based security enforcement
- Resource quota management
- Sandboxing for untrusted modules

## Module Manifest (Future)

```json
{
    "name": "module_name",
    "version": "1.0.0",
    "description": "Module description",
    "author": "Author Name",
    "license": "MIT",
    "trust_level": "community",
    "required_capabilities": ["CAP_BASIC"],
    "requested_capabilities": ["CAP_FS"],
    "dependencies": [],
    "entry_point": "create_module_name"
}
```

## Verification

### Check Module File

```bash
file modules/minimal/libminimal.so
```

**Expected Output:**
```
modules/minimal/libminimal.so: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, not stripped
```

### Test Loading

```bash
cd build
./godnux module-load minimal
```

## Next Development Priorities

1. **API Module** - Create interface for external systems
2. **Module Interface Extension** - Proper initialization/shutdown cycles
3. **Registry Integration** - Make loaded modules discoverable system-wide
4. **Security Enforcement** - Activate trust level and capability checks
5. **Resource Management** - Implement quotas and monitoring

## Building Complex Modules

For more complex modules that need to interact with kernel systems:

```cpp
#include "godnux/kernel.hpp"
#include <iostream>

extern "C" {
    void* create_advanced_module() {
        auto kernel = godnux::Kernel::get_singleton();
        if (kernel && kernel->initialize()) {
            std::cout << "Advanced module: Kernel access available" << std::endl;
            // Access kernel subsystems here
        }
        return nullptr;
    }
}
```

---

**Note**: This documentation reflects the current development state. The system is evolving rapidly, and some features may change as development progresses.
