add_executable(UnitTests_jsonld-cpp main.cpp test_IriUtils.cpp test_JsonLdApi.cpp test_JsonLdProcessor_expand.cpp test_JsonLdProcessor_toRDF.cpp test_JsonLdProcessor_normalize.cpp test_JsonLdUtils.cpp test_DocumentLoader.cpp testHelpers.cpp testHelpers.h test_NodeComparisons.cpp test_ObjectComparisons.cpp test_UniqueIdentifierGenerator.cpp test_DoubleFormatter.cpp test_Permutator.cpp test_NormalizeUtils.cpp test_Sha1.cpp test_Sha256.cpp)

target_compile_features(UnitTests_jsonld-cpp PRIVATE cxx_std_11)
target_compile_options(UnitTests_jsonld-cpp PRIVATE ${DCD_CXX_FLAGS})
set_target_properties(UnitTests_jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(UnitTests_jsonld-cpp
    PUBLIC
        ${PROJECT_SOURCE_DIR}/libjsonld-cpp)

target_link_libraries(UnitTests_jsonld-cpp jsonld-cpp Boost::filesystem gtest gmock rapidcheck_gtest)

add_test(NAME UnitTests_jsonld-cpp
         COMMAND UnitTests_jsonld-cpp)

