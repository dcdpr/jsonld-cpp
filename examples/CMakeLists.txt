cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project("jsonld-cpp-examples" VERSION ${JSONLDCPP_VERSION}
        LANGUAGES CXX )

find_package(jsonld-cpp ${JSONLDCPP_VERSION} CONFIG REQUIRED)

add_executable(jsonld2rdf jsonld2rdf.cpp)

# Misc properties

set_target_properties(jsonld2rdf PROPERTIES CXX_EXTENSIONS OFF)

# Set version

target_compile_definitions(jsonld2rdf PRIVATE -DJSONLDCPP_VERSION_MAJOR=${JSONLDCPP_VERSION_MAJOR})
target_compile_definitions(jsonld2rdf PRIVATE -DJSONLDCPP_VERSION_MINOR=${JSONLDCPP_VERSION_MINOR})
target_compile_definitions(jsonld2rdf PRIVATE -DJSONLDCPP_VERSION_PATCH=${JSONLDCPP_VERSION_PATCH})

# Link

target_link_libraries(jsonld2rdf jsonld-cpp::jsonld-cpp)

