
#include "HttpPocoServer.hpp"
#include "RequestHandlerFactory.hpp"
#include "Poco/Util/Option.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/ThreadPool.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTTPServer.h"

using Poco::Util::Option;
using Poco::DateTimeFormat;
using Poco::ThreadPool;
using Poco::Net::HTTPServerParams;
using Poco::Net::ServerSocket;
using Poco::Net::HTTPServer;

HTTPPocoServer::HTTPPocoServer(RequestHandlerCallback the_requestHandlerCallback) :
    requestHandlerCallback{the_requestHandlerCallback} {

}

void HTTPPocoServer::initialize(Application &self) {
  loadConfiguration(); // load default configuration files, if present
  ServerApplication::initialize(self);
}
void HTTPPocoServer::uninitialize() {
  ServerApplication::uninitialize();
}
void HTTPPocoServer::defineOptions(OptionSet &options) {
  ServerApplication::defineOptions(options);
  options.addOption(
      Option("help", "h", "display help information on command line arguments")
          .required(false)
          .repeatable(false));
}
int HTTPPocoServer::main(const std::vector<std::string> &args) {
  {
    // get parameters from configuration file
    unsigned short port = 9080;
    int maxQueued = 10;
    int maxThreads = 1;
    ThreadPool::defaultPool().addCapacity(maxThreads);

    HTTPServerParams *pParams = new HTTPServerParams;
    pParams->setMaxQueued(maxQueued);
    pParams->setMaxThreads(maxThreads);

    // set-up a server socket
    ServerSocket svs(port);
    // set-up a HTTPServer instance
    HTTPServer srv(new RequestHandlerFactory(requestHandlerCallback), svs, pParams);
    // start the HTTPServer
    srv.start();
    // wait for CTRL-C or kill
    waitForTerminationRequest();
    // Stop the HTTPServer
    srv.stop();
  }
  return Application::EXIT_OK;
}
