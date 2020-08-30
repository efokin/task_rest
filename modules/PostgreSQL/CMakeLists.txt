
find_package(PostgreSQL REQUIRED)
add_library(postgresql
        PGConnection.cpp
        PGTask.cpp
        PGSubtask.cpp
        )
target_link_libraries(postgresql PUBLIC ${PostgreSQL_LIBRARIES} helper)