
#ifndef TASK_REST_MODULES_POSTGRESQL_PGTASK_HPP_
#define TASK_REST_MODULES_POSTGRESQL_PGTASK_HPP_
#include "PGConnection.hpp"

int createTask(PGConnection&, const IEntity&);
int create(PGConnection&, const Task&);

std::list<std::shared_ptr<IEntity>> getTasks(PGConnection &conn, const std::string& query);
std::list<std::shared_ptr<IEntity>> getTasks(PGConnection&);
bool deleteTask(PGConnection&, int id);

bool updateTask(PGConnection&, const IEntity&);
bool update(PGConnection&, const Task&);

std::list<std::shared_ptr<IEntity>> findTasks(PGConnection&, const IEntity&);
std::list<std::shared_ptr<IEntity>> find(PGConnection&, const Task&);

#endif //TASK_REST_MODULES_POSTGRESQL_PGTASK_HPP_
