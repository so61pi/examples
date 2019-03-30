add_executable(${PROJECT_NAME} main.cpp)
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

find_package(Boost 1.58 REQUIRED)

find_library(LIBCPPCMS cppcms)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBCPPCMS})

find_library(LIBBOOSTER booster)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBBOOSTER})

# target_link_libraries(${PROJECT_NAME} PRIVATE template)
