#include <gtest/gtest.h>
#include <httprequest.hpp>

using namespace shttp;

TEST(HttpRequest, ParseSimplePostRequest) {
  std::string raw = "POST /submit HTTP/1.1\r\n"
                    "Host: example.com\r\n"
                    "Content-Type: text/plain\r\n"
                    "Content-Length: 11\r\n"
                    "\r\n"
                    "hello world";

  HttpRequest req(raw);

  EXPECT_EQ(req.getMethod(), HttpMethod::POST);
  EXPECT_EQ(req.getUri(), "/submit");
  EXPECT_EQ(req.getVersion(), HttpVersion(HttpVersion::HTTP_1_1));

  ASSERT_TRUE(req.hasHeader("Host"));
  EXPECT_EQ(req.getHeaderContent("Host"), "example.com");

  ASSERT_TRUE(req.hasHeader("Content-Type"));
  EXPECT_EQ(req.getHeaderContent("Content-Type"), "text/plain");

  EXPECT_EQ(req.getBody(), "hello world");
}
