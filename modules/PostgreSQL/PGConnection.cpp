
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include <netinet/in.h>
#include "PGConnection.hpp"
#include "../Helpers/Helper.hpp"

PGConnection::PGConnection(const std::string &connInfo) {
  p_pgConn = PQconnectdb(connInfo.c_str());
  if (PQstatus(p_pgConn) != CONNECTION_OK) {
    throw std::runtime_error("Cannot connect to DB");
  }
}

PGConnection::~PGConnection() {
  PQfinish(p_pgConn);
}
int PGConnection::createTask(const Task & task) {
  std::lock_guard<std::mutex> lock(mutex);
  const char *paramValues[3];
  int paramLengths[3] = {};
  int paramFormats[3] = {};
  uint32_t binaryIntVal;

  std::string created_str;
  created_str = serializeTimePoint(task.created.value());
  paramValues[0] = created_str.c_str();
  paramValues[1] = task.description.value().c_str();

  std::string priority_str = std::to_string(task.priority.value());
  paramValues[2] = priority_str.c_str();

  PGResultWrapper result_wrapper(PQexecParams(p_pgConn,
                                              "INSERT INTO tasks (created,description,priority) "
                                              "VALUES ($1,$2,$3) RETURNING id;",
                                              3,       /* one param */
                                              NULL,    /* let the backend deduce param type */
                                              paramValues,
                                              paramLengths,
                                              paramFormats,
                                              0));      /* ask for binary results */

  if (PQresultStatus(result_wrapper) != PGRES_TUPLES_OK) {
    fprintf(stderr, "%s", PQerrorMessage(p_pgConn));
    throw std::runtime_error("Cannot insert record into tasks table");
  }
  if(PQntuples(result_wrapper)!=1) throw std::runtime_error("Insert record into tasks table does not return id");
  auto id_ptr = PQgetvalue(result_wrapper, 0, 0);
  int result;
  sscanf(id_ptr, "%d", &result);
  return result;
}

std::list<Task> PGConnection::getTasks() {
  std::lock_guard<std::mutex> lock(mutex);
  std::list<Task> result;
  PGResultWrapper result_wrapper(PQexec(p_pgConn, "SELECT id,created,description,priority FROM tasks;"));

  if (PQresultStatus(result_wrapper) != PGRES_TUPLES_OK) {
    fprintf(stderr, "%s", PQerrorMessage(p_pgConn));
    throw std::runtime_error("Cannot insert record into tasks table");
  }

  int id_fnum,
      created_fnum,
      description_fnum,
      priority_fnum;

  /* Use PQfnumber to avoid assumptions about field order in result */
  id_fnum = PQfnumber(result_wrapper, "id");
  created_fnum = PQfnumber(result_wrapper, "created");
  description_fnum = PQfnumber(result_wrapper, "description");
  priority_fnum = PQfnumber(result_wrapper, "priority");

  for (int i = 0; i < PQntuples(result_wrapper); i++) {
    Task task;

    /* Get the field values (we ignore possibility they are null!) */
    auto id_ptr = PQgetvalue(result_wrapper, i, id_fnum);
    auto created_ptr = PQgetvalue(result_wrapper, i, created_fnum);
    auto description_ptr = PQgetvalue(result_wrapper, i, description_fnum);
    auto priority_ptr = PQgetvalue(result_wrapper, i, priority_fnum);

    task.id = 0;
    sscanf(id_ptr, "%d", &task.id.value());

    task.priority = 0;
    sscanf(priority_ptr, "%d", &task.priority.value());

    task.created = deserializeTimePoint(created_ptr);

    task.description = std::string(description_ptr);
    result.push_back(task);
  }
  return result;
}
void PGConnection::truncate() {
  std::lock_guard<std::mutex> lock(mutex);
  PGResultWrapper result_wrapper(PQexec(p_pgConn,
                                              "TRUNCATE TABLE tasks RESTART IDENTITY CASCADE;"
                                              ));      /* ask for binary results */

  if (PQresultStatus(result_wrapper) != PGRES_COMMAND_OK) {
    fprintf(stderr, "%s", PQerrorMessage(p_pgConn));
    throw std::runtime_error("Cannot truncate tasks table");
  }
}
bool PGConnection::deleteTask(int id) {
  std::lock_guard<std::mutex> lock(mutex);
  bool result;
  result = deteteSubtaskRecord(id);
  result &= deteteTaskRecord(id);
  return result;
}
bool PGConnection::deteteSubtaskRecord(int id) {
  const char *paramValues[1];
  int paramLengths[1] = {};
  int paramFormats[1] = {};

  std::string priority_str = std::to_string(id);
  paramValues[0] = priority_str.c_str();

  PGResultWrapper result_wrapper(PQexecParams(p_pgConn,
                                              "DELETE FROM subtasks WHERE task_id = $1;",
                                              1,       /* one param */
                                              NULL,    /* let the backend deduce param type */
                                              paramValues,
                                              paramLengths,
                                              paramFormats,
                                              0));      /* ask for binary results */

  if (PQresultStatus(result_wrapper) != PGRES_COMMAND_OK) {
    fprintf(stderr, "%s", PQerrorMessage(p_pgConn));
    throw std::runtime_error("Error during deleting a record from tasks table");
  }
  return true;
}

bool PGConnection::deteteTaskRecord(int id) {
  const char *paramValues[1];
  int paramLengths[1] = {};
  int paramFormats[1] = {};

  std::string priority_str = std::to_string(id);
  paramValues[0] = priority_str.c_str();

  PGResultWrapper result_wrapper(PQexecParams(p_pgConn,
                                              //"DELETE FROM subtask WHERE task_id = $1;"
                                              "DELETE FROM tasks WHERE id = $1;",
                                              1,       /* one param */
                                              NULL,    /* let the backend deduce param type */
                                              paramValues,
                                              paramLengths,
                                              paramFormats,
                                              0));      /* ask for binary results */

  if (PQresultStatus(result_wrapper) != PGRES_COMMAND_OK) {
    fprintf(stderr, "%s", PQerrorMessage(p_pgConn));
    throw std::runtime_error("Error during deleting a record from tasks table");
  }
  return true;
}

bool PGConnection::updateTask(const Task & taks) {
  std::lock_guard<std::mutex> lock(mutex);
  std::vector<std::string> fieldNames;
  std::vector<std::string> fieldValues;
  updateTaskPrepareFields(taks, fieldNames, fieldValues);
  const char *paramValues[4];
  int paramLengths[4] = {};
  int paramFormats[4] = {};

  std::string id_str = std::to_string(taks.id.value());
  paramValues[0] = id_str.c_str();
  int index = 1;
  for(auto& value : fieldValues) {
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

  std::string updateQuery("UPDATE tasks SET ");
  updateQuery += nameValueText;
  updateQuery += " WHERE id = $1;";
  PGResultWrapper result_wrapper(PQexecParams(p_pgConn,
                                              updateQuery.c_str(),
                                              fieldNames.size()+1, //plus one for id
                                              NULL,    /* let the backend deduce param type */
                                              paramValues,
                                              paramLengths,
                                              paramFormats,
                                              0));      /* ask for binary results */

  if (PQresultStatus(result_wrapper) != PGRES_COMMAND_OK) {
    fprintf(stderr, "%s", PQerrorMessage(p_pgConn));
    throw std::runtime_error("Cannot update record into tasks table");
  }
  return false;
}
void PGConnection::updateTaskPrepareFields(const Task &taks,
                                           std::vector<std::string> &fieldNames,
                                           std::vector<std::string> &fieldValues) const {
  if(taks.description.has_value()) {
    fieldNames.push_back("description");
    fieldValues.push_back(taks.description.value());
  }
  if(taks.created.has_value()) {
    fieldNames.push_back("created");
    fieldValues.push_back(serializeTimePoint(taks.created.value()));
  }
  if(taks.priority.has_value()) {
    fieldNames.push_back("priority");
    fieldValues.push_back(std::to_string(taks.priority.value()));
  }
}

PGConnection::PGResultWrapper::PGResultWrapper(PGresult *the_result) :
    result{the_result} {
}
PGConnection::PGResultWrapper::~PGResultWrapper() {
  PQclear(result);
}
