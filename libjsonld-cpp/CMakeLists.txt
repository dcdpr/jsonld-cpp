############################################################
# Target: libjsonld-cpp

add_library(jsonld-cpp STATIC RemoteDocument.cpp RemoteDocument.h DocumentLoader.cpp DocumentLoader.h JsonLdOptions.h JsonLdConsts.h JsonLdUtils.cpp JsonLdUtils.h JsonLdApi.cpp JsonLdApi.h JsonLdOptions.cpp Context.cpp Context.h JsonLdError.cpp JsonLdError.h JsonLdProcessor.cpp JsonLdProcessor.h ObjUtils.cpp ObjUtils.h JsonLdUrl.cpp JsonLdUrl.h IriUtils.cpp IriUtils.h RDFDataset.cpp RDFDataset.h BlankNodeIdentifierGenerator.cpp BlankNodeIdentifierGenerator.h RDFDatasetUtils.cpp RDFDatasetUtils.h DoubleFormatter.cpp DoubleFormatter.h)

target_include_directories(jsonld-cpp
    PUBLIC
        $<INSTALL_INTERFACE:include/libjsonld-cpp>
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include;${CMAKE_CURRENT_SOURCE_DIR}/../include/libjsonld-cpp>"
)

target_compile_features(jsonld-cpp PRIVATE cxx_std_11)
target_compile_options(jsonld-cpp PRIVATE ${DCD_CXX_FLAGS})
set_target_properties(jsonld-cpp PROPERTIES CXX_EXTENSIONS OFF)

install (TARGETS jsonld-cpp DESTINATION lib)
install (FILES
${CMAKE_CURRENT_SOURCE_DIR}/../include/libjsonld-cpp/jsonld.h DESTINATION include)

