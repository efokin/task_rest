
#ifndef TASK_MANAGER_MODULES_HTTPPOCOSERVER_HPP_
#define TASK_MANAGER_MODULES_HTTPPOCOSERVER_HPP_

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>

#include "IServer.hpp"

using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::OptionSet;


class HTTPPocoServer : public Poco::Util::ServerApplication, IServer
  /// The main application class.

  /// To test the Server you can use any web browser (http://localhost:9980/).
{
 public:
  HTTPPocoServer(RequestHandlerCallback);

 protected:
  void initialize(Application &self);

  void uninitialize();

  void defineOptions(OptionSet &options);

  int main(const std::vector <std::string> &args);

 private:
  RequestHandlerCallback requestHandlerCallback;
};

#endif //TASK_MANAGER_MODULES_HTTPPOCOSERVER_HPP_
