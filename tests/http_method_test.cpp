#include <gtest/gtest.h>
#include <httpmethod.hpp>
#include <string>

using namespace shttp;

TEST(HttpMethodTest, Creation) {
  HttpMethod get(HttpMethod::GET);
  ASSERT_EQ(get, HttpMethod::GET);
}

TEST(HttpMethodTest, ToStringConvertion) {
  HttpMethod post(HttpMethod::POST);
  ASSERT_EQ(HttpMethod::toString(post), "POST");
}

TEST(HttpMethodTest, FromString) {
  std::string strMethod = "DELETE";
  ASSERT_EQ(HttpMethod::fromString(strMethod), HttpMethod::DELETE);
}

TEST(HttpMethodTest, RoundTripConversion) {
  const std::vector<HttpMethod> allMethods = {
      HttpMethod::GET,    HttpMethod::POST,    HttpMethod::PUT,
      HttpMethod::DELETE, HttpMethod::HEAD,    HttpMethod::OPTIONS,
      HttpMethod::PATCH,  HttpMethod::CONNECT, HttpMethod::TRACE};

  for (auto method : allMethods) {
    auto str = HttpMethod::toString(method);
    auto from = HttpMethod::fromString(str);
    EXPECT_EQ(method, from) << "Failed for " << str;
  }
}

TEST(HttpMethodTest, FromStringCaseInsensitive) {
  EXPECT_EQ(HttpMethod::fromString("get"), HttpMethod::GET);
  EXPECT_EQ(HttpMethod::fromString("pOsT"), HttpMethod::POST);
}

TEST(HttpMethodTest, FromStringUnknown) {
  EXPECT_EQ(HttpMethod::fromString("FOOBAR"), HttpMethod::UNKNOWN);
  EXPECT_EQ(HttpMethod::fromString(""), HttpMethod::UNKNOWN);
  EXPECT_EQ(HttpMethod::fromString("123"), HttpMethod::UNKNOWN);
}

TEST(HttpMethodTest, EqualityAndCopy) {
  HttpMethod m1(HttpMethod::PUT);
  HttpMethod m2(HttpMethod::PUT);
  HttpMethod m3(HttpMethod::DELETE);

  EXPECT_EQ(m1, m2);
  EXPECT_NE(m1, m3);

  HttpMethod copy = m1;
  EXPECT_EQ(copy, m1);
}
