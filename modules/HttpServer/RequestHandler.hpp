
#ifndef TASK_MANAGER_MODULES_HTTPSERVER_REQUESTHANDLER_HPP_
#define TASK_MANAGER_MODULES_HTTPSERVER_REQUESTHANDLER_HPP_

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "IServer.hpp"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

class RequestHandler: public HTTPRequestHandler
  /// Return a HTML document with the current date and time.
{
 public:
  RequestHandler(RequestHandlerCallback requestHandlerCallback);

  void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response);

 private:
  RequestHandlerCallback requestHandlerCallback;
};

#endif //TASK_MANAGER_MODULES_HTTPSERVER_REQUESTHANDLER_HPP_
