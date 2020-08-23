#include <iostream>
#include "modules/HttpServer/HttpPocoServer.hpp"
#include "modules/HttpServer/PathDispatcher.hpp"

int main(int argc, char **arg) {
  HTTPPocoServer poco_srv = HTTPPocoServer(pathDisatch);
  poco_srv.run(argc, arg);
  return 0;
}
