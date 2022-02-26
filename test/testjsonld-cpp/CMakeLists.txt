#
# Build common testfiles into lib so they only need to be compiled once
#
add_library(
    libjsonld-cpp_test STATIC
    main.cpp
    testHelpers.cpp
    testHelpers.h
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

target_include_directories(
    libjsonld-cpp_test
    PUBLIC
    ${PROJECT_SOURCE_DIR}/libjsonld-cpp
)

target_link_libraries(
    libjsonld-cpp_test
    jsonld-cpp
    Boost::filesystem
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
    test_JsonLdApi.cpp
    test_JsonLdUtils.cpp
    test_NodeComparisons.cpp
    test_ObjectComparisons.cpp
    test_UniqueNamer.cpp
    test_DoubleFormatter.cpp
    test_Permutator.cpp
    test_NormalizeUtils.cpp
    test_Sha1.cpp
    test_Regex.cpp
    test_RDFDataset.cpp
    test_RDFDatasetUtils.cpp
    test_Uri.cpp
    test_UriParser.cpp
    test_Sha256.cpp
)

target_compile_features(
    UnitTests_jsonld-cpp
    PRIVATE
    cxx_std_11
)
target_compile_options(
    UnitTests_jsonld-cpp
    PRIVATE
    ${DCD_CXX_FLAGS}
)
set_target_properties(
    UnitTests_jsonld-cpp
    PROPERTIES
    CXX_EXTENSIONS OFF
)
target_include_directories(
    UnitTests_jsonld-cpp
    PUBLIC
    ${PROJECT_SOURCE_DIR}/libjsonld-cpp
)
target_link_libraries(
    UnitTests_jsonld-cpp
    libjsonld-cpp_test
)
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
    target_compile_features(
        UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp
        PRIVATE
        cxx_std_11
    )
    target_compile_options(
        UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp
        PRIVATE
        ${DCD_CXX_FLAGS}
    )
    set_target_properties(
        UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp
        PROPERTIES
        CXX_EXTENSIONS OFF
    )
    target_include_directories(
        UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp
        PUBLIC
        ${PROJECT_SOURCE_DIR}/libjsonld-cpp
    )
    target_link_libraries(
        UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp
        libjsonld-cpp_test
    )
    add_test(NAME UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp
        COMMAND UnitTests_JsonLdProcessor_${ALGO}_jsonld-cpp)
endforeach()
