cmake_minimum_required(VERSION 3.14)

project(libjsonld-cpp VERSION 1.0.0
                 DESCRIPTION "Digital Contract Design's C++ implementation of JSON-LD"
                 LANGUAGES C CXX )

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  set(DCD_CXX_FLAGS -Wall -Wextra -Wpedantic -Wno-unknown-pragmas)
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  set(DCD_CXX_FLAGS -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-missing-prototypes)
endif()

find_package(Boost 1.70 REQUIRED COMPONENTS  filesystem)

add_subdirectory(libjsonld-cpp)

enable_testing()

# Set options so we build googletest and rapidcheck. Other projects that
# embed libjsonld-cpp should set these options to OFF
option(LIBJSONLDCPP_BUILD_GOOGLETEST "Build googletest" ON)
option(LIBJSONLDCPP_BUILD_RAPIDCHECK "Build rapidcheck" ON)

add_subdirectory(test)

