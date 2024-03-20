# Turn off some warnings to silence issues coming from googletest code
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  add_definitions(-Wno-weak-vtables -Wno-exit-time-destructors -Wno-global-constructors)
endif()

add_library(
    libjsonld-cpp_test STATIC
    main.cpp
    ManifestLoader.cpp
    ManifestLoader.h
    RDFDatasetComparison.cpp
    RDFDatasetComparison.h
    TestCase.cpp
    TestCase.h
    TestCaseOptions.cpp
    TestCaseOptions.h
    UriBaseRewriter.cpp
    UriBaseRewriter.h
)

# These target_properties do not seem to be inherited from the jsonld-cpp::jsonld-cpp
# dependency, perhaps because there is conflict from properties in other dependencies?
# Set it here to be sure.
set_target_properties(libjsonld-cpp_test PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(
    libjsonld-cpp_test
    jsonld-cpp::jsonld-cpp
    gtest
    rapidcheck_gtest
)

#
# jsonld-cpp
#
add_executable(
    UnitTests_jsonld-cpp
    test_BlankNodeNames.cpp
    test_DoubleFormatter.cpp
    test_FileLoader.cpp
    test_JsonLdUrl.cpp
    test_JsonLdUtils.cpp
    test_NQuadsSerialization.cpp
    test_NodeComparisons.cpp
    test_ObjectComparisons.cpp
    test_RDFDataset.cpp
    test_RDFDatasetComparison.cpp
    test_RDFQuad.cpp
    test_RDFTriple.cpp
    test_Regex.cpp
    test_Uri.cpp
    test_UriParser.cpp
)

# These target_properties do not seem to be inherited from the jsonld-cpp::jsonld-cpp
# dependency, perhaps because there is conflict from properties in other dependencies?
# Set it here to be sure.
set_target_properties(UnitTests_jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)

target_compile_definitions(UnitTests_jsonld-cpp PUBLIC TEST_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

target_link_libraries(UnitTests_jsonld-cpp PUBLIC libjsonld-cpp_test )

add_test(NAME UnitTests_jsonld-cpp
         COMMAND UnitTests_jsonld-cpp)

#
# API
#
set(ALGORITHMS "expand;toRDF")

foreach(ALGO ${ALGORITHMS})
    message(STATUS "Adding ${ALGO}")
    add_executable(
        UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp
        test_JsonLdProcessor_${ALGO}.cpp
    )

    # These target_properties do not seem to be inherited from the jsonld-cpp::jsonld-cpp
    # dependency, perhaps because there is conflict from properties in other dependencies?
    # Set it here to be sure.
    set_target_properties(UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)

    target_compile_definitions(UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp PUBLIC TEST_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

    target_link_libraries(UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp libjsonld-cpp_test)

    add_test(NAME UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp
             COMMAND UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp)
endforeach()
