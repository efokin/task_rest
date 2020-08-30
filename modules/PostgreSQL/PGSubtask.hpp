
#ifndef TASK_REST_MODULES_POSTGRESQL_PGSUBTASK_HPP_
#define TASK_REST_MODULES_POSTGRESQL_PGSUBTASK_HPP_
#include "PGConnection.hpp"

int createSubtask(PGConnection&, const IEntity&);
int create(PGConnection&, const Subtask&);

std::list<std::shared_ptr<IEntity>> getSubtasks(PGConnection &conn, const std::string& query);
std::list<std::shared_ptr<IEntity>> getSubtasks(PGConnection&);

bool deleteSubtask(PGConnection&, int id);

bool updateSubtask(PGConnection&, const IEntity&);
bool update(PGConnection&, const Subtask&);

#endif //TASK_REST_MODULES_POSTGRESQL_PGSUBTASK_HPP_
