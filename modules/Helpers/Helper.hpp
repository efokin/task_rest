
#ifndef TASK_MANAGER_MODULES_POSTGRESQL_PGHELPER_HPP_
#define TASK_MANAGER_MODULES_POSTGRESQL_PGHELPER_HPP_

#include <string>
#include <chrono>
#include <optional>

std::string serializeTimePoint(const std::chrono::system_clock::time_point &time);

std::optional<std::chrono::system_clock::time_point> deserializeTimePoint(const std::string &str);

std::string readStringFromIstream(std::istream& istr, size_t length);

#endif //TASK_MANAGER_MODULES_POSTGRESQL_PGHELPER_HPP_
