cmake_minimum_required(VERSION 3.14)

project(libjsonld-cpp VERSION 0.5.0
                 DESCRIPTION "Digital Contract Design's C++ implementation of JSON-LD"
                 LANGUAGES C CXX )

set(CMAKE_CXX_STANDARD 11)

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  set(DCD_CXX_FLAGS -Wall -Wextra -Wpedantic -Wno-unknown-pragmas)
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  set(DCD_CXX_FLAGS -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-missing-prototypes)
endif()

include(GNUInstallDirs)

find_package(Boost 1.65 REQUIRED COMPONENTS  filesystem)

# Add subdirectories for external dependencies

option(CPR_BUILD_TESTS "Set to ON to build cpr tests." OFF)
add_subdirectory(external/cpr EXCLUDE_FROM_ALL)

add_subdirectory(external/http-link-header-cpp EXCLUDE_FROM_ALL)

option(URIPARSER_BUILD_DOCS "Build API documentation" OFF)
option(URIPARSER_BUILD_TESTS "Build test suite" OFF)
option(URIPARSER_BUILD_TOOLS "Build tools" OFF)
option(URIPARSER_BUILD_CHAR "Build code supporting data type 'char'" ON)
option(URIPARSER_BUILD_WCHAR_T "Build code supporting data type 'wchar_t'" ON)
option(URIPARSER_ENABLE_INSTALL "Enable installation of uriparser" OFF)
add_subdirectory(external/uriparser EXCLUDE_FROM_ALL)
add_library(uriparser::uriparser ALIAS uriparser)


# Add local subdirectories

# Set option to build tests or not. Other projects that embed jsonld-cpp
# may want to set this to OFF
set(JSONLDCPP_BUILD_TESTS ON CACHE BOOL "JSONLDCPP_BUILD_TESTS")

# Set option to build examples or not. Other projects that embed jsonld-cpp
# may want to set this to OFF
set(JSONLDCPP_BUILD_EXAMPLES ON CACHE BOOL "JSONLDCPP_BUILD_EXAMPLES")

# Set option to install jsonld-cpp library and header file. Other projects that
# embed jsonld-cpp may want to set this to OFF
option(INSTALL_JSONLDCPP "Enable installation of jsonld-cpp. (Projects embedding jsonld-cpp may want to turn this OFF.)" ON)


add_subdirectory(libjsonld-cpp)

if(JSONLDCPP_BUILD_TESTS)
  enable_testing()
  # Set options so we build googletest and rapidcheck. Other projects that
  # embed libjsonld-cpp should set these options to OFF
  # todo: can we detect that with toplevel?
  option(LIBJSONLDCPP_BUILD_GOOGLETEST "Build googletest" ON)
  option(LIBJSONLDCPP_BUILD_RAPIDCHECK "Build rapidcheck" ON)
  add_subdirectory(test)
endif()

if(JSONLDCPP_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
