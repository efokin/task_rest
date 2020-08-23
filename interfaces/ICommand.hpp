
#ifndef TASK_MANAGER_INTERFACES_ICOMMAND_HPP_
#define TASK_MANAGER_INTERFACES_ICOMMAND_HPP_

#include <optional>
#include <string>
#include <list>
#include <chrono>

class ICommand {
 public:
  virtual ~ICommand() = default;
};

class Task : public ICommand {
 public:
  std::optional<int> id;
  std::optional<std::string> description;
  std::optional<std::chrono::system_clock::time_point> created;
  std::optional<int> priority;
};

class TaskCreate : public Task {
};

class IDB {
 public:
  virtual ~IDB() = default;
  virtual int createTask(const Task&) = 0;
  virtual std::list<Task> getTasks() = 0;
  virtual bool deleteTask(int id) = 0;
  virtual bool updateTask(const Task&) = 0;
};

#endif //TASK_MANAGER_INTERFACES_ICOMMAND_HPP_
