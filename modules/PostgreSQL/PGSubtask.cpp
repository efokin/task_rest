
#include "PGSubtask.hpp"
#include "Helper.hpp"
#include <stdexcept>
#include <sstream>

int createSubtask(PGConnection &conn, const IEntity &entity) {
  return create(conn, static_cast<const Subtask &>(entity));
}

int create(PGConnection &conn, const Subtask &subtask) {
  const char *paramValues[4];
  int paramLengths[4] = {};
  int paramFormats[4] = {};

  std::string created_str;
  created_str = serializeTimePoint(subtask.created.value());
  paramValues[0] = created_str.c_str();
  std::string taskId_str = std::to_string(subtask.taskId.value());
  paramValues[1] = taskId_str.c_str();
  paramValues[2] = subtask.description.value().c_str();

  std::string priority_str = std::to_string(subtask.priority.value());
  paramValues[3] = priority_str.c_str();

  PGResultWrapper result_wrapper(PQexecParams(conn,
                                              "INSERT INTO subtasks (created,task_id,description,priority) "
                                              "VALUES ($1,$2,$3,$4) RETURNING id;",
                                              4,       /* one param */
                                              NULL,    /* let the backend deduce param type */
                                              paramValues,
                                              paramLengths,
                                              paramFormats,
                                              0));      /* ask for binary results */

  if (PQresultStatus(result_wrapper) != PGRES_TUPLES_OK) {
    fprintf(stderr, "%s", PQerrorMessage(conn));
    throw std::runtime_error("Cannot insert record into tasks table");
  }
  if (PQntuples(result_wrapper) != 1) throw std::runtime_error("Insert record into tasks table does not return id");
  auto id_ptr = PQgetvalue(result_wrapper, 0, 0);
  int result;
  sscanf(id_ptr, "%d", &result);
  return result;
}

std::list<std::shared_ptr<IEntity>> getSubtasks(PGConnection &conn) {
  return getSubtasks(conn, "SELECT id,task_id,created,description,priority FROM subtasks;");
}

std::list<std::shared_ptr<IEntity>> getSubtasks(PGConnection &conn, const std::string &query) {
  std::list<std::shared_ptr<IEntity>> result;
  PGResultWrapper result_wrapper(PQexec(conn, query.c_str()));

  if (PQresultStatus(result_wrapper) != PGRES_TUPLES_OK) {
    fprintf(stderr, "%s", PQerrorMessage(conn));
    throw std::runtime_error("Cannot insert record into tasks table");
  }

  int id_fnum,
      task_id_fnum,
      created_fnum,
      description_fnum,
      priority_fnum;

  /* Use PQfnumber to avoid assumptions about field order in result */
  id_fnum = PQfnumber(result_wrapper, "id");
  task_id_fnum = PQfnumber(result_wrapper, "task_id");
  created_fnum = PQfnumber(result_wrapper, "created");
  description_fnum = PQfnumber(result_wrapper, "description");
  priority_fnum = PQfnumber(result_wrapper, "priority");

  for (int i = 0; i < PQntuples(result_wrapper); i++) {
    Subtask subtask;

    /* Get the field values (we ignore possibility they are null!) */
    auto id_ptr = PQgetvalue(result_wrapper, i, id_fnum);
    auto task_id_ptr = PQgetvalue(result_wrapper, i, task_id_fnum);
    auto created_ptr = PQgetvalue(result_wrapper, i, created_fnum);
    auto description_ptr = PQgetvalue(result_wrapper, i, description_fnum);
    auto priority_ptr = PQgetvalue(result_wrapper, i, priority_fnum);

    subtask.id = 0;
    sscanf(id_ptr, "%d", &subtask.id.value());

    subtask.taskId = 0;
    sscanf(task_id_ptr, "%d", &subtask.taskId.value());

    subtask.priority = 0;
    sscanf(priority_ptr, "%d", &subtask.priority.value());

    subtask.created = deserializeTimePoint(created_ptr);

    subtask.description = std::string(description_ptr);
    result.push_back(std::make_shared<Subtask>(subtask));
  }
  return result;
}

bool deteteSubtaskRecord(PGConnection &conn, int id);

bool deleteSubtask(PGConnection &conn, int id) {
  return deteteSubtaskRecord(conn, id);
}

bool deteteSubtaskRecord(PGConnection &conn, int id) {
  const char *paramValues[1];
  int paramLengths[1] = {};
  int paramFormats[1] = {};

  std::string subtask_id = std::to_string(id);
  paramValues[0] = subtask_id.c_str();

  PGResultWrapper result_wrapper(PQexecParams(conn,
                                              "DELETE FROM subtasks WHERE id = $1;",
                                              1,       /* one param */
                                              NULL,    /* let the backend deduce param type */
                                              paramValues,
                                              paramLengths,
                                              paramFormats,
                                              0));      /* ask for binary results */

  if (PQresultStatus(result_wrapper) != PGRES_COMMAND_OK) {
    fprintf(stderr, "%s", PQerrorMessage(conn));
    throw std::runtime_error("Error during deleting a record from tasks table");
  }
  return true;
}

void updateSubtaskPrepareFields(const Subtask &subtask,
                             std::vector<std::string> &fieldNames,
                             std::vector<std::string> &fieldValues);

bool update(PGConnection &conn, const Subtask &subtaks) {
  std::vector<std::string> fieldNames;
  std::vector<std::string> fieldValues;
  updateSubtaskPrepareFields(subtaks, fieldNames, fieldValues);
  const char *paramValues[5];
  int paramLengths[5] = {};
  int paramFormats[5] = {};

  std::string id_str = std::to_string(subtaks.id.value());
  paramValues[0] = id_str.c_str();
  int index = 1;
  for (auto &value : fieldValues) {
    paramValues[index++] = value.c_str();
  }

  index = 2; // $1 reserved for id
  auto fieldNameIt = fieldNames.begin();
  std::string nameValueText = *fieldNameIt;
  nameValueText += " = $" + std::to_string(index);
  while (++fieldNameIt != fieldNames.end()) {
    nameValueText += ",";
    nameValueText += *fieldNameIt + " = $" + std::to_string(++index);
  }

  std::string updateQuery("UPDATE subtasks SET ");
  updateQuery += nameValueText;
  updateQuery += " WHERE id = $1;";
  PGResultWrapper result_wrapper(PQexecParams(conn,
                                              updateQuery.c_str(),
                                              fieldNames.size() + 1, //plus one for id
                                              NULL,    /* let the backend deduce param type */
                                              paramValues,
                                              paramLengths,
                                              paramFormats,
                                              0));      /* ask for binary results */

  if (PQresultStatus(result_wrapper) != PGRES_COMMAND_OK) {
    fprintf(stderr, "%s", PQerrorMessage(conn));
    throw std::runtime_error("Cannot update record into tasks table");
  }
  return true;
}

bool updateSubtask(PGConnection &conn, const IEntity &entity) {
  return update(conn, static_cast<const Subtask &>(entity));
}

void updateSubtaskPrepareFields(const Subtask &subtask,
                             std::vector<std::string> &fieldNames,
                             std::vector<std::string> &fieldValues) {
  if (subtask.taskId.has_value()) {
    fieldNames.push_back("task_id");
    fieldValues.push_back(std::to_string(subtask.taskId.value()));
  }
  if (subtask.description.has_value()) {
    fieldNames.push_back("description");
    fieldValues.push_back(subtask.description.value());
  }
  if (subtask.created.has_value()) {
    fieldNames.push_back("created");
    fieldValues.push_back(serializeTimePoint(subtask.created.value()));
  }
  if (subtask.priority.has_value()) {
    fieldNames.push_back("priority");
    fieldValues.push_back(std::to_string(subtask.priority.value()));
  }
}