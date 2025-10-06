enable_testing()

set(Target Tests)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(
  ${Target}
  ${TEST_SOURCES}
)

target_link_libraries(
  ${Target}
  PRIVATE
  GTest::gtest_main
  ${PROJECT_NAME}
)

include(GoogleTest)
gtest_discover_tests(${Target})
