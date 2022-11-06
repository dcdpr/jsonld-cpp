# Turn off some warnings to silence issues coming from googletest code
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  add_definitions(-Wno-weak-vtables -Wno-exit-time-destructors -Wno-global-constructors)
endif()

#
# Build common test files into lib so they only need to be compiled once
#
add_library(
    libjsonld-cpp_test STATIC
    main.cpp
    ManifestLoader.cpp
    ManifestLoader.h
    UrdnaManifestLoader.cpp
    UrdnaManifestLoader.h
    TestCase.cpp
    TestCase.h
    UriBaseRewriter.cpp
    UriBaseRewriter.h
    TestCaseOptions.cpp
    TestCaseOptions.h
)

target_link_libraries(
    libjsonld-cpp_test
    jsonld-cpp::jsonld-cpp
    gtest
    gmock
    rapidcheck_gtest
)

#
# jsonld-cpp
#
add_executable(
    UnitTests_jsonld-cpp
    test_FileLoader.cpp
    test_JsonLdUtils.cpp
    test_NodeComparisons.cpp
    test_ObjectComparisons.cpp
    test_BlankNodeNames.cpp
    test_DoubleFormatter.cpp
    test_NQuadsParser.cpp
    test_Permutator.cpp
    test_Regex.cpp
    test_RDFDataset.cpp
    test_RDFDatasetComparison.cpp
    test_RDFTriple.cpp
    test_RDFQuad.cpp
    test_Uri.cpp
    test_UriParser.cpp
    test_Sha256.cpp
)

target_compile_features(UnitTests_jsonld-cpp PRIVATE cxx_std_11)
set_target_properties(UnitTests_jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)
target_compile_definitions(UnitTests_jsonld-cpp PUBLIC TEST_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(UnitTests_jsonld-cpp PUBLIC libjsonld-cpp_test )

add_test(NAME UnitTests_jsonld-cpp
         COMMAND UnitTests_jsonld-cpp)

#
# API
#
set(ALGORITHMS "expand;toRDF;normalize-urdna2015")

foreach(ALGO ${ALGORITHMS})
    message(STATUS "Adding ${ALGO}")
    add_executable(
        UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp
        test_JsonLdProcessor_${ALGO}.cpp
    )

    target_compile_features(UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp PRIVATE cxx_std_11)
    set_target_properties(UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)
    target_compile_definitions(UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp PUBLIC TEST_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

    target_link_libraries(UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp libjsonld-cpp_test)

    add_test(NAME UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp
             COMMAND UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp)
endforeach()
