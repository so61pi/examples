add_executable(cliex Cli.cpp)
target_include_directories(cliex PRIVATE ${LIBCLICXX_INCLUDE_DIR})

target_link_libraries(cliex debug asan)
target_link_libraries(cliex clicxx)
