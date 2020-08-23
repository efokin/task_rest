
#include "RequestHandlerFactory.hpp"
#include "RequestHandler.hpp"

HTTPRequestHandler *RequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request) {
  return new RequestHandler(requestHandlerCallback);
}
RequestHandlerFactory::RequestHandlerFactory(RequestHandlerCallback the_requestHandlerCallback) :
    requestHandlerCallback(the_requestHandlerCallback) {
}
