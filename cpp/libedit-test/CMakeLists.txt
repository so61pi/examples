cmake_minimum_required(VERSION 3.5)

project(libclicxx)

set(LIBCLICXX_VERSION_MAJOR 0)
set(LIBCLICXX_VERSION_MINOR 1)
set(LIBCLICXX_VERSION ${LIBCLICXX_VERSION_MAJOR}.${LIBCLICXX_VERSION_MINOR})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(APPEND CMAKE_CXX_FLAGS " -pedantic -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -g3 -O0")
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address -fsanitize=leak -fsanitize=undefined")

set(LIBCLICXX_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(lib)
add_subdirectory(examples)

install(DIRECTORY ${LIBCLICXX_INCLUDE_DIR}/clicxx
    	DESTINATION include)
