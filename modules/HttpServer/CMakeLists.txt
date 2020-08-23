find_package(Poco REQUIRED Net Util)
add_library(http_poco_server
        HttpPocoServer.cpp
        RequestHandlerFactory.cpp
        RequestHandler.cpp
        PathDispatcher.cpp
        )
target_link_libraries(http_poco_server PUBLIC Poco::Net Poco::Util json_poco postgresql)