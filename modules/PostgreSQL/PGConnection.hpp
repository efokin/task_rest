
#ifndef TASK_MANAGER_MODULES_POSTGRESQL_PGCONNECTION_HPP_
#define TASK_MANAGER_MODULES_POSTGRESQL_PGCONNECTION_HPP_

#include "postgresql/libpq-fe.h"
#include <string>
#include <list>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include <memory>
#include "IEntity.hpp"

class PGConnection;
using CreateEntityCall = int(*)(PGConnection&, const IEntity&);
using GetEntityesCall = std::list<std::shared_ptr<IEntity>> (*)(PGConnection&);
using FindEntityesCall = std::list<std::shared_ptr<IEntity>> (*)(PGConnection&, const IEntity&);
using DeleteEntityCall = bool(*)(PGConnection&, int /*id*/);
using UpdateEntityCall = bool(*)(PGConnection&, const IEntity&);

class PGConnection : public IDB {
 public:
  PGConnection(const std::string& connInfo);
  ~PGConnection();
  void chechConnection();
  void truncate();

  int create(const IEntity& entity) override;
  std::list<std::shared_ptr<IEntity>> getEntities(std::type_index) override;
  std::list<std::shared_ptr<IEntity>> findEntities(const IEntity &entity) override;
  bool deleteEntity(std::type_index, int id) override;
  bool update(const IEntity&) override;

  PGconn* operator->() {
    return pgConnPtr;
  }
  operator PGconn* () {
    return pgConnPtr;
  }

 private:
  PGconn* pgConnPtr;
  std::string connetInfo;
  static std::unordered_map<std::type_index, CreateEntityCall> createEntityCalls;
  static std::unordered_map<std::type_index, GetEntityesCall> getEntityCalls;
  static std::unordered_map<std::type_index, FindEntityesCall> findEntityCalls;
  static std::unordered_map<std::type_index, DeleteEntityCall> deleteEntityCalls;
  static std::unordered_map<std::type_index, UpdateEntityCall> updateEntityCalls;

};

class PGResultWrapper {
 public:
  PGResultWrapper(PGresult*);
  ~PGResultWrapper();
  PGresult* operator->() {
    return result;
  }
  operator PGresult* () {
    return result;
  }
 private:
  PGresult* result;
};

#endif //TASK_MANAGER_MODULES_POSTGRESQL_PGCONNECTION_HPP_
