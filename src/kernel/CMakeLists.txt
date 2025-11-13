add_executable(godnux_kernel 
    main.cpp 
    godnux_kernel.cpp
    object.cpp
    test_object.cpp
    test_module.cpp
    module_discovery.cpp
    shared_library_loader.cpp
    module_manager.cpp
)
target_include_directories(godnux_kernel PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(godnux_kernel dl)