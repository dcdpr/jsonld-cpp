add_executable(UnitTests_jsonld-cpp main.cpp test_JsonLdApi.cpp test_JsonLdUtils.cpp test_FileLoader.cpp testHelpers.cpp testHelpers.h test_NodeComparisons.cpp test_ObjectComparisons.cpp test_UniqueNamer.cpp test_DoubleFormatter.cpp test_Permutator.cpp test_NormalizeUtils.cpp test_Sha1.cpp test_Regex.cpp test_RDFDataset.cpp ManifestLoader.cpp ManifestLoader.h UrdnaManifestLoader.cpp UrdnaManifestLoader.h TestCase.cpp TestCase.h UriBaseRewriter.cpp UriBaseRewriter.h TestCaseOptions.cpp TestCaseOptions.h test_RDFDatasetUtils.cpp test_Uri.cpp test_UriParser.cpp)

target_compile_features(UnitTests_jsonld-cpp PRIVATE cxx_std_11)
target_compile_options(UnitTests_jsonld-cpp PRIVATE ${DCD_CXX_FLAGS})
set_target_properties(UnitTests_jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(UnitTests_jsonld-cpp
        PUBLIC
        ${PROJECT_SOURCE_DIR}/libjsonld-cpp)

target_link_libraries(UnitTests_jsonld-cpp jsonld-cpp Boost::filesystem gtest gmock rapidcheck_gtest)

add_test(NAME UnitTests_jsonld-cpp
        COMMAND UnitTests_jsonld-cpp)

####

add_executable(UnitTests_JsonLdProcessor_expand_jsonld-cpp main.cpp test_JsonLdProcessor_expand.cpp testHelpers.cpp testHelpers.h ManifestLoader.cpp ManifestLoader.h UrdnaManifestLoader.cpp UrdnaManifestLoader.h TestCase.cpp TestCase.h UriBaseRewriter.cpp UriBaseRewriter.h TestCaseOptions.cpp TestCaseOptions.h)

target_compile_features(UnitTests_JsonLdProcessor_expand_jsonld-cpp PRIVATE cxx_std_11)
target_compile_options(UnitTests_JsonLdProcessor_expand_jsonld-cpp PRIVATE ${DCD_CXX_FLAGS})
set_target_properties(UnitTests_JsonLdProcessor_expand_jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(UnitTests_JsonLdProcessor_expand_jsonld-cpp
        PUBLIC
        ${PROJECT_SOURCE_DIR}/libjsonld-cpp)

target_link_libraries(UnitTests_JsonLdProcessor_expand_jsonld-cpp jsonld-cpp Boost::filesystem gtest gmock rapidcheck_gtest)

add_test(NAME UnitTests_JsonLdProcessor_expand_jsonld-cpp
        COMMAND UnitTests_JsonLdProcessor_expand_jsonld-cpp)

####

add_executable(UnitTests_JsonLdProcessor_toRdf_jsonld-cpp main.cpp test_JsonLdProcessor_toRdf.cpp testHelpers.cpp testHelpers.h ManifestLoader.cpp ManifestLoader.h UrdnaManifestLoader.cpp UrdnaManifestLoader.h TestCase.cpp TestCase.h UriBaseRewriter.cpp UriBaseRewriter.h TestCaseOptions.cpp TestCaseOptions.h)

target_compile_features(UnitTests_JsonLdProcessor_toRdf_jsonld-cpp PRIVATE cxx_std_11)
target_compile_options(UnitTests_JsonLdProcessor_toRdf_jsonld-cpp PRIVATE ${DCD_CXX_FLAGS})
set_target_properties(UnitTests_JsonLdProcessor_toRdf_jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(UnitTests_JsonLdProcessor_toRdf_jsonld-cpp
        PUBLIC
        ${PROJECT_SOURCE_DIR}/libjsonld-cpp)

target_link_libraries(UnitTests_JsonLdProcessor_toRdf_jsonld-cpp jsonld-cpp Boost::filesystem gtest gmock rapidcheck_gtest)

add_test(NAME UnitTests_JsonLdProcessor_toRdf_jsonld-cpp
        COMMAND UnitTests_JsonLdProcessor_toRdf_jsonld-cpp)

####

add_executable(UnitTests_JsonLdProcessor_normalize_jsonld-cpp main.cpp test_JsonLdProcessor_normalize.cpp testHelpers.cpp testHelpers.h ManifestLoader.cpp ManifestLoader.h UrdnaManifestLoader.cpp UrdnaManifestLoader.h TestCase.cpp TestCase.h UriBaseRewriter.cpp UriBaseRewriter.h TestCaseOptions.cpp TestCaseOptions.h)

target_compile_features(UnitTests_JsonLdProcessor_normalize_jsonld-cpp PRIVATE cxx_std_11)
target_compile_options(UnitTests_JsonLdProcessor_normalize_jsonld-cpp PRIVATE ${DCD_CXX_FLAGS})
set_target_properties(UnitTests_JsonLdProcessor_normalize_jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(UnitTests_JsonLdProcessor_normalize_jsonld-cpp
        PUBLIC
        ${PROJECT_SOURCE_DIR}/libjsonld-cpp)

target_link_libraries(UnitTests_JsonLdProcessor_normalize_jsonld-cpp jsonld-cpp Boost::filesystem gtest gmock rapidcheck_gtest)

add_test(NAME UnitTests_JsonLdProcessor_normalize_jsonld-cpp
        COMMAND UnitTests_JsonLdProcessor_normalize_jsonld-cpp)

####

add_executable(UnitTests_JsonLdProcessor_normalize_urdna2015_jsonld-cpp main.cpp test_JsonLdProcessor_normalize-urdna2015.cpp testHelpers.cpp testHelpers.h ManifestLoader.cpp ManifestLoader.h UrdnaManifestLoader.cpp UrdnaManifestLoader.h TestCase.cpp TestCase.h UriBaseRewriter.cpp UriBaseRewriter.h TestCaseOptions.cpp TestCaseOptions.h)

target_compile_features(UnitTests_JsonLdProcessor_normalize_urdna2015_jsonld-cpp PRIVATE cxx_std_11)
target_compile_options(UnitTests_JsonLdProcessor_normalize_urdna2015_jsonld-cpp PRIVATE ${DCD_CXX_FLAGS})
set_target_properties(UnitTests_JsonLdProcessor_normalize_urdna2015_jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(UnitTests_JsonLdProcessor_normalize_urdna2015_jsonld-cpp
        PUBLIC
        ${PROJECT_SOURCE_DIR}/libjsonld-cpp)

target_link_libraries(UnitTests_JsonLdProcessor_normalize_urdna2015_jsonld-cpp jsonld-cpp Boost::filesystem gtest gmock rapidcheck_gtest)

add_test(NAME UnitTests_JsonLdProcessor_normalize_urdna2015_jsonld-cpp
        COMMAND UnitTests_JsonLdProcessor_normalize_urdna2015_jsonld-cpp)

