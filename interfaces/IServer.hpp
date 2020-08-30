#ifndef TASK_MANAGER_ISERVER_HPP
#define TASK_MANAGER_ISERVER_HPP

#include <string>
#include <functional>

enum class HttpMethod {
  get,
  post,
  del,
  put,
  unknown
};

struct HttpHeader {
  std::string path;
  HttpMethod method;
  size_t contentLength;
};

/// @param ResponseStream - reference to response stream
/// @param Path
/// @param HttpMethod
using RequestHandlerCallback = std::function<int(const HttpHeader &header, std::istream &body, std::ostream &ostr)>;

class IServer {
 public:
  virtual ~IServer() = default;
};

#endif //TASK_MANAGER_ISERVER_HPP
