#
# Possible Input Settings (defined in environment or set on command line)
#
# JSONLDCPP_PROJECT : Project name. Default: "jsonld-cpp"
#
# JSONLDCPP_BUILD_TESTS : Build test executables [ON OFF]. Default: ON.
# JSONLDCPP_BUILD_EXAMPLES : Build example executables [ON OFF]. Default: ON.
# INSTALL_JSONLDCPP : Enable installation [ON OFF]. Default: ON.
#
# The test executables use googletest and rapidcheck. Other projects that
# embed jsonld-cpp and already use googletest and/or rapidcheck could set
# these options to OFF
# JSONLDCPP_BUILD_GOOGLETEST : Build googletest for testing [ON OFF]. Default: ON if JSONLDCPP_BUILD_TESTS is ON.
# JSONLDCPP_BUILD_RAPIDCHECK : Build rapidcheck for testing [ON OFF]. Default: ON if JSONLDCPP_BUILD_TESTS is ON.

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# Version settings

set(JSONLDCPP_VERSION_MAJOR 0)
set(JSONLDCPP_VERSION_MINOR 5)
set(JSONLDCPP_VERSION_PATCH 0)
set(JSONLDCPP_VERSION ${JSONLDCPP_VERSION_MAJOR}.${JSONLDCPP_VERSION_MINOR}.${JSONLDCPP_VERSION_PATCH} CACHE STRING "" FORCE)

# General settings

set(JSONLDCPP_PROJECT "jsonld-cpp" CACHE STRING "Project Name")

# Test settings

set(JSONLDCPP_BUILD_TESTS ON CACHE BOOL "Build test executables")
set(JSONLDCPP_BUILD_GOOGLETEST ON CACHE BOOL "Build googletest for testing")
set(JSONLDCPP_BUILD_RAPIDCHECK ON CACHE BOOL "Build rapidcheck for testing")

# Examples settings

set(JSONLDCPP_BUILD_EXAMPLES ON CACHE BOOL "Build example executables")

# Install

set(INSTALL_JSONLDCPP ON CACHE BOOL "Enable installation")


# Log settings

message(STATUS "---------------- JSONLDCPP OPTIONS ----------------")

message(STATUS "JSONLDCPP_PROJECT            : " ${JSONLDCPP_PROJECT})
message(STATUS "JSONLDCPP_VERSION            : " ${JSONLDCPP_VERSION})

message(STATUS "JSONLDCPP_BUILD_TESTS        : " ${JSONLDCPP_BUILD_TESTS})
message(STATUS "JSONLDCPP_BUILD_EXAMPLES     : " ${JSONLDCPP_BUILD_EXAMPLES})

message(STATUS "INSTALL_JSONLDCPP            : " ${INSTALL_JSONLDCPP})
message(STATUS "CMAKE_BUILD_TYPE            : " ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_INSTALL_PREFIX        : " ${CMAKE_INSTALL_PREFIX})
message(STATUS "CMAKE_TOOLCHAIN_FILE        : " ${CMAKE_TOOLCHAIN_FILE})

message(STATUS "---------------------------------------------------")

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(installDir ${CMAKE_CURRENT_BINARY_DIR}/install)
else()
  set(installDir ${CMAKE_INSTALL_PREFIX})
endif()
message(STATUS "installDir        : " ${installDir})


# Project

project(${JSONLDCPP_PROJECT} VERSION ${JSONLDCPP_VERSION}
        DESCRIPTION "Digital Contract Design's C++ implementation of JSON-LD"
        HOMEPAGE_URL "https://github.com/dcdpr/jsonld-cpp"
        LANGUAGES CXX )


# Super build...
include(ExternalProject)

# Add subdirectories for external dependencies

ExternalProject_Add(cpr
      SOURCE_DIR  ${CMAKE_CURRENT_LIST_DIR}/external/cpr
      INSTALL_DIR ${installDir}
      CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                  -DCPR_BUILD_TESTS=OFF
  )

ExternalProject_Add(uriparser
      SOURCE_DIR  ${CMAKE_CURRENT_LIST_DIR}/external/uriparser
      INSTALL_DIR ${installDir}
      CMAKE_ARGS  -DCMAKE_PREFIX_PATH:PATH=<INSTALL_DIR>
                  -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                  -DURIPARSER_BUILD_DOCS=OFF
                  -DURIPARSER_BUILD_TESTS=OFF
                  -DURIPARSER_BUILD_TOOLS=OFF
                  -DURIPARSER_BUILD_CHAR=ON
                  -DURIPARSER_BUILD_WCHAR_T=ON
                  -DURIPARSER_ENABLE_INSTALL=ON
  )

ExternalProject_Add(http-link-header-cpp
      SOURCE_DIR  ${CMAKE_CURRENT_LIST_DIR}/external/http-link-header-cpp
      GIT_REPOSITORY https://github.com/dcdpr/http-link-header-cpp.git
      GIT_TAG origin/main
      INSTALL_DIR ${installDir}
      UPDATE_DISCONNECTED 1
      CMAKE_ARGS  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                  -DCMAKE_PREFIX_PATH:PATH=<INSTALL_DIR>
                  -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
      DEPENDS     uriparser
  )

# Add local subdirectories

ExternalProject_Add(jsonld-cpp-library
      SOURCE_DIR  ${CMAKE_CURRENT_LIST_DIR}/jsonld-cpp
      INSTALL_DIR ${installDir}
      CMAKE_ARGS  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                  -DCMAKE_PREFIX_PATH:PATH=<INSTALL_DIR>
                  -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                  -DJSONLDCPP_VERSION=${JSONLDCPP_VERSION}
                  -DINSTALL_JSONLDCPP=${INSTALL_JSONLDCPP}
                  -DJSONLDCPP_BUILD_TESTS=${JSONLDCPP_BUILD_TESTS}
                  -DJSONLDCPP_BUILD_GOOGLETEST=${JSONLDCPP_BUILD_GOOGLETEST}
                  -DJSONLDCPP_BUILD_RAPIDCHECK=${JSONLDCPP_BUILD_RAPIDCHECK}
      DEPENDS     uriparser http-link-header-cpp cpr
  )

if(JSONLDCPP_BUILD_EXAMPLES)

ExternalProject_Add(jsonld-cpp-examples
      SOURCE_DIR  ${CMAKE_CURRENT_LIST_DIR}/examples
      INSTALL_DIR ${installDir}
      CMAKE_ARGS  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                  -DCMAKE_PREFIX_PATH:PATH=<INSTALL_DIR>
                  -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
                  -DJSONLDCPP_VERSION=${JSONLDCPP_VERSION}
      INSTALL_COMMAND     ""
      DEPENDS     jsonld-cpp-library
  )

endif()

add_custom_target(
  test
  COMMAND ctest
  WORKING_DIRECTORY jsonld-cpp-library-prefix/src/jsonld-cpp-library-build/test/testjsonld-cpp
  DEPENDS jsonld-cpp-library)
