#ifndef TASK_MANAGER_ISERVER_HPP
#define TASK_MANAGER_ISERVER_HPP

#include <string>
#include <functional>

enum class HttpMethod {
  get,
  post,
  del,
  unknown
};

/// @param ResponseStream - reference to response stream
/// @param Path
/// @param HttpMethod
using RequestHandlerCallback = std::function<int(std::ostream &ostr, const std::string &, HttpMethod)>;

class IServer {
 public:
  virtual ~IServer() = default;
};

#endif //TASK_MANAGER_ISERVER_HPP
