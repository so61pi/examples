cmake_minimum_required(VERSION 3.5)
project(test-connect CXX)

include(GNUInstallDirs)

# Enable C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX)
    string(APPEND CMAKE_CXX_FLAGS " -pedantic -Wall -Wextra -Werror")
    string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address -fsanitize=leak -fsanitize=undefined")
endif()

add_executable(${PROJECT_NAME} main.cpp)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

find_package(Qt5 REQUIRED COMPONENTS Core Sql)
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Sql)
