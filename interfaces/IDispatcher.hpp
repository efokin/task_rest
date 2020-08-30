
#ifndef TASK_REST_INTERFACES_IDISPATCHER_HPP_
#define TASK_REST_INTERFACES_IDISPATCHER_HPP_

#include "IServer.hpp"

class IDispatcher {
 public:
  virtual ~IDispatcher() {};
  virtual int dispatch(const HttpHeader& header, std::istream& body, std::ostream &ostr) = 0;
};

#endif //TASK_REST_INTERFACES_IDISPATCHER_HPP_
