cmake_minimum_required(VERSION 3.0)
project(mini-memory-scanner)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.5.8
    REQUIRED
    COMPONENTS program_options)

add_definitions(/DUNICODE)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

set(SOURCES
    parser/option_parser.cpp
    parser/pattern_parser.cpp
    process/process.cpp
    process/memory/utility.cpp
    utility/handler_wrapper.cpp
    main.cpp)

add_executable(mini-memory-scanner ${SOURCES})
target_link_libraries(mini-memory-scanner ${Boost_LIBRARIES})
