cmake_minimum_required(VERSION 3.20)
project(shttp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/includes)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(UTIL_DIR ${CMAKE_SOURCE_DIR}/utils)

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} ${UTIL_DIR})

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)
