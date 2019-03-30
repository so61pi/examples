cmake_minimum_required(VERSION 3.5)
project(cppcms-test CXX)

include(GNUInstallDirs)

# Enable C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCC)
    string(APPEND CMAKE_CXX_FLAGS " -pedantic -Wall -Wextra -Werror")
    string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address -fsanitize=leak -fsanitize=undefined")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(tools)
add_subdirectory(web)
add_subdirectory(config)
