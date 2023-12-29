include(CTest)


add_subdirectory(testjsonld-cpp)
add_subdirectory(implementation-report)

if(JSONLDCPP_BUILD_GOOGLETEST)
  # Turn off some warnings to silence issues coming from googletest code
  if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    add_definitions(-Wno-everything -Wno-global-constructors -Wno-error)
  endif()

  option(INSTALL_GTEST "Enable installation of googletest." OFF)
  option(BUILD_GMOCK "Builds the googlemock subproject" OFF)
  add_subdirectory("googletest" EXCLUDE_FROM_ALL)

  # Restore warnings
  if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang)
    remove_definitions(-Wno-everything -Wno-global-constructors -Wno-error)
  endif()

endif()

if(JSONLDCPP_BUILD_RAPIDCHECK)
  # Turn off some warnings to silence issues coming from rapidcheck code
  if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    add_definitions(-Wno-everything -Wno-c++98-compat-pedantic)
  endif()

  option(RC_ENABLE_GTEST "Build Google Test integration" ON)
  add_subdirectory("rapidcheck" EXCLUDE_FROM_ALL)

  # Restore warnings
  if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang)
    remove_definitions(-Wno-everything -Wno-c++98-compat-pedantic)
  endif()

endif()

