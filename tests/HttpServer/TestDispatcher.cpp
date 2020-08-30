#include <gmock/gmock.h>
#include <sstream>
#include "PathDispatcher.hpp"

using namespace testing;

class MockPathDispatcher : public Dispatcher {
 public:
  MOCK_METHOD(int, addTask, (std::istream &body, size_t contentLenght, std::ostream &ostr), (override));
};

struct TestPathDispatcher: Test {
  StrictMock<MockPathDispatcher> dispatcher;
};

TEST_F(TestPathDispatcher, TasksGet) {
  HttpHeader header;
  header.path = "/tasks";
  header.method = HttpMethod::get;
  std::stringstream sstr;

  EXPECT_CALL(dispatcher, processTaskGet(_,_));
  dispatcher.dispatch(header,"",sstr);
}

TEST_F(TestPathDispatcher, SubtasksGet) {
  HttpHeader header;
  header.path = "/subtasks";
  header.method = HttpMethod::get;
  std::stringstream sstr;

  EXPECT_CALL(dispatcher, processTaskGet(_,_));
  dispatcher.dispatch(header,"",sstr);
}
