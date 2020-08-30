
#include "PGTask.hpp"
#include "Helper.hpp"
#include <stdexcept>
#include <sstream>

int create(PGConnection &conn, const Task &task) {
  const char *paramValues[3];
  int paramLengths[3] = {};
  int paramFormats[3] = {};

  std::string created_str;
  created_str = serializeTimePoint(task.created.value());
  paramValues[0] = created_str.c_str();
  paramValues[1] = task.description.value().c_str();

  std::string priority_str = std::to_string(task.priority.value());
  paramValues[2] = priority_str.c_str();

  PGResultWrapper result_wrapper(PQexecParams(conn,
                                              "INSERT INTO tasks (created,description,priority) "
                                              "VALUES ($1,$2,$3) RETURNING id;",
                                              3,       /* one param */
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
int createTask(PGConnection &conn, const IEntity &entity) {
  return create(conn, static_cast<const Task &>(entity));
}

std::list<std::shared_ptr<IEntity>> getTasks(PGConnection &conn) {
  return getTasks(conn, "SELECT id,created,description,priority FROM tasks;");
}

std::list<std::shared_ptr<IEntity>> getTasks(PGConnection &conn, const std::string& query) {
  std::list<std::shared_ptr<IEntity>> result;
  PGResultWrapper result_wrapper(PQexec(conn, query.c_str()));

  if (PQresultStatus(result_wrapper) != PGRES_TUPLES_OK) {
    fprintf(stderr, "%s", PQerrorMessage(conn));
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
    result.push_back(std::make_shared<Task>(task));
  }
  return result;
}

bool deteteTaskRecord(PGConnection &conn, int id);

bool deleteTask(PGConnection &conn, int id) {
  bool result;
  // delete subtasks if required
  result = deteteTaskRecord(conn, id);
  return result;
}

bool deteteTaskRecord(PGConnection &conn, int id) {
  const char *paramValues[1];
  int paramLengths[1] = {};
  int paramFormats[1] = {};

  std::string priority_str = std::to_string(id);
  paramValues[0] = priority_str.c_str();

  PGResultWrapper result_wrapper(PQexecParams(conn,
                                              "DELETE FROM tasks WHERE id = $1;",
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

void updateTaskPrepareFields(const Task &task,
                             std::vector<std::string> &fieldNames,
                             std::vector<std::string> &fieldValues);

bool update(PGConnection &conn, const Task &taks) {
  std::vector<std::string> fieldNames;
  std::vector<std::string> fieldValues;
  updateTaskPrepareFields(taks, fieldNames, fieldValues);
  const char *paramValues[4];
  int paramLengths[4] = {};
  int paramFormats[4] = {};

  std::string id_str = std::to_string(taks.id.value());
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

  std::string updateQuery("UPDATE tasks SET ");
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
  return false;
}

bool updateTask(PGConnection &conn, const IEntity &entity) {
  return update(conn, static_cast<const Task &>(entity));
}
std::list<std::shared_ptr<IEntity>> findTasks(PGConnection &conn, const IEntity &task) {
  return find(conn, static_cast<const Task &>(task));
}

std::list<std::shared_ptr<IEntity>> find(PGConnection &conn, const Task &task) {
  std::vector<std::string> fieldNames;
  std::vector<std::string> fieldValues;
  const char *paramValues[4];
  int paramLengths[4] = {};
  int paramFormats[4] = {};
  int fieldIndex{};

  std::stringstream ss;
  ss << "SELECT * FROM tasks WHERE ";
  std::string andStr;
  if (task.description.has_value()) {
    paramValues[fieldIndex] = task.description.value().c_str();
    ss << andStr;
    ss << "description = $" << ++fieldIndex;
    andStr = " AND ";
  }
  std::string created_str;
  if (task.created.has_value()) {
    created_str = serializeTimePoint(task.created.value());
    paramValues[fieldIndex] = created_str.c_str();
    ss << andStr;
    ss << "created = $" << ++fieldIndex;
    andStr = " AND ";
  }
  std::string priority_str;
  if (task.priority.has_value()) {
    priority_str = std::to_string(task.priority.value());
    paramValues[fieldIndex] = priority_str.c_str();
    ss << andStr;
    ss << "priority = $" << ++fieldIndex;
    andStr = " AND ";
  }

  std::list<std::shared_ptr<IEntity>> result;

  std::string query = ss.str();
  PGResultWrapper result_wrapper(PQexecParams(conn,
                                              query.c_str(),
                                              fieldIndex, //plus one for id
                                              NULL,    /* let the backend deduce param type */
                                              paramValues,
                                              paramLengths,
                                              paramFormats,
                                              0));      /* ask for binary results */

  if (PQresultStatus(result_wrapper) != PGRES_TUPLES_OK) {
    fprintf(stderr, "%s", PQerrorMessage(conn));
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
    result.push_back(std::make_shared<Task>(task));
  }
  return result;
}

void updateTaskPrepareFields(const Task &task,
                             std::vector<std::string> &fieldNames,
                             std::vector<std::string> &fieldValues) {
  if (task.description.has_value()) {
    fieldNames.push_back("description");
    fieldValues.push_back(task.description.value());
  }
  if (task.created.has_value()) {
    fieldNames.push_back("created");
    fieldValues.push_back(serializeTimePoint(task.created.value()));
  }
  if (task.priority.has_value()) {
    fieldNames.push_back("priority");
    fieldValues.push_back(std::to_string(task.priority.value()));
  }
}