
#include <gmock/gmock.h>
#include "ICommand.hpp"
#include "PGConnection.hpp"
#include "../../modules/Helpers/Helper.hpp"

using namespace testing;

struct TestPostgrSql : Test {
  TestPostgrSql() :
      connection{"host=localhost dbname=tasks user=task_tracker_user password=password"} {
    connection.truncate();
  }
  PGConnection connection;
};

TEST_F(TestPostgrSql, InsertAndSelect) {
  Task task;
  task.created = std::chrono::system_clock::now();
  task.description = "Task description";
  task.priority = 3;

  auto id = connection.createTask(task);
  ASSERT_THAT(id,Eq(1));
  auto tasks = connection.getTasks();
  ASSERT_THAT(tasks.size(), Eq(1));
  std::string first_task_serialized_created = serializeTimePoint(tasks.front().created.value());
  std::string task_serialized_created = serializeTimePoint(task.created.value());
  ASSERT_THAT(first_task_serialized_created,Eq(task_serialized_created));
  ASSERT_THAT(tasks.front().description,Eq(task.description));
  ASSERT_THAT(tasks.front().priority,Eq(task.priority));
}

TEST_F(TestPostgrSql, Delete) {
  Task task;
  task.created = std::chrono::system_clock::now();
  task.description = "Task description";
  task.priority = 3;

  connection.createTask(task);
  ASSERT_TRUE(connection.deleteTask(1));
  auto tasks = connection.getTasks();
  ASSERT_TRUE(tasks.empty());
}

TEST_F(TestPostgrSql, DISABLED_DeleteWithUnknownId) {
  ASSERT_FALSE(connection.deleteTask(123));
}

TEST_F(TestPostgrSql, Update) {
  Task task;
  task.created = std::chrono::system_clock::now();
  task.description = "Task description";
  task.priority = 3;
  connection.createTask(task);

  task.id = 1;
  task.created.value() -= std::chrono::hours(1);
  task.description = "Updated task description";
  task.priority = 5;
  connection.updateTask(task);

  auto tasks = connection.getTasks();
  ASSERT_THAT(tasks.size(), Eq(1));
  std::string first_task_serialized_created = serializeTimePoint(tasks.front().created.value());
  std::string task_serialized_created = serializeTimePoint(task.created.value());
  ASSERT_THAT(first_task_serialized_created,Eq(task_serialized_created));
  ASSERT_THAT(tasks.front().description,Eq(task.description));
  ASSERT_THAT(tasks.front().priority,Eq(task.priority));
}