include(GNUInstallDirs)

# find_library is low level in comparison with find_package
find_package(Boost 1.58 REQUIRED)

add_library(example SHARED libexample.cpp)
install(TARGETS example LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

message(STATUS "libexample version: ${LIBEXAMPLE_VERSION}")
set_target_properties(example PROPERTIES VERSION   ${LIBEXAMPLE_VERSION}
                                         SOVERSION ${LIBEXAMPLE_VERSION_MAJOR})
