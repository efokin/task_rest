
#include <gmock/gmock.h>
#include "ICommand.hpp"
#include "JsonSerializator.hpp"
#include "Helper.hpp"

using namespace testing;

TEST(TestJSON, SerializeDeserialize) {
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

TEST(TestJSON, DeserializeTask) {
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

TEST(TestJSON, DeserializeWrongId) {
  std::string json_str(R"({"id":"abc"})");
  auto test_optional = parse_task_from_json(json_str);
  ASSERT_FALSE(test_optional.has_value());
}