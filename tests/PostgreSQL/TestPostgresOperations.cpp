
#include <typeindex>
#include <gmock/gmock.h>
#include "IEntity.hpp"
#include "PGConnection.hpp"
#include "Helper.hpp"

using namespace testing;

struct TestPostgrSql : Test {
  TestPostgrSql() :
      connection{"host=localhost dbname=tasks user=task_tracker_user password=password"} {
    connection.truncate();
  }
  PGConnection connection;
};

TEST_F(TestPostgrSql, TaskInsertAndSelect) {
  Task task;
  task.created = std::chrono::system_clock::now();
  task.description = "Task description";
  task.priority = 3;

  auto id = connection.create(task);
  ASSERT_THAT(id,Eq(1));
  auto task_ptrs = connection.getEntities(std::type_index(typeid(Task)));
  ASSERT_THAT(task_ptrs.size(), Eq(1));
  auto const& first_task = static_cast<const Task&>(*task_ptrs.front());
  std::string first_task_serialized_created = serializeTimePoint(first_task.created.value());
  std::string task_serialized_created = serializeTimePoint(task.created.value());
  ASSERT_THAT(first_task_serialized_created,Eq(task_serialized_created));
  ASSERT_THAT(first_task.description, Eq(task.description));
  ASSERT_THAT(first_task.priority, Eq(task.priority));
}

TEST_F(TestPostgrSql, InsertAndFindDesription) {
  Task task;
  task.created = std::chrono::system_clock::now();
  task.description = "Task description";
  task.priority = 3;

  auto id = connection.create(task);
  ASSERT_THAT(id,Eq(1));

  Task findTask;
  findTask.description = "Task description";

  auto task_ptrs = connection.findEntities(findTask);
  ASSERT_THAT(task_ptrs.size(), Eq(1));
  auto const& first_task = static_cast<const Task&>(*task_ptrs.front());
  std::string first_task_serialized_created = serializeTimePoint(first_task.created.value());
  std::string task_serialized_created = serializeTimePoint(task.created.value());
  ASSERT_THAT(first_task_serialized_created,Eq(task_serialized_created));
  ASSERT_THAT(first_task.description, Eq(task.description));
  ASSERT_THAT(first_task.priority, Eq(task.priority));
}

TEST_F(TestPostgrSql, InsertAndFindCreated) {
  Task task;
  task.created = std::chrono::system_clock::now();
  task.description = "Task description";
  task.priority = 3;

  auto id = connection.create(task);
  ASSERT_THAT(id,Eq(1));

  Task findTask;
  findTask.created = task.created;

  auto task_ptrs = connection.findEntities(findTask);
  ASSERT_THAT(task_ptrs.size(), Eq(1));
  auto const& first_task = static_cast<const Task&>(*task_ptrs.front());
  std::string first_task_serialized_created = serializeTimePoint(first_task.created.value());
  std::string task_serialized_created = serializeTimePoint(task.created.value());
  ASSERT_THAT(first_task_serialized_created,Eq(task_serialized_created));
  ASSERT_THAT(first_task.description, Eq(task.description));
  ASSERT_THAT(first_task.priority, Eq(task.priority));
}

TEST_F(TestPostgrSql, InsertAndFindAllFields) {
  Task task;
  task.created = std::chrono::system_clock::now();
  task.description = "Task description";
  task.priority = 3;

  auto id = connection.create(task);
  ASSERT_THAT(id,Eq(1));

  auto task_ptrs = connection.findEntities(task);
  ASSERT_THAT(task_ptrs.size(), Eq(1));
  auto const& first_task = static_cast<const Task&>(*task_ptrs.front());
  std::string first_task_serialized_created = serializeTimePoint(first_task.created.value());
  std::string task_serialized_created = serializeTimePoint(task.created.value());
  ASSERT_THAT(first_task_serialized_created,Eq(task_serialized_created));
  ASSERT_THAT(first_task.description, Eq(task.description));
  ASSERT_THAT(first_task.priority, Eq(task.priority));
}

TEST_F(TestPostgrSql, DeleteTask) {
  Task task;
  task.created = std::chrono::system_clock::now();
  task.description = "Task description";
  task.priority = 3;

  connection.create(task);
  ASSERT_TRUE(connection.deleteEntity(typeid(Task),1));
  auto tasks = connection.getEntities(std::type_index(typeid(Task)));;
  ASSERT_TRUE(tasks.empty());
}

TEST_F(TestPostgrSql, DISABLED_DeleteWithUnknownId) {
  ASSERT_FALSE(connection.deleteEntity(typeid(Task),123));
}

TEST_F(TestPostgrSql, UpdateTask) {
  Task task;
  task.created = std::chrono::system_clock::now();
  task.description = "Task description";
  task.priority = 3;
  connection.create(task);

  task.id = 1;
  task.created.value() -= std::chrono::hours(1);
  task.description = "Updated task description";
  task.priority = 5;
  connection.update(task);

  auto task_ptrs = connection.getEntities(std::type_index(typeid(Task)));
  ASSERT_THAT(task_ptrs.size(), Eq(1));
  auto const& first_task = static_cast<const Task&>(*task_ptrs.front());
  std::string first_task_serialized_created = serializeTimePoint(first_task.created.value());
  std::string task_serialized_created = serializeTimePoint(task.created.value());
  ASSERT_THAT(first_task_serialized_created,Eq(task_serialized_created));
  ASSERT_THAT(first_task.description,Eq(task.description));
  ASSERT_THAT(first_task.priority,Eq(task.priority));
}

TEST_F(TestPostgrSql, SubtaskInsertAndSelect) {
  Task task;
  task.created = std::chrono::system_clock::now();
  task.description = "Task description";
  task.priority = 3;

  auto taskId = connection.create(task);

  Subtask subtask;
  subtask.created = std::chrono::system_clock::now();
  subtask.taskId = taskId;
  subtask.description = "Subtask description";
  subtask.priority = 2;

  auto subtaskId = connection.create(subtask);

  auto task_ptrs = connection.getEntities(std::type_index(typeid(Subtask)));
  ASSERT_THAT(task_ptrs.size(), Eq(1));
  auto const& first_subtask = static_cast<const Subtask&>(*task_ptrs.front());
  ASSERT_THAT(first_subtask.taskId, Optional(taskId));
  std::string first_task_serialized_created = serializeTimePoint(first_subtask.created.value());
  std::string task_serialized_created = serializeTimePoint(subtask.created.value());
  ASSERT_THAT(first_task_serialized_created,Eq(task_serialized_created));
  ASSERT_THAT(first_subtask.description, Eq(subtask.description));
  ASSERT_THAT(first_subtask.priority, Eq(subtask.priority));
}

TEST_F(TestPostgrSql, SubtaskInsertAndDelete) {
  Task task;
  task.created = std::chrono::system_clock::now();
  task.description = "Task description";
  task.priority = 3;

  auto taskId = connection.create(task);

  Subtask subtask;
  subtask.created = std::chrono::system_clock::now();
  subtask.taskId = taskId;
  subtask.description = "Subtask description";
  subtask.priority = 2;

  auto subtaskId = connection.create(subtask);

  connection.deleteEntity(typeid(Subtask),subtaskId);

  auto task_ptrs = connection.getEntities(std::type_index(typeid(Subtask)));
  ASSERT_THAT(task_ptrs.size(), Eq(0));
}

TEST_F(TestPostgrSql, SubtaskInsertAndDeleteTask) {
  Task task;
  task.created = std::chrono::system_clock::now();
  task.description = "Task description";
  task.priority = 3;

  auto taskId = connection.create(task);

  Subtask subtask;
  subtask.created = std::chrono::system_clock::now();
  subtask.taskId = taskId;
  subtask.description = "Subtask description";
  subtask.priority = 2;

  auto subtaskId = connection.create(subtask);

  connection.deleteEntity(typeid(Task),taskId);

  auto task_ptrs = connection.getEntities(std::type_index(typeid(Subtask)));
  ASSERT_THAT(task_ptrs.size(), Eq(0));
}

TEST_F(TestPostgrSql, SubtaskInsertAndUpdate) {
  Task task;
  task.created = std::chrono::system_clock::now();
  task.description = "Task description";
  task.priority = 3;

  auto taskId1 = connection.create(task);
  auto taskId2 = connection.create(task);

  Subtask subtask;
  subtask.created = std::chrono::system_clock::now();
  subtask.taskId = taskId1;
  subtask.description = "Subtask description";
  subtask.priority = 2;

  auto subtaskId = connection.create(subtask);

  subtask.priority = 1;
  subtask.description = "Updated description";
  subtask.id = subtaskId;
  subtask.taskId = taskId2;
  connection.update(subtask);

  auto task_ptrs = connection.getEntities(std::type_index(typeid(Subtask)));
  ASSERT_THAT(task_ptrs.size(), Eq(1));
  auto const& first_subtask = static_cast<const Subtask&>(*task_ptrs.front());
  ASSERT_THAT(first_subtask.taskId, Optional(taskId2));
  std::string first_task_serialized_created = serializeTimePoint(first_subtask.created.value());
  std::string task_serialized_created = serializeTimePoint(subtask.created.value());
  ASSERT_THAT(first_task_serialized_created,Eq(task_serialized_created));
  ASSERT_THAT(first_subtask.description, Eq(subtask.description));
  ASSERT_THAT(first_subtask.priority, Eq(subtask.priority));
}
