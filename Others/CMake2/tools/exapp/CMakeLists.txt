if(NOT DEFINED CONFIG_STRING)
    set(CONFIG_STRING "CONFIG_STRING is missing!")
endif()

add_executable(exapp main.cpp)
install(TARGETS exapp DESTINATION ${CMAKE_INSTALL_BINDIR})

configure_file(config.h.in config.h)
target_include_directories(exapp PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# libexample is in our tree, do not need to use find_library/find_package
target_link_libraries(exapp PRIVATE example)

#find_library(LIBSYSTEMD NAMES systemd libsystemd)
#if(NOT LIBSYSTEMD)
#    message(FATAL_ERROR "systemd library not found!")
#endif()
#target_link_libraries(exapp PRIVATE ${LIBSYSTEMD})
