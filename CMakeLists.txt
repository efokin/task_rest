cmake_minimum_required(VERSION 3.17)
project(task_rest)

set(CMAKE_CXX_STANDARD 17)
if (TESTS_ENABLED)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
endif ()

include_directories(interfaces)
add_subdirectory(modules)

add_executable(task_rest main.cpp)
target_link_libraries(task_rest
        http_poco_server)

option(TESTS_ENABLED "Is it required to build tests?" OFF)

if (TESTS_ENABLED)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
    enable_testing()
    add_subdirectory(tests)
endif ()
