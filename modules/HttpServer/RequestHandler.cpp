
#include "RequestHandler.hpp"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/HTTPResponse.h"
#include <sstream>

using Poco::Util::Application;
using Poco::Net::HTTPResponse;

RequestHandler::RequestHandler(RequestHandlerCallback the_requestHandlerCallback) :
    requestHandlerCallback(the_requestHandlerCallback) {
}

HttpMethod getHttpMethod(const std::string& method) {
  if( method.compare("GET") == 0) return HttpMethod::get;
  return HttpMethod::unknown;
}

void RequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response) {
  Application &app = Application::instance();
  app.logger().information("Request from " + request.clientAddress().toString());
  app.logger().information("URI:" + request.getURI());
  app.logger().information("Http method:" + request.getMethod());

  response.setChunkedTransferEncoding(true);
  response.setContentType("application/json");

  std::stringstream sstr;
  HttpMethod httpMethod = getHttpMethod(request.getMethod());

  //if(httpMethod == HttpMethod::unknown) return;
  auto response_code = requestHandlerCallback(sstr, request.getURI(), httpMethod);
  response.setStatusAndReason(HTTPResponse::HTTPStatus(response_code),"reason");
  app.logger().information("Response reason:" + response.getReason());
  app.logger().information("Response status:" + std::to_string( response.getStatus()));

  std::ostream &ostr = response.send();
  ostr << sstr.str();
}
