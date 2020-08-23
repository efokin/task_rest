cmake_minimum_required(VERSION 3.17)
project(task_rest)

set(CMAKE_CXX_STANDARD 17)

include_directories(interfaces)
add_subdirectory(modules)

add_executable(task_rest main.cpp)
target_link_libraries(task_rest
        http_poco_server)

option(TESTS_ENABLED "Is it required to build tests?" ON)

if (TESTS_ENABLED)
    enable_testing()
    add_subdirectory(tests)
endif ()
