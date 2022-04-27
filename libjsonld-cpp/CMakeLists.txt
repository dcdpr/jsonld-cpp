############################################################
# Target: libjsonld-cpp

add_library(jsonld-cpp STATIC RemoteDocument.cpp RemoteDocument.h FileLoader.cpp FileLoader.h JsonLdOptions.h JsonLdConsts.h JsonLdUtils.cpp JsonLdUtils.h JsonLdApi.cpp JsonLdApi.h JsonLdOptions.cpp Context.cpp Context.h JsonLdError.cpp JsonLdError.h JsonLdProcessor.cpp JsonLdProcessor.h ObjUtils.cpp ObjUtils.h JsonLdUrl.cpp JsonLdUrl.h RDFDataset.cpp RDFDataset.h UniqueNamer.cpp UniqueNamer.h RDFDatasetUtils.cpp RDFDatasetUtils.h DoubleFormatter.cpp DoubleFormatter.h NormalizeUtils.cpp NormalizeUtils.h sha1.cpp sha1.h Permutator.cpp Permutator.h ../include/jsoninc.h DocumentLoader.cpp MediaType.cpp MediaType.h JSONDocument.cpp JSONDocument.h RDFDocument.cpp RDFDocument.h RDFRegex.h UriParser.cpp UriParser.h Uri.cpp Uri.h BlankNode.cpp BlankNode.h sha256.cpp sha256.h)

target_include_directories(jsonld-cpp
    PUBLIC
        $<INSTALL_INTERFACE:include/libjsonld-cpp>
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include;${CMAKE_CURRENT_SOURCE_DIR}>"
)

target_link_libraries(jsonld-cpp PUBLIC uriparser)
target_compile_features(jsonld-cpp PRIVATE cxx_std_11)
target_compile_options(jsonld-cpp PRIVATE ${DCD_CXX_FLAGS})
set_target_properties(jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)

install (TARGETS jsonld-cpp DESTINATION lib)
FILE(GLOB INCLUDES_H ${CMAKE_CURRENT_SOURCE_DIR}/Json*.h)
install (FILES ${INCLUDES_H} DESTINATION include)

