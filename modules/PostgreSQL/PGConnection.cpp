
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include <typeindex>
#include <netinet/in.h>
#include "PGConnection.hpp"
#include "PGTask.hpp"
#include "PGSubtask.hpp"
#include "../Helpers/Helper.hpp"

PGConnection::PGConnection(const std::string &_connInfo) :
pgConnPtr{}, connetInfo{_connInfo} {
  pgConnPtr = PQconnectdb(connetInfo.c_str());
  if (PQstatus(pgConnPtr) != CONNECTION_OK) {
    throw std::runtime_error("Cannot connect to DB");
  }
}

std::unordered_map<std::type_index, CreateEntityCall> initializeCreateEntityCalls() {
  std::unordered_map<std::type_index, CreateEntityCall> result;
  result[std::type_index(typeid(Task))] = &createTask;
  result[std::type_index(typeid(Subtask))] = &createSubtask;
  return result;
}
std::unordered_map<std::type_index, CreateEntityCall> PGConnection::createEntityCalls = initializeCreateEntityCalls();

std::unordered_map<std::type_index, GetEntityesCall> initializeGetEntityCalls() {
  std::unordered_map<std::type_index, GetEntityesCall> result;
  result[std::type_index(typeid(Task))] = &getTasks;
  result[std::type_index(typeid(Subtask))] = &getSubtasks;
  return result;
}

std::unordered_map<std::type_index, GetEntityesCall> PGConnection::getEntityCalls = initializeGetEntityCalls();

std::unordered_map<std::type_index, FindEntityesCall> initializeFindEntityCalls() {
  std::unordered_map<std::type_index, FindEntityesCall> result;
  result[std::type_index(typeid(Task))] = &findTasks;
  return result;
}

std::unordered_map<std::type_index, FindEntityesCall> PGConnection::findEntityCalls = initializeFindEntityCalls();

std::unordered_map<std::type_index, DeleteEntityCall> initializeDeleteEntityCalls() {
  std::unordered_map<std::type_index, DeleteEntityCall> result;
  result[std::type_index(typeid(Task))] = &deleteTask;
  result[std::type_index(typeid(Subtask))] = &deleteSubtask;
  return result;
}

std::unordered_map<std::type_index, DeleteEntityCall> PGConnection::deleteEntityCalls = initializeDeleteEntityCalls();

std::unordered_map<std::type_index, UpdateEntityCall> initializeUpdateEntityCalls() {
  std::unordered_map<std::type_index, UpdateEntityCall> result;
  result[std::type_index(typeid(Task))] = &updateTask;
  result[std::type_index(typeid(Subtask))] = &updateSubtask;
  return result;
}
std::unordered_map<std::type_index, UpdateEntityCall> PGConnection::updateEntityCalls = initializeUpdateEntityCalls();

PGConnection::~PGConnection() {
  PQfinish(pgConnPtr);
}

std::list<std::shared_ptr<IEntity>> PGConnection::getEntities(std::type_index type) {
  if (auto it = getEntityCalls.find(type); it != getEntityCalls.end())
    return it->second(*this);

  throw std::runtime_error("Unknown DB entity type");
}

void PGConnection::truncate() {
  PGResultWrapper result_wrapper(PQexec(pgConnPtr,
                                        "TRUNCATE TABLE tasks RESTART IDENTITY CASCADE;"
  ));

  if (PQresultStatus(result_wrapper) != PGRES_COMMAND_OK) {
    fprintf(stderr, "%s", PQerrorMessage(pgConnPtr));
    throw std::runtime_error("Cannot truncate tasks table");
  }
}

bool PGConnection::deleteEntity(std::type_index type, int id) {
  if (auto it = deleteEntityCalls.find(type); it != deleteEntityCalls.end())
    return it->second(*this, id);

  throw std::runtime_error("Unknown DB entity type");
}


bool PGConnection::update(const IEntity &entity) {
  if (auto it = updateEntityCalls.find(std::type_index(typeid(entity))); it != updateEntityCalls.end())
    return it->second(*this, entity);

  throw std::runtime_error("Unknown DB entity type");
}

int PGConnection::create(const IEntity &entity) {
  if (auto it = createEntityCalls.find(std::type_index(typeid(entity))); it != createEntityCalls.end())
    return it->second(*this, entity);

  throw std::runtime_error("Unknown DB entity type");
}
std::list<std::shared_ptr<IEntity>> PGConnection::findEntities(const IEntity &entity) {
  if (auto it = findEntityCalls.find(std::type_index(typeid(entity))); it != findEntityCalls.end())
    return it->second(*this, entity);

  throw std::runtime_error("Unknown DB entity type");
}
void PGConnection::chechConnection() {
  if(PQstatus(pgConnPtr) != CONNECTION_OK) PQfinish(pgConnPtr);
  pgConnPtr = PQconnectdb(connetInfo.c_str());
  if (PQstatus(pgConnPtr) != CONNECTION_OK) {
    throw std::runtime_error("Cannot connect to DB");
  }
}

PGResultWrapper::PGResultWrapper(PGresult *the_result) :
    result{the_result} {
}
PGResultWrapper::~PGResultWrapper() {
  PQclear(result);
}
