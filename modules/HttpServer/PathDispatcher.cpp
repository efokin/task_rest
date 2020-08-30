
#include "PathDispatcher.hpp"
#include <regex>
#include <cstdio>
#include "../PostgreSQL/PGConnection.hpp"
#include "../JSON/JsonSerializator.hpp"
#include "Helper.hpp"

PGConnection &getConnection() {
  static PGConnection connection{"host=localhost dbname=tasks user=task_tracker_user password=password"};
  connection.chechConnection();
  return connection;
}

int pathDisatch(const HttpHeader &header, std::istream &body, std::ostream &ostr) {
  return DispatcherImpl().dispatch(header, body, ostr);
}

int Dispatcher::dispatch(const HttpHeader &header, std::istream &body, std::ostream &ostr) {
  if (header.path.rfind("/task", 0) == 0) {
    switch (header.method) {
      case HttpMethod::get: {
        if (header.path.rfind("/tasks", 0) == 0)
          return findTasks(body, header.contentLength, ostr);
        else
          return getTasks(body, header.contentLength, ostr);
      }
      case HttpMethod::post: return addTask(body, header.contentLength, ostr);
      case HttpMethod::del: {
        int id{};
        if (header.path.length() <= 6) return 404; // should be /task/%d
        auto readParameters = sscanf(header.path.c_str() + 6, "%d", &id);
        if (readParameters != 1) return 404;
        return deleteTask(id);
      }
      case HttpMethod::put : {
        int id{};
        if (header.path.length() <= 6) return 404; // should be /task/%d
        auto readParameters = sscanf(header.path.c_str() + 6, "%d", &id);
        if (readParameters != 1) return 404;
        return updateTask(id, body, header.contentLength, ostr);
      }
      default: return 404;
    }
  }
  else if (header.path.rfind("/subtask", 0) == 0) {
    switch (header.method) {
      case HttpMethod::get: {
          return getSubtasks(body, header.contentLength, ostr);
      }
      case HttpMethod::post: return addSubtask(body, header.contentLength, ostr);
      case HttpMethod::del: {
        int id{};
        if (header.path.length() <= 9) return 404; // should be /subtask/%d
        auto readParameters = sscanf(header.path.c_str() + 9, "%d", &id);
        if (readParameters != 1) return 404;
        return deleteSubtask(id);
      }
      case HttpMethod::put : {
        int id{};
        if (header.path.length() <= 9) return 404; // should be /subtask/%d
        auto readParameters = sscanf(header.path.c_str() + 9, "%d", &id);
        if (readParameters != 1) return 404;
        return updateSubtask(id, body, header.contentLength, ostr);
      }
      default: return 404;
    }
  }
  return 404;
}

int DispatcherImpl::getTasks(std::istream &body, size_t contentLength, std::ostream &ostr) {
  auto tasks = getConnection().getEntities(typeid(Task));
  ostr << "[";
  bool first = true;
  for (auto &task: tasks) {
    if (!first) ostr << ',';
    else first = false;
    ostr << to_json(static_cast<const Task &>(*task));
  }
  ostr << "]";
  return 200;
}

int DispatcherImpl::addTask(std::istream &body, size_t contentLenght, std::ostream &ostr) {
  std::string task_json = readStringFromIstream(body, contentLenght);
  auto task_optional = parse_task_from_json(task_json);
  if (!task_optional.has_value()) return 404;
  auto id = getConnection().create(task_optional.value());
  ostr << "{id:" << id << "}";
  return 200;
}

int DispatcherImpl::deleteTask(int id) {
  bool deleted = getConnection().deleteEntity(typeid(Task), id);
  return deleted ? 200 : 404;
}

int DispatcherImpl::updateTask(int id, std::istream &body, size_t contentLength, std::ostream &ostr) {
  std::string task_json = readStringFromIstream(body, contentLength);
  auto task_optional = parse_task_from_json(task_json);
  if (!task_optional.has_value()) return 404;
  task_optional->id = id;
  auto updated = getConnection().update(task_optional.value());
  return updated ? 200 : 404;
}

int DispatcherImpl::findTasks(std::istream &body, size_t contentLength, std::ostream &ostr) {
  std::string task_json = readStringFromIstream(body, contentLength);
  auto task_optional = parse_task_from_json(task_json);
  if (!task_optional.has_value()) return 404;
  auto tasks = getConnection().findEntities(task_optional.value());
  ostr << "{";
  bool first = true;
  for (auto &task: tasks) {
    if (!first) ostr << ',';
    else first = false;
    ostr << to_json(static_cast<const Task &>(*task));
  }
  ostr << "}";
  return 200;
}

int DispatcherImpl::getSubtasks(std::istream &body, size_t contentLength, std::ostream &ostr) {
  auto tasks = getConnection().getEntities(typeid(Subtask));
  ostr << "[";
  bool first = true;
  for (auto &task: tasks) {
    if (!first) ostr << ',';
    else first = false;
    ostr << to_json(static_cast<const Subtask &>(*task));
  }
  ostr << "]";
  return 200;
}

int DispatcherImpl::addSubtask(std::istream &body, size_t contentLenght, std::ostream &ostr) {
  std::string subtask_json = readStringFromIstream(body, contentLenght);
  auto subtask_optional = parse_subtask_from_json(subtask_json);
  if (!subtask_optional.has_value()) return 404;
  auto id = getConnection().create(subtask_optional.value());
  ostr << "{id:" << id << "}";
  return 200;
}

int DispatcherImpl::deleteSubtask(int id) {
  bool deleted = getConnection().deleteEntity(typeid(Subtask), id);
  return deleted ? 200 : 404;
}

int DispatcherImpl::updateSubtask(int id, std::istream &body, size_t contentLength, std::ostream &ostr) {
  std::string subtask_json = readStringFromIstream(body, contentLength);
  auto subtask_optional = parse_subtask_from_json(subtask_json);
  if (!subtask_optional.has_value()) return 404;
  subtask_optional->id = id;
  auto updated = getConnection().update(subtask_optional.value());
  return updated ? 200 : 404;
}