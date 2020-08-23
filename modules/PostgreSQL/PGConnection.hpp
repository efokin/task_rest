
#ifndef TASK_MANAGER_MODULES_POSTGRESQL_PGCONNECTION_HPP_
#define TASK_MANAGER_MODULES_POSTGRESQL_PGCONNECTION_HPP_

#include "postgresql/libpq-fe.h"
#include <string>
#include <list>
#include <mutex>
#include "ICommand.hpp"

class PGConnection : public IDB {
 public:
  PGConnection(const std::string& connInfo);
  ~PGConnection();
  void truncate();
  int createTask(const Task&) override;
  std::list<Task> getTasks() override;
  bool deleteTask(int id) override;
  bool updateTask(const Task&) override;;

 private:
  PGconn* p_pgConn;
  std::mutex mutex;
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
  bool deteteSubtaskRecord(int id);
  bool deteteTaskRecord(int id);
  void updateTaskPrepareFields(const Task &taks,
                               std::vector<std::string> &fieldNames,
                               std::vector<std::string> &fieldValues) const;
};

#endif //TASK_MANAGER_MODULES_POSTGRESQL_PGCONNECTION_HPP_
