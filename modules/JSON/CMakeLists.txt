find_package(Poco REQUIRED JSON)
add_library(json_poco
        JsonSerializator.cpp
        )

target_link_libraries(json_poco PUBLIC Poco::JSON helper)
