add_executable(testJSON TestJSON.cpp)
target_include_directories(testJSON PUBLIC ${CMAKE_SOURCE_DIR}/modules/JSON)
target_link_libraries(testJSON gmock_main json_poco)
