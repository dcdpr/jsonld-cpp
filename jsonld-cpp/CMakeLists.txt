cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

## set default install prefix if we are top-level project
#if(NOT WIN32 AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
#    set(CMAKE_INSTALL_PREFIX "/opt/contract.design/jsonld-cpp")
#endif()

project(jsonld-cpp-library VERSION ${JSONLDCPP_VERSION}
        LANGUAGES CXX )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG_INIT -fno-limit-debug-info)
include(GNUInstallDirs)

find_package(uriparser 0.9.5 CONFIG REQUIRED char wchar_t)


# Define library target

set(LIB_HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Context.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ContextProcessor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/DocumentLoader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/DoubleFormatter.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ExpansionProcessor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/FileLoader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/JSONDocument.h
        ${CMAKE_CURRENT_SOURCE_DIR}/JsonLdConsts.h
        ${CMAKE_CURRENT_SOURCE_DIR}/JsonLdError.h
        ${CMAKE_CURRENT_SOURCE_DIR}/JsonLdOptions.h
        ${CMAKE_CURRENT_SOURCE_DIR}/JsonLdProcessor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/JsonLdUrl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/JsonLdUtils.h
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaType.h
        ${CMAKE_CURRENT_SOURCE_DIR}/NQuadsSerialization.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Permutator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFCanonicalizationProcessor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFDataset.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFDatasetComparison.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFDocument.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFNode.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFSerializationProcessor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFTriple.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFRegex.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFQuad.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RemoteDocument.h
        ${CMAKE_CURRENT_SOURCE_DIR}/BlankNodeNames.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Uri.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UriParser.h
        ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/jsoninc.h
        ${CMAKE_CURRENT_SOURCE_DIR}/sha256.h
        )

set(LIB_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Context.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ContextProcessor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/DocumentLoader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/DoubleFormatter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ExpansionProcessor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/FileLoader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/JSONDocument.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/JsonLdError.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/JsonLdOptions.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/JsonLdProcessor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/JsonLdUrl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/JsonLdUtils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/MediaType.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/NQuadsSerialization.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Permutator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFCanonicalizationProcessor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFDataset.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFDatasetComparison.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFDocument.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFNode.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFSerializationProcessor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFTriple.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RDFQuad.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RemoteDocument.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/BlankNodeNames.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Uri.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UriParser.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sha256.cpp)

add_library(jsonld-cpp STATIC ${LIB_HEADER_FILES} ${LIB_SOURCE_FILES})
add_library(jsonld-cpp::jsonld-cpp ALIAS jsonld-cpp)


set_property(
        TARGET jsonld-cpp
        PROPERTY PUBLIC_HEADER ${LIB_HEADER_FILES} ${LIB_INT_HEADER_FILES}
)

# Set include directory

target_include_directories(jsonld-cpp
        PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        )

# Link libraries

target_link_libraries(jsonld-cpp PUBLIC uriparser::uriparser)

# Misc properties

target_compile_features(jsonld-cpp PRIVATE cxx_std_11)
set_target_properties(jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)

# Set version

target_compile_definitions(jsonld-cpp PRIVATE -DJSONLDCPP_VERSION_MAJOR=${JSONLDCPP_VERSION_MAJOR})
target_compile_definitions(jsonld-cpp PRIVATE -DJSONLDCPP_VERSION_MINOR=${JSONLDCPP_VERSION_MINOR})
target_compile_definitions(jsonld-cpp PRIVATE -DJSONLDCPP_VERSION_PATCH=${JSONLDCPP_VERSION_PATCH})

if(JSONLDCPP_BUILD_TESTS)
  enable_testing()
  # Set options to build googletest and rapidcheck or not. Other
  # projects that embed libjsonld-cpp and already use googletest and/or
  # rapidcheck could set these options to OFF
  option(JSONLDCPP_BUILD_GOOGLETEST "Build googletest" ${JSONLDCPP_BUILD_GOOGLETEST})
  option(JSONLDCPP_BUILD_RAPIDCHECK "Build rapidcheck" ${JSONLDCPP_BUILD_RAPIDCHECK})
  add_subdirectory(test)
endif()

if(INSTALL_JSONLDCPP)

  # Configuration

  set(version_config "${CMAKE_CURRENT_BINARY_DIR}/jsonld-cppConfigVersion.cmake")
  set(project_config "${CMAKE_CURRENT_BINARY_DIR}/jsonld-cppConfig.cmake")
  set(namespace "jsonld-cpp::")

  # Include module with function 'write_basic_package_version_file' and
  # 'configure_package_config_file'
  include(CMakePackageConfigHelpers)

  # Configure '<PROJECT-NAME>ConfigVersion.cmake'
  # Note: PROJECT_VERSION is used as a VERSION
  write_basic_package_version_file(
          "${version_config}"
          COMPATIBILITY SameMajorVersion
  )

  # Install '<PROJECT-NAME>ConfigVersion.cmake'
  install(
          FILES ${version_config}
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/jsonld-cpp)

  # Configure '<PROJECT-NAME>Config.cmake' from .in file
  configure_package_config_file(
          "${PROJECT_SOURCE_DIR}/cmake/jsonld-cppConfig.cmake.in"
          ${project_config}
          INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/jsonld-cpp
  )

  # Install '<PROJECT-NAME>Config.cmake'
  install(FILES ${project_config}
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/jsonld-cpp
          )

  # Install Targets

  install(
          EXPORT jsonld-cpp_Targets
          FILE ${PROJECT_NAME}Targets.cmake
          NAMESPACE ${namespace}
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/jsonld-cpp
  )


  install(
          TARGETS jsonld-cpp
          EXPORT jsonld-cpp_Targets
          ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
          LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
          RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jsonld-cpp
  )


endif()

