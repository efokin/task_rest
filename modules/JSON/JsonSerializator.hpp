
#ifndef TASK_MANAGER_MODULES_JSON_JSONSERIALIZATOR_HPP_
#define TASK_MANAGER_MODULES_JSON_JSONSERIALIZATOR_HPP_
#include "IEntity.hpp"

std::string to_json(const Task& task);
std::optional<Task> parse_task_from_json(const std::string& json_str);

std::string to_json(const Subtask& subtask);
std::optional<Subtask> parse_subtask_from_json(const std::string& json_str);

#endif //TASK_MANAGER_MODULES_JSON_JSONSERIALIZATOR_HPP_
