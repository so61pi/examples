cmake_minimum_required(VERSION 3.5)
project(example CXX)

include(GNUInstallDirs)

# Build configurations
option(BUILD_WITH_SANITIZER "Build with sanitizer in debug mode" ON)

# Enable C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set compiler flags
if(CMAKE_COMPILER_IS_GNUCC)
    string(APPEND CMAKE_CXX_FLAGS " -pedantic -Wall -Wextra -Werror")
    if(BUILD_WITH_SANITIZER)
        string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address -fsanitize=leak -fsanitize=undefined")
    endif()
endif()

# Add top include/ to search path
include_directories(BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ... and install include/example into system
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/example
        DESTINATION include
        FILES_MATCHING
        PATTERN "*.h")

add_subdirectory(lib)
add_subdirectory(tools)
