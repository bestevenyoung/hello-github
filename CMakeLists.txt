cmake_minimum_required(VERSION 3.16)
project(godnux VERSION 0.6.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Core kernel with all subsystems

set(KERNEL_SOURCES
    src/kernel/kernel.cpp
    src/kernel/main.cpp
    src/object/object.cpp
    src/memory/memory_manager.cpp
    src/process/process.cpp
    src/process/process_manager.cpp
    src/security/security_manager.cpp
    src/security/trust_store.cpp      # KEEP THIS
    src/ipc/ipc_manager.cpp
    src/module/module.cpp
    src/module/module_registry.cpp
    src/module/test_module.cpp
    src/module/shared_library_loader.cpp
    src/module/dynamic_module.cpp
    src/module/module_discovery.cpp
    src/module/module_security_manager.cpp
    src/cli/godnuxctl.cpp
)

add_executable(godnux ${KERNEL_SOURCES})
target_compile_options(godnux PRIVATE -std=c++17 -Wall -Wextra)

# Link with dl library on Unix-like systems
if(UNIX AND NOT APPLE)
    target_link_libraries(godnux dl)
endif()
