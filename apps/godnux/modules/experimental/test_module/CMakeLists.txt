# Test Module CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(test_module)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
)

# Source files
set(MODULE_SOURCES
    src/test_module.cpp
)

# Create shared library
add_library(test_module SHARED ${MODULE_SOURCES})

# Set output directory
set_target_properties(test_module PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules/experimental
    PREFIX ""
)

# Compiler flags
target_compile_options(test_module PRIVATE -std=c++17 -fPIC)

# Link against standard C++ library
target_link_libraries(test_module stdc++fs)  # For std::filesystem if needed

# -------------------------------
# Add test executable
# -------------------------------
add_executable(test_module_loading
    tests/test_loading.cpp
)

# Compiler flags for test executable
target_compile_options(test_module_loading PRIVATE -std=c++17)

# Link against dl (for dynamic loading) and stdc++fs
target_link_libraries(test_module_loading
    dl
    stdc++fs
    test_module   # <-- also link against the module itself
)
# Test Module CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(test_module)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include
)

# Source files
set(MODULE_SOURCES
    src/test_module.cpp
)

# Create shared library
add_library(test_module SHARED ${MODULE_SOURCES})

# Set output directory
set_target_properties(test_module PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules/experimental
    PREFIX ""
)

# Compiler flags
target_compile_options(test_module PRIVATE -std=c++17 -fPIC)

# Link against standard C++ library
target_link_libraries(test_module stdc++fs)  # For std::filesystem if needed