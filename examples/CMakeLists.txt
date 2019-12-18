add_executable(jsonld2rdf jsonld2rdf.cpp)

target_compile_features(jsonld2rdf PRIVATE cxx_std_11)
target_compile_options(jsonld2rdf PRIVATE ${DCD_CXX_FLAGS})
set_target_properties(jsonld2rdf PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(jsonld2rdf
        PUBLIC
        ${PROJECT_SOURCE_DIR}/libjsonld-cpp)

target_link_libraries(jsonld2rdf jsonld-cpp Boost::filesystem)

