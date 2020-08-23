
#include "PathDispatcher.hpp"
#include <regex>
#include <cstdio>
#include <../PostgreSQL/PGConnection.hpp>
#include <../JSON/JsonSerializator.hpp>

std::regex tasks_regex(R"(\/tasks(\/\d)?)");

PGConnection &getConnection() {
  static PGConnection connection{"host=localhost dbname=tasks user=task_tracker_user password=password"};
  return connection;
}

int pathDisatch(std::ostream &ostr, const std::string &path, HttpMethod httpMethod) {
  std::smatch match;
  if (std::regex_match(path, match, tasks_regex)) {
    if (match.size() == 2) {
      if (httpMethod == HttpMethod::get) {
        auto tasks = getConnection().getTasks();
        ostr << "{";
        bool first = true;
        for(auto& task: tasks) {
          if(!first) ostr << ',';
          else first = false;
          ostr << to_json(task);
        }
        ostr << "}";
      }
    } else if (match.size() == 3) {
      std::ssub_match match_id = match[1];
      std::string id_str = match_id.str();
      int id;
      if (sscanf(id_str.c_str(), "%d", &id) == 1);
    }
  }
  return 200;
}
