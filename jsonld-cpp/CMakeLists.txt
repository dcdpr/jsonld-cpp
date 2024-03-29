cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

## set default install prefix if we are top-level project
#if(NOT WIN32 AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
#    set(CMAKE_INSTALL_PREFIX "/opt/contract.design/jsonld-cpp")
#endif()

project(jsonld-cpp-library VERSION ${JSONLDCPP_VERSION}
        LANGUAGES CXX )

include(GNUInstallDirs)

find_package(uriparser 0.9.5 CONFIG REQUIRED char wchar_t)


# Define library target

set(LIB_HEADER_FILES
        BlankNodeNames.h
        DocumentLoader.h
        FileLoader.h
        JSONDocument.h
        JsonLdConsts.h
        JsonLdError.h
        JsonLdOptions.h
        JsonLdProcessor.h
        JsonLdUrl.h
        MediaType.h
        NQuadsSerialization.h
        RDFDataset.h
        RDFDocument.h
        RDFNode.h
        RDFQuad.h
        RDFTriple.h
        RemoteDocument.h
        json.hpp
        jsoninc.h
)

set(LIB_PRIVATE_HEADER_FILES
        detail/Context.h
        detail/ContextProcessor.h
        detail/DoubleFormatter.h
        detail/ExpansionProcessor.h
        detail/JsonLdUtils.h
        detail/RDFRegex.h
        detail/RDFSerializationProcessor.h
        detail/Uri.h
        detail/UriParser.h
        detail/WellFormed.h
)

set(LIB_SOURCE_FILES
        BlankNodeNames.cpp
        DocumentLoader.cpp
        FileLoader.cpp
        JSONDocument.cpp
        JsonLdError.cpp
        JsonLdProcessor.cpp
        JsonLdUrl.cpp
        MediaType.cpp
        NQuadsSerialization.cpp
        RDFDataset.cpp
        RDFDocument.cpp
        RDFNode.cpp
        RDFQuad.cpp
        RDFTriple.cpp
        RemoteDocument.cpp
        detail/Context.cpp
        detail/ContextProcessor.cpp
        detail/DoubleFormatter.cpp
        detail/ExpansionProcessor.cpp
        detail/JsonLdUtils.cpp
        detail/RDFRegex.cpp
        detail/RDFSerializationProcessor.cpp
        detail/Uri.cpp
        detail/UriParser.cpp
        detail/WellFormed.cpp
)

add_library(jsonld-cpp STATIC ${LIB_HEADER_FILES} ${LIB_PRIVATE_HEADER_FILES} ${LIB_SOURCE_FILES})
add_library(jsonld-cpp::jsonld-cpp ALIAS jsonld-cpp)


set_property(
        TARGET jsonld-cpp
        PROPERTY PUBLIC_HEADER ${LIB_HEADER_FILES}
)

set_property(
        TARGET jsonld-cpp
        PROPERTY PRIVATE_HEADER ${LIB_PRIVATE_HEADER_FILES}
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

target_compile_features(jsonld-cpp PUBLIC cxx_std_14)
set_target_properties(jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)

# Set version

target_compile_definitions(jsonld-cpp PRIVATE -DJSONLDCPP_VERSION=${JSONLDCPP_VERSION})

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

