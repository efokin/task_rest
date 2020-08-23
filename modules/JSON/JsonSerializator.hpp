
#ifndef TASK_MANAGER_MODULES_JSON_JSONSERIALIZATOR_HPP_
#define TASK_MANAGER_MODULES_JSON_JSONSERIALIZATOR_HPP_
#include "ICommand.hpp"

std::string to_json(const Task& task);
std::optional<Task> parse_task_from_json(const std::string& json_str);

#endif //TASK_MANAGER_MODULES_JSON_JSONSERIALIZATOR_HPP_
