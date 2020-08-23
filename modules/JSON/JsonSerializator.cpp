
#include "JsonSerializator.hpp"
#include "Poco/JSON/Parser.h"
#include "Helper.hpp"

std::string to_json(const Task& task) {
  std::string json("{");
  if(task.id.has_value()) json += "\"id\":" + std::to_string(task.id.value()) + ",";
  if(task.description.has_value()) json += "\"description\":\"" + task.description.value() + "\",";
  if(task.created.has_value()) json += "\"created\":\"" + serializeTimePoint(task.created.value()) + "\",";
  if(task.priority.has_value()) json += "\"priority\":" + std::to_string(task.priority.value()) + ",";
  if(json.back() == ',')json.erase(--json.end());
  json += "}";
  return json;
}

std::optional<Task> parse_task_from_json(const std::string& json_str) try {
  Task task;
  Poco::JSON::Parser parser;
  auto json = parser.parse(json_str);
  Poco::JSON::Object::Ptr object = json.extract<Poco::JSON::Object::Ptr>();
  if (object->has("id")) {
    task.id = object->getValue<int>("id");
  }
  if (object->has("description")) task.description = object->getValue<std::string>("description");
  if (object->has("created")) {
    task.created = deserializeTimePoint(object->getValue<std::string>("created"));
    if(!task.created.has_value()) return {}; //wrong time point format
  }
  if (object->has("priority")) task.priority = object->getValue<int>("priority");
  return task;
} catch (...) {
  return {};
}