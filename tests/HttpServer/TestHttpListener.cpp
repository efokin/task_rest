#include <gmock/gmock.h>
#include "IServer.hpp"
#include "HttpPocoServer.hpp"
#include <Poco/Process.h>

using namespace testing;

bool requiestIsReseived = false;
int responseHandler(std::ostream &ostr, const std::string &, HttpMethod) {
  ostr << "test output 123\n";
  requiestIsReseived = true;
  return 400;
}

struct TestHttpListener : Test {
  RequestHandlerCallback requestHandlerCallback = &responseHandler;
  HTTPPocoServer poco_srv = HTTPPocoServer(requestHandlerCallback);
  IServer &srv = srv;
};

TEST_F(TestHttpListener, DISABLED_ReceiveMessage) {
  char* arg[] = {"TestHttpListener","h"};
  poco_srv.run(2, (char**)arg);
  sleep(5);
  ASSERT_TRUE(requiestIsReseived);
}