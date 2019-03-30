find_package(Boost REQUIRED)

add_library(clicxx SHARED Editline.cpp Handler.cpp)
target_include_directories(clicxx PRIVATE ${LIBCLICXX_INCLUDE_DIR})

target_link_libraries(clicxx debug asan)
target_link_libraries(clicxx edit)

set_target_properties(clicxx PROPERTIES VERSION   ${LIBCLICXX_VERSION}
                                        SOVERSION ${LIBCLICXX_VERSION_MAJOR})

install(TARGETS clicxx
        LIBRARY DESTINATION lib)
