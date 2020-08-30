
#include <gmock/gmock.h>
#include "IEntity.hpp"
#include "JsonSerializator.hpp"
#include "Helper.hpp"

using namespace testing;

TEST(TestTaskJSON, SerializeDeserialize) {
  Task task;
  task.id = 123;
  task.created = deserializeTimePoint("2020-01-01 12:00:00");
  task.description = "Description";
  task.priority = 3;

  std::string json_str = to_json(task);
  auto deserialized_option = parse_task_from_json(json_str);
  ASSERT_TRUE(deserialized_option.has_value());
  ASSERT_THAT(deserialized_option->id, Optional(123));
  ASSERT_THAT(deserialized_option->description, Optional(std::string("Description")));
  ASSERT_THAT(serializeTimePoint(deserialized_option->created.value()), Eq("2020-01-01 12:00:00"));
  ASSERT_THAT(deserialized_option->priority, Optional(3));
}

TEST(TestTaskJSON, SerializeDeserializeRussian) {
  Task task;
  task.id = 123;
  task.created = deserializeTimePoint("2020-01-01 12:00:00");
  task.description = "Новая задача";
  task.priority = 3;

  std::string json_str = to_json(task);
  auto deserialized_option = parse_task_from_json(json_str);
  ASSERT_TRUE(deserialized_option.has_value());
  ASSERT_THAT(deserialized_option->id, Optional(123));
  ASSERT_THAT(deserialized_option->description, Optional(std::string("Новая задача")));
  ASSERT_THAT(serializeTimePoint(deserialized_option->created.value()), Eq("2020-01-01 12:00:00"));
  ASSERT_THAT(deserialized_option->priority, Optional(3));
}

TEST(TestTaskJSON, DeserializeTask) {
  std::string json_str(R"({
    "id":123,
    "description":"Description",
    "created":"2020-01-01 12:00:00",
    "priority":3
  })");
  auto test_optional = parse_task_from_json(json_str);
  ASSERT_TRUE(test_optional.has_value());
  ASSERT_THAT(test_optional->id, Optional(123));
  ASSERT_THAT(test_optional->description, Optional(std::string("Description")));
  ASSERT_THAT(serializeTimePoint(test_optional->created.value()), Eq("2020-01-01 12:00:00"));
  ASSERT_THAT(test_optional->priority, Optional(3));
}

TEST(TestTaskJSON, DeserializeWrongId) {
  std::string json_str(R"({"id":"abc"})");
  auto test_optional = parse_task_from_json(json_str);
  ASSERT_FALSE(test_optional.has_value());
}

TEST(TestSubtaskJSON, SerializeDeserialize) {
  Subtask subtask;
  subtask.id = 123;
  subtask.taskId = 32;
  subtask.created = deserializeTimePoint("2020-01-01 12:00:00");
  subtask.description = "Description";
  subtask.priority = 3;

  std::string json_str = to_json(subtask);
  auto deserialized_option = parse_subtask_from_json(json_str);
  ASSERT_TRUE(deserialized_option.has_value());
  ASSERT_THAT(deserialized_option->id, Optional(123));
  ASSERT_THAT(deserialized_option->taskId, Optional(32));
  ASSERT_THAT(deserialized_option->description, Optional(std::string("Description")));
  ASSERT_THAT(serializeTimePoint(deserialized_option->created.value()), Eq("2020-01-01 12:00:00"));
  ASSERT_THAT(deserialized_option->priority, Optional(3));
}