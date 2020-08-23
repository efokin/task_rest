
#ifndef TASK_MANAGER_MODULES_HTTPSERVER_REQUESTHANDLERFACTORY_HPP_
#define TASK_MANAGER_MODULES_HTTPSERVER_REQUESTHANDLERFACTORY_HPP_

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "IServer.hpp"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServerRequest;

class RequestHandlerFactory : public HTTPRequestHandlerFactory {
 public:
  explicit RequestHandlerFactory(RequestHandlerCallback requestHandlerCallback);

  HTTPRequestHandler *createRequestHandler(const HTTPServerRequest &request);

 private:
  RequestHandlerCallback requestHandlerCallback;
};

#endif //TASK_MANAGER_MODULES_HTTPSERVER_REQUESTHANDLERFACTORY_HPP_
