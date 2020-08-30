
#ifndef TASK_MANAGER_INTERFACES_ICOMMAND_HPP_
#define TASK_MANAGER_INTERFACES_ICOMMAND_HPP_

#include <optional>
#include <string>
#include <list>
#include <typeindex>
#include <memory>
#include <chrono>

class IEntity {
 public:
  virtual ~IEntity() = default;
};

class Task : public IEntity {
 public:
  std::optional<int> id;
  std::optional<std::string> description;
  std::optional<std::chrono::system_clock::time_point> created;
  std::optional<int> priority;
};

class Subtask : public IEntity {
 public:
  std::optional<int> id;
  std::optional<int> taskId;
  std::optional<std::string> description;
  std::optional<std::chrono::system_clock::time_point> created;
  std::optional<int> priority;
};

class IDB {
 public:
  virtual ~IDB() = default;

  virtual int create(const IEntity&) = 0;
  virtual std::list<std::shared_ptr<IEntity>> getEntities(std::type_index) = 0;
  virtual std::list<std::shared_ptr<IEntity>> findEntities(const IEntity &entity) = 0;
  virtual bool deleteEntity(std::type_index, int id) = 0;
  virtual bool update(const IEntity&) = 0;
};

#endif //TASK_MANAGER_INTERFACES_ICOMMAND_HPP_
