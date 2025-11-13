# Module Template CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(${MODULE_NAME} VERSION 1.0.0 LANGUAGES CXX)

# Module configuration
set(MODULE_NAME "your_module_name")
set(MODULE_CATEGORY "category")  # core, scripting, rendering, etc.

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GODNUX_SOURCE_DIR}/core
)

# Source files
file(GLOB MODULE_SOURCES
    "src/*.cpp"
)

# Create shared library
add_library(${MODULE_NAME} SHARED ${MODULE_SOURCES})

# Link dependencies
target_link_libraries(${MODULE_NAME} godnux_core)

# Set output properties
set_target_properties(${MODULE_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${GODNUX_BINARY_DIR}/modules/${MODULE_CATEGORY}
    PREFIX ""
)

# Install rules
install(TARGETS ${MODULE_NAME}
    DESTINATION lib/godnux/modules/${MODULE_CATEGORY}
)