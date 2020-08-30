
#ifndef TASK_REST_MODULES_HTTPSERVER_URIDISPATCHER_HPP_
#define TASK_REST_MODULES_HTTPSERVER_URIDISPATCHER_HPP_

#include <IServer.hpp>
#include <IDispatcher.hpp>

int pathDisatch(const HttpHeader &header, std::istream &body, std::ostream &ostr);

class Dispatcher : public IDispatcher {
 public:
  int dispatch(const HttpHeader& header, std::istream& body, std::ostream &ostr);
  virtual int getTasks(std::istream &body, size_t contentLength, std::ostream &ostr) = 0;
  virtual int findTasks(std::istream &body, size_t contentLength, std::ostream &ostr) = 0;
  virtual int addTask(std::istream &body, size_t contentLenght, std::ostream &ostr) = 0;
  virtual int deleteTask(int id) = 0;
  virtual int updateTask(int id, std::istream &body, size_t contentLenght, std::ostream &ostr) = 0;

  virtual int getSubtasks(std::istream &body, size_t contentLength, std::ostream &ostr) = 0;
  virtual int addSubtask(std::istream &body, size_t contentLenght, std::ostream &ostr) = 0;
  virtual int deleteSubtask(int id) = 0;
  virtual int updateSubtask(int id, std::istream &body, size_t contentLenght, std::ostream &ostr) = 0;
};

class DispatcherImpl : public Dispatcher {
 public:
  int getTasks(std::istream &body, size_t contentLength, std::ostream &ostr) override;
  int findTasks(std::istream &body, size_t contentLength, std::ostream &ostr) override;
  int addTask(std::istream &body, size_t contentLenght, std::ostream &ostr) override;
  int deleteTask(int id) override;
  int updateTask(int id, std::istream &body, size_t contentLenght, std::ostream &ostr) override;

  int getSubtasks(std::istream &body, size_t contentLength, std::ostream &ostr) override;
  int addSubtask(std::istream &body, size_t contentLenght, std::ostream &ostr) override;
  int deleteSubtask(int id) override;
  int updateSubtask(int id, std::istream &body, size_t contentLenght, std::ostream &ostr) override;
};

#endif //TASK_REST_MODULES_HTTPSERVER_URIDISPATCHER_HPP_
