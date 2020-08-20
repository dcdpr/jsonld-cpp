#pragma ide diagnostic ignored "cert-err58-cpp"
#include "JsonLdProcessor.h"
#include "testHelpers.h"
#include "ManifestLoader.h"
#include <fstream>

using nlohmann::json;

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop


void performExpandTest(TestCase &testCase) {
    JsonLdOptions options = testCase.getOptions();

    std::cout << "Id: " << testCase.id << std::endl;
    std::cout << "Name: " << testCase.name << std::endl;
    if(!testCase.options.specVersion.empty())
        std::cout << "SpecVersion: " << testCase.options.specVersion;
    if(testCase.options.specVersion == "json-ld-1.1") {
        std::cout << " ...skipping for now." << std::endl;
        SUCCEED();
        return;
    }
    else
        std::cout << std::endl;

    json expanded;
    try {
        expanded = JsonLdProcessor::expand(testCase.input, options);
    }
    catch (JsonLdError &e) {
        std::cout << "JsonLdError: " << e.what() << std::endl;
        if(testCase.type.count("jld:NegativeEvaluationTest")) {
            std::string error = e.what();
            if(error.find(testCase.expectErrorCode) != std::string::npos) {
                SUCCEED();
                return;
            }
        }
    }
    catch (std::runtime_error &e) {
        FAIL() << e.what();
    }

    std::unique_ptr<RemoteDocument> expectedDocument =
            options.getDocumentLoader()->loadDocument(testCase.expect);

    const json& expected = expectedDocument->getJSONContent();

    EXPECT_TRUE(JsonLdUtils::deepCompare(expected, expanded));
    //std::cout << expected.dump() << std::endl;
    //std::cout << expanded.dump() << std::endl;
}

void performExpandTestFromManifest(const std::string& testName, const std::string& manifestName="expand-manifest.jsonld") {

    ManifestLoader manifestLoader(
            resolvePath("test/testjsonld-cpp/test_data/"),
            manifestName);
    std::map<std::string, TestCase> testCases = manifestLoader.load();

    auto testCase = testCases.at(testName);

    performExpandTest(testCase);

}

TEST(JsonLdProcessorTest, expand_t0001) {
    performExpandTestFromManifest("#t0001");
}

TEST(JsonLdProcessorTest, expand_t0002) {
    performExpandTestFromManifest("#t0002");
}

TEST(JsonLdProcessorTest, expand_t0003) {
    performExpandTestFromManifest("#t0003");
}

TEST(JsonLdProcessorTest, expand_t0004) {
    performExpandTestFromManifest("#t0004");
}

TEST(JsonLdProcessorTest, expand_t0005) {
    performExpandTestFromManifest("#t0005");
}

TEST(JsonLdProcessorTest, expand_t0006) {
    performExpandTestFromManifest("#t0006");
}

TEST(JsonLdProcessorTest, expand_t0007) {
    performExpandTestFromManifest("#t0007");
}

TEST(JsonLdProcessorTest, expand_t0008) {
    performExpandTestFromManifest("#t0008");
}

TEST(JsonLdProcessorTest, expand_t0009) {
    performExpandTestFromManifest("#t0009");
}

TEST(JsonLdProcessorTest, expand_t0010) {
    performExpandTestFromManifest("#t0010");
}

TEST(JsonLdProcessorTest, expand_t0011) {
    performExpandTestFromManifest("#t0011");
}

TEST(JsonLdProcessorTest, expand_t0012) {
    performExpandTestFromManifest("#t0012");
}

TEST(JsonLdProcessorTest, expand_t0013) {
    performExpandTestFromManifest("#t0013");
}

TEST(JsonLdProcessorTest, expand_t0014) {
    performExpandTestFromManifest("#t0014");
}

TEST(JsonLdProcessorTest, expand_t0015) {
    performExpandTestFromManifest("#t0015");
}

TEST(JsonLdProcessorTest, expand_t0016) {
    performExpandTestFromManifest("#t0016");
}

TEST(JsonLdProcessorTest, expand_t0017) {
    performExpandTestFromManifest("#t0017");
}

TEST(JsonLdProcessorTest, expand_t0018) {
    performExpandTestFromManifest("#t0018");
}

TEST(JsonLdProcessorTest, expand_t0019) {
    performExpandTestFromManifest("#t0019");
}

TEST(JsonLdProcessorTest, expand_t0020) {
    performExpandTestFromManifest("#t0020");
}

TEST(JsonLdProcessorTest, expand_t0021) {
    performExpandTestFromManifest("#t0021");
}

TEST(JsonLdProcessorTest, expand_t0022) {
    performExpandTestFromManifest("#t0022");
}

TEST(JsonLdProcessorTest, expand_t0023) {
    performExpandTestFromManifest("#t0023");
}

TEST(JsonLdProcessorTest, expand_t0024) {
    performExpandTestFromManifest("#t0024");
}

TEST(JsonLdProcessorTest, expand_t0025) {
    performExpandTestFromManifest("#t0025");
}

TEST(JsonLdProcessorTest, expand_t0026) {
    performExpandTestFromManifest("#t0026");
}

TEST(JsonLdProcessorTest, expand_t0027) {
    performExpandTestFromManifest("#t0027");
}

TEST(JsonLdProcessorTest, expand_t0028) {
    performExpandTestFromManifest("#t0028");
}

TEST(JsonLdProcessorTest, expand_t0029) {
    performExpandTestFromManifest("#t0029");
}

TEST(JsonLdProcessorTest, expand_t0030) {
    performExpandTestFromManifest("#t0030");
}

TEST(JsonLdProcessorTest, expand_t0031) {
    performExpandTestFromManifest("#t0031");
}

TEST(JsonLdProcessorTest, expand_t0032) {
    performExpandTestFromManifest("#t0032");
}

TEST(JsonLdProcessorTest, expand_t0033) {
    performExpandTestFromManifest("#t0033");
}

TEST(JsonLdProcessorTest, expand_t0034) {
    performExpandTestFromManifest("#t0034");
}

TEST(JsonLdProcessorTest, expand_t0035) {
    performExpandTestFromManifest("#t0035");
}

TEST(JsonLdProcessorTest, expand_t0036) {
    performExpandTestFromManifest("#t0036");
}

TEST(JsonLdProcessorTest, expand_t0037) {
    performExpandTestFromManifest("#t0037");
}

TEST(JsonLdProcessorTest, expand_t0038) {
    performExpandTestFromManifest("#t0038");
}

TEST(JsonLdProcessorTest, expand_t0039) {
    performExpandTestFromManifest("#t0039");
}

TEST(JsonLdProcessorTest, expand_t0040) {
    performExpandTestFromManifest("#t0040");
}

TEST(JsonLdProcessorTest, expand_t0041) {
    performExpandTestFromManifest("#t0041");
}

TEST(JsonLdProcessorTest, expand_t0042) {
    performExpandTestFromManifest("#t0042");
}

TEST(JsonLdProcessorTest, expand_t0043) {
    performExpandTestFromManifest("#t0043");
}

TEST(JsonLdProcessorTest, expand_t0044) {
    performExpandTestFromManifest("#t0044");
}

TEST(JsonLdProcessorTest, expand_t0045) {
    performExpandTestFromManifest("#t0045");
}

TEST(JsonLdProcessorTest, expand_t0046) {
    performExpandTestFromManifest("#t0046");
}

TEST(JsonLdProcessorTest, expand_t0047) {
    performExpandTestFromManifest("#t0047");
}

TEST(JsonLdProcessorTest, expand_t0048) {
    performExpandTestFromManifest("#t0048");
}

TEST(JsonLdProcessorTest, expand_t0049) {
    performExpandTestFromManifest("#t0049");
}

TEST(JsonLdProcessorTest, expand_t0050) {
    performExpandTestFromManifest("#t0050");
}

TEST(JsonLdProcessorTest, expand_t0051) {
    performExpandTestFromManifest("#t0051");
}

TEST(JsonLdProcessorTest, expand_t0052) {
    performExpandTestFromManifest("#t0052");
}

TEST(JsonLdProcessorTest, expand_t0053) {
    performExpandTestFromManifest("#t0053");
}

TEST(JsonLdProcessorTest, expand_t0054) {
    performExpandTestFromManifest("#t0054");
}

TEST(JsonLdProcessorTest, expand_t0055) {
    performExpandTestFromManifest("#t0055");
}

TEST(JsonLdProcessorTest, expand_t0056) {
    performExpandTestFromManifest("#t0056");
}

TEST(JsonLdProcessorTest, expand_t0057) {
    performExpandTestFromManifest("#t0057");
}

TEST(JsonLdProcessorTest, expand_t0058) {
    performExpandTestFromManifest("#t0058");
}

TEST(JsonLdProcessorTest, expand_t0059) {
    performExpandTestFromManifest("#t0059");
}

TEST(JsonLdProcessorTest, expand_t0060) {
    performExpandTestFromManifest("#t0060");
}

TEST(JsonLdProcessorTest, expand_t0061) {
    performExpandTestFromManifest("#t0061");
}

TEST(JsonLdProcessorTest, expand_t0062) {
    performExpandTestFromManifest("#t0062");
}

TEST(JsonLdProcessorTest, expand_t0063) {
    performExpandTestFromManifest("#t0063");
}

TEST(JsonLdProcessorTest, expand_t0064) {
    performExpandTestFromManifest("#t0064");
}

TEST(JsonLdProcessorTest, expand_t0065) {
    performExpandTestFromManifest("#t0065");
}

TEST(JsonLdProcessorTest, expand_t0066) {
    performExpandTestFromManifest("#t0066");
}

TEST(JsonLdProcessorTest, expand_t0067) {
    performExpandTestFromManifest("#t0067");
}

TEST(JsonLdProcessorTest, expand_t0068) {
    performExpandTestFromManifest("#t0068");
}

TEST(JsonLdProcessorTest, expand_t0069) {
    performExpandTestFromManifest("#t0069");
}

TEST(JsonLdProcessorTest, expand_t0070) {
    performExpandTestFromManifest("#t0070");
}

TEST(JsonLdProcessorTest, expand_t0071) {
    performExpandTestFromManifest("#t0071");
}

TEST(JsonLdProcessorTest, expand_t0072) {
    performExpandTestFromManifest("#t0072");
}

TEST(JsonLdProcessorTest, expand_t0073) {
    performExpandTestFromManifest("#t0073");
}

TEST(JsonLdProcessorTest, expand_t0074) {
    performExpandTestFromManifest("#t0074");
}

TEST(JsonLdProcessorTest, expand_t0075) {
    performExpandTestFromManifest("#t0075");
}

TEST(JsonLdProcessorTest, expand_t0076) {
    performExpandTestFromManifest("#t0076");
}

// Disabled this test because we need to implement expandContext option
//TEST(JsonLdProcessorTest, expand_0077) {
//    performExpandTestFromManifest("#t0077");
//}

TEST(JsonLdProcessorTest, expand_t0078) {
    performExpandTestFromManifest("#t0078");
}

TEST(JsonLdProcessorTest, expand_t0079) {
    performExpandTestFromManifest("#t0079");
}

TEST(JsonLdProcessorTest, expand_t0080) {
    performExpandTestFromManifest("#t0080");
}

TEST(JsonLdProcessorTest, expand_t0081) {
    performExpandTestFromManifest("#t0081");
}

TEST(JsonLdProcessorTest, expand_t0082) {
    performExpandTestFromManifest("#t0082");
}

TEST(JsonLdProcessorTest, expand_t0083) {
    performExpandTestFromManifest("#t0083");
}

TEST(JsonLdProcessorTest, expand_t0084) {
    performExpandTestFromManifest("#t0084");
}

TEST(JsonLdProcessorTest, expand_t0085) {
    performExpandTestFromManifest("#t0085");
}

TEST(JsonLdProcessorTest, expand_t0086) {
    performExpandTestFromManifest("#t0086");
}

TEST(JsonLdProcessorTest, expand_t0087) {
    performExpandTestFromManifest("#t0087");
}

TEST(JsonLdProcessorTest, expand_t0088) {
    performExpandTestFromManifest("#t0088");
}

TEST(JsonLdProcessorTest, expand_t0089) {
    performExpandTestFromManifest("#t0089");
}

TEST(JsonLdProcessorTest, expand_t0090) {
    performExpandTestFromManifest("#t0090");
}

TEST(JsonLdProcessorTest, expand_t0091) {
    performExpandTestFromManifest("#t0091");
}

TEST(JsonLdProcessorTest, expand_t0092) {
    performExpandTestFromManifest("#t0092");
}

TEST(JsonLdProcessorTest, expand_t0093) {
    performExpandTestFromManifest("#t0093");
}

TEST(JsonLdProcessorTest, expand_t0094) {
    performExpandTestFromManifest("#t0094");
}

TEST(JsonLdProcessorTest, expand_t0095) {
    performExpandTestFromManifest("#t0095");
}

TEST(JsonLdProcessorTest, expand_t0096) {
    performExpandTestFromManifest("#t0096");
}

TEST(JsonLdProcessorTest, expand_t0097) {
    performExpandTestFromManifest("#t0097");
}

TEST(JsonLdProcessorTest, expand_t0098) {
    performExpandTestFromManifest("#t0098");
}

TEST(JsonLdProcessorTest, expand_t0099) {
    performExpandTestFromManifest("#t0099");
}

TEST(JsonLdProcessorTest, expand_t0100) {
    performExpandTestFromManifest("#t0100");
}

TEST(JsonLdProcessorTest, expand_t0101) {
    performExpandTestFromManifest("#t0101");
}

TEST(JsonLdProcessorTest, expand_t0102) {
    performExpandTestFromManifest("#t0102");
}

TEST(JsonLdProcessorTest, expand_t0103) {
    performExpandTestFromManifest("#t0103");
}

TEST(JsonLdProcessorTest, expand_t0104) {
    performExpandTestFromManifest("#t0104");
}

TEST(JsonLdProcessorTest, expand_t0105) {
    performExpandTestFromManifest("#t0105");
}

TEST(JsonLdProcessorTest, expand_t0106) {
    performExpandTestFromManifest("#t0106");
}

TEST(JsonLdProcessorTest, expand_t0107) {
    performExpandTestFromManifest("#t0107");
}

TEST(JsonLdProcessorTest, expand_t0108) {
    performExpandTestFromManifest("#t0108");
}

TEST(JsonLdProcessorTest, expand_t0109) {
    performExpandTestFromManifest("#t0109");
}

TEST(JsonLdProcessorTest, expand_t0110) {
    performExpandTestFromManifest("#t0110");
}

TEST(JsonLdProcessorTest, expand_t0111) {
    performExpandTestFromManifest("#t0111");
}

TEST(JsonLdProcessorTest, expand_t0112) {
    performExpandTestFromManifest("#t0112");
}

TEST(JsonLdProcessorTest, expand_t0113) {
    performExpandTestFromManifest("#t0113");
}

TEST(JsonLdProcessorTest, expand_t0114) {
    performExpandTestFromManifest("#t0114");
}

TEST(JsonLdProcessorTest, expand_t0115) {
    performExpandTestFromManifest("#t0115");
}

TEST(JsonLdProcessorTest, expand_t0116) {
    performExpandTestFromManifest("#t0116");
}

TEST(JsonLdProcessorTest, expand_t0117) {
    performExpandTestFromManifest("#t0117");
}

TEST(JsonLdProcessorTest, expand_t0118) {
    performExpandTestFromManifest("#t0118");
}

TEST(JsonLdProcessorTest, expand_t0119) {
    performExpandTestFromManifest("#t0119");
}

TEST(JsonLdProcessorTest, expand_t0120) {
    performExpandTestFromManifest("#t0120");
}

TEST(JsonLdProcessorTest, expand_t0121) {
    performExpandTestFromManifest("#t0121");
}

TEST(JsonLdProcessorTest, expand_t0122) {
    performExpandTestFromManifest("#t0122");
}

TEST(JsonLdProcessorTest, expand_t0123) {
    performExpandTestFromManifest("#t0123");
}

TEST(JsonLdProcessorTest, expand_t0124) {
    performExpandTestFromManifest("#t0124");
}

TEST(JsonLdProcessorTest, expand_t0125) {
    performExpandTestFromManifest("#t0125");
}

TEST(JsonLdProcessorTest, expand_t0126) {
    performExpandTestFromManifest("#t0126");
}

TEST(JsonLdProcessorTest, expand_t0127) {
    performExpandTestFromManifest("#t0127");
}

TEST(JsonLdProcessorTest, expand_t0128) {
    performExpandTestFromManifest("#t0128");
}

TEST(JsonLdProcessorTest, expand_t0129) {
    performExpandTestFromManifest("#t0129");
}

TEST(JsonLdProcessorTest, expand_t0130) {
    performExpandTestFromManifest("#t0130");
}

TEST(JsonLdProcessorTest, expand_tc001) {
    performExpandTestFromManifest("#tc001");
}

TEST(JsonLdProcessorTest, expand_tc002) {
    performExpandTestFromManifest("#tc002");
}

TEST(JsonLdProcessorTest, expand_tc003) {
    performExpandTestFromManifest("#tc003");
}

TEST(JsonLdProcessorTest, expand_tc004) {
    performExpandTestFromManifest("#tc004");
}

TEST(JsonLdProcessorTest, expand_tc005) {
    performExpandTestFromManifest("#tc005");
}

TEST(JsonLdProcessorTest, expand_tc006) {
    performExpandTestFromManifest("#tc006");
}

TEST(JsonLdProcessorTest, expand_tc007) {
    performExpandTestFromManifest("#tc007");
}

TEST(JsonLdProcessorTest, expand_tc008) {
    performExpandTestFromManifest("#tc008");
}

TEST(JsonLdProcessorTest, expand_tc009) {
    performExpandTestFromManifest("#tc009");
}

TEST(JsonLdProcessorTest, expand_tc010) {
    performExpandTestFromManifest("#tc010");
}

TEST(JsonLdProcessorTest, expand_tc011) {
    performExpandTestFromManifest("#tc011");
}

TEST(JsonLdProcessorTest, expand_tc012) {
    performExpandTestFromManifest("#tc012");
}

TEST(JsonLdProcessorTest, expand_tc013) {
    performExpandTestFromManifest("#tc013");
}

TEST(JsonLdProcessorTest, expand_tc014) {
    performExpandTestFromManifest("#tc014");
}

TEST(JsonLdProcessorTest, expand_tc015) {
    performExpandTestFromManifest("#tc015");
}

TEST(JsonLdProcessorTest, expand_tc016) {
    performExpandTestFromManifest("#tc016");
}

TEST(JsonLdProcessorTest, expand_tc017) {
    performExpandTestFromManifest("#tc017");
}

TEST(JsonLdProcessorTest, expand_tc018) {
    performExpandTestFromManifest("#tc018");
}

TEST(JsonLdProcessorTest, expand_tc019) {
    performExpandTestFromManifest("#tc019");
}

TEST(JsonLdProcessorTest, expand_tc020) {
    performExpandTestFromManifest("#tc020");
}

TEST(JsonLdProcessorTest, expand_tc021) {
    performExpandTestFromManifest("#tc021");
}

TEST(JsonLdProcessorTest, expand_tc022) {
    performExpandTestFromManifest("#tc022");
}

TEST(JsonLdProcessorTest, expand_tc023) {
    performExpandTestFromManifest("#tc023");
}

TEST(JsonLdProcessorTest, expand_tc024) {
    performExpandTestFromManifest("#tc024");
}

TEST(JsonLdProcessorTest, expand_tc025) {
    performExpandTestFromManifest("#tc025");
}

TEST(JsonLdProcessorTest, expand_tc026) {
    performExpandTestFromManifest("#tc026");
}

TEST(JsonLdProcessorTest, expand_tc027) {
    performExpandTestFromManifest("#tc027");
}

TEST(JsonLdProcessorTest, expand_tc028) {
    performExpandTestFromManifest("#tc028");
}

TEST(JsonLdProcessorTest, expand_tc029) {
    performExpandTestFromManifest("#tc029");
}

TEST(JsonLdProcessorTest, expand_tc030) {
    performExpandTestFromManifest("#tc030");
}

TEST(JsonLdProcessorTest, expand_tc031) {
    performExpandTestFromManifest("#tc031");
}

TEST(JsonLdProcessorTest, expand_tc032) {
    performExpandTestFromManifest("#tc032");
}

TEST(JsonLdProcessorTest, expand_tc033) {
    performExpandTestFromManifest("#tc033");
}

TEST(JsonLdProcessorTest, expand_tc034) {
    performExpandTestFromManifest("#tc034");
}

TEST(JsonLdProcessorTest, expand_tc035) {
    performExpandTestFromManifest("#tc035");
}

TEST(JsonLdProcessorTest, expand_tc036) {
    performExpandTestFromManifest("#tc036");
}

TEST(JsonLdProcessorTest, expand_tdi01) {
    performExpandTestFromManifest("#tdi01");
}

TEST(JsonLdProcessorTest, expand_tdi02) {
    performExpandTestFromManifest("#tdi02");
}

TEST(JsonLdProcessorTest, expand_tdi03) {
    performExpandTestFromManifest("#tdi03");
}

TEST(JsonLdProcessorTest, expand_tdi04) {
    performExpandTestFromManifest("#tdi04");
}

TEST(JsonLdProcessorTest, expand_tdi05) {
    performExpandTestFromManifest("#tdi05");
}

TEST(JsonLdProcessorTest, expand_tdi06) {
    performExpandTestFromManifest("#tdi06");
}

TEST(JsonLdProcessorTest, expand_tdi07) {
    performExpandTestFromManifest("#tdi07");
}

TEST(JsonLdProcessorTest, expand_tdi08) {
    performExpandTestFromManifest("#tdi08");
}

TEST(JsonLdProcessorTest, expand_tdi09) {
    performExpandTestFromManifest("#tdi09");
}

TEST(JsonLdProcessorTest, expand_tec01) {
    performExpandTestFromManifest("#tec01");
}

TEST(JsonLdProcessorTest, expand_tec02) {
    performExpandTestFromManifest("#tec02");
}

TEST(JsonLdProcessorTest, expand_tem01) {
    performExpandTestFromManifest("#tem01");
}

TEST(JsonLdProcessorTest, expand_ten01) {
    performExpandTestFromManifest("#ten01");
}

TEST(JsonLdProcessorTest, expand_ten02) {
    performExpandTestFromManifest("#ten02");
}

TEST(JsonLdProcessorTest, expand_ten03) {
    performExpandTestFromManifest("#ten03");
}

TEST(JsonLdProcessorTest, expand_ten04) {
    performExpandTestFromManifest("#ten04");
}

TEST(JsonLdProcessorTest, expand_ten05) {
    performExpandTestFromManifest("#ten05");
}

TEST(JsonLdProcessorTest, expand_ten06) {
    performExpandTestFromManifest("#ten06");
}

TEST(JsonLdProcessorTest, expand_tep02) {
    performExpandTestFromManifest("#tep02");
}

TEST(JsonLdProcessorTest, expand_tep03) {
    performExpandTestFromManifest("#tep03");
}

TEST(JsonLdProcessorTest, expand_ter01) {
    performExpandTestFromManifest("#ter01");
}

TEST(JsonLdProcessorTest, expand_ter02) {
    performExpandTestFromManifest("#ter02");
}

TEST(JsonLdProcessorTest, expand_ter03) {
    performExpandTestFromManifest("#ter03");
}

TEST(JsonLdProcessorTest, expand_ter04) {
    performExpandTestFromManifest("#ter04");
}

TEST(JsonLdProcessorTest, expand_ter05) {
    performExpandTestFromManifest("#ter05");
}

TEST(JsonLdProcessorTest, expand_ter06) {
    performExpandTestFromManifest("#ter06");
}

TEST(JsonLdProcessorTest, expand_ter07) {
    performExpandTestFromManifest("#ter07");
}

TEST(JsonLdProcessorTest, expand_ter08) {
    performExpandTestFromManifest("#ter08");
}

TEST(JsonLdProcessorTest, expand_ter09) {
    performExpandTestFromManifest("#ter09");
}

TEST(JsonLdProcessorTest, expand_ter10) {
    performExpandTestFromManifest("#ter10");
}

TEST(JsonLdProcessorTest, expand_ter11) {
    performExpandTestFromManifest("#ter11");
}

TEST(JsonLdProcessorTest, expand_ter12) {
    performExpandTestFromManifest("#ter12");
}

TEST(JsonLdProcessorTest, expand_ter13) {
    performExpandTestFromManifest("#ter13");
}

TEST(JsonLdProcessorTest, expand_ter14) {
    performExpandTestFromManifest("#ter14");
}

TEST(JsonLdProcessorTest, expand_ter15) {
    performExpandTestFromManifest("#ter15");
}

TEST(JsonLdProcessorTest, expand_ter17) {
    performExpandTestFromManifest("#ter17");
}

TEST(JsonLdProcessorTest, expand_ter18) {
    performExpandTestFromManifest("#ter18");
}

TEST(JsonLdProcessorTest, expand_ter19) {
    performExpandTestFromManifest("#ter19");
}

TEST(JsonLdProcessorTest, expand_ter20) {
    performExpandTestFromManifest("#ter20");
}

TEST(JsonLdProcessorTest, expand_ter21) {
    performExpandTestFromManifest("#ter21");
}

TEST(JsonLdProcessorTest, expand_ter22) {
    performExpandTestFromManifest("#ter22");
}

TEST(JsonLdProcessorTest, expand_ter23) {
    performExpandTestFromManifest("#ter23");
}

TEST(JsonLdProcessorTest, expand_ter24) {
    performExpandTestFromManifest("#ter24");
}

TEST(JsonLdProcessorTest, expand_ter25) {
    performExpandTestFromManifest("#ter25");
}

TEST(JsonLdProcessorTest, expand_ter26) {
    performExpandTestFromManifest("#ter26");
}

TEST(JsonLdProcessorTest, expand_ter27) {
    performExpandTestFromManifest("#ter27");
}

TEST(JsonLdProcessorTest, expand_ter28) {
    performExpandTestFromManifest("#ter28");
}

TEST(JsonLdProcessorTest, expand_ter29) {
    performExpandTestFromManifest("#ter29");
}

TEST(JsonLdProcessorTest, expand_ter30) {
    performExpandTestFromManifest("#ter30");
}

TEST(JsonLdProcessorTest, expand_ter31) {
    performExpandTestFromManifest("#ter31");
}

TEST(JsonLdProcessorTest, expand_ter32) {
    performExpandTestFromManifest("#ter32");
}

TEST(JsonLdProcessorTest, expand_ter33) {
    performExpandTestFromManifest("#ter33");
}

TEST(JsonLdProcessorTest, expand_ter34) {
    performExpandTestFromManifest("#ter34");
}

TEST(JsonLdProcessorTest, expand_ter35) {
    performExpandTestFromManifest("#ter35");
}

TEST(JsonLdProcessorTest, expand_ter36) {
    performExpandTestFromManifest("#ter36");
}

TEST(JsonLdProcessorTest, expand_ter37) {
    performExpandTestFromManifest("#ter37");
}

TEST(JsonLdProcessorTest, expand_ter38) {
    performExpandTestFromManifest("#ter38");
}

TEST(JsonLdProcessorTest, expand_ter39) {
    performExpandTestFromManifest("#ter39");
}

TEST(JsonLdProcessorTest, expand_ter40) {
    performExpandTestFromManifest("#ter40");
}

TEST(JsonLdProcessorTest, expand_ter41) {
    performExpandTestFromManifest("#ter41");
}

TEST(JsonLdProcessorTest, expand_ter42) {
    performExpandTestFromManifest("#ter42");
}

TEST(JsonLdProcessorTest, expand_ter43) {
    performExpandTestFromManifest("#ter43");
}

TEST(JsonLdProcessorTest, expand_ter44) {
    performExpandTestFromManifest("#ter44");
}

TEST(JsonLdProcessorTest, expand_ter48) {
    performExpandTestFromManifest("#ter48");
}

TEST(JsonLdProcessorTest, expand_ter49) {
    performExpandTestFromManifest("#ter49");
}

TEST(JsonLdProcessorTest, expand_ter50) {
    performExpandTestFromManifest("#ter50");
}

TEST(JsonLdProcessorTest, expand_ter51) {
    performExpandTestFromManifest("#ter51");
}

TEST(JsonLdProcessorTest, expand_ter52) {
    performExpandTestFromManifest("#ter52");
}

TEST(JsonLdProcessorTest, expand_ter53) {
    performExpandTestFromManifest("#ter53");
}

TEST(JsonLdProcessorTest, expand_ter54) {
    performExpandTestFromManifest("#ter54");
}

TEST(JsonLdProcessorTest, expand_ter55) {
    performExpandTestFromManifest("#ter55");
}

TEST(JsonLdProcessorTest, expand_tes01) {
    performExpandTestFromManifest("#tes01");
}

TEST(JsonLdProcessorTest, expand_tes02) {
    performExpandTestFromManifest("#tes02");
}

TEST(JsonLdProcessorTest, expand_tin01) {
    performExpandTestFromManifest("#tin01");
}

TEST(JsonLdProcessorTest, expand_tin02) {
    performExpandTestFromManifest("#tin02");
}

TEST(JsonLdProcessorTest, expand_tin03) {
    performExpandTestFromManifest("#tin03");
}

TEST(JsonLdProcessorTest, expand_tin04) {
    performExpandTestFromManifest("#tin04");
}

TEST(JsonLdProcessorTest, expand_tin05) {
    performExpandTestFromManifest("#tin05");
}

TEST(JsonLdProcessorTest, expand_tin06) {
    performExpandTestFromManifest("#tin06");
}

TEST(JsonLdProcessorTest, expand_tin07) {
    performExpandTestFromManifest("#tin07");
}

TEST(JsonLdProcessorTest, expand_tin08) {
    performExpandTestFromManifest("#tin08");
}

TEST(JsonLdProcessorTest, expand_tin09) {
    performExpandTestFromManifest("#tin09");
}

TEST(JsonLdProcessorTest, expand_tjs01) {
    performExpandTestFromManifest("#tjs01");
}

TEST(JsonLdProcessorTest, expand_tjs02) {
    performExpandTestFromManifest("#tjs02");
}

TEST(JsonLdProcessorTest, expand_tjs03) {
    performExpandTestFromManifest("#tjs03");
}

TEST(JsonLdProcessorTest, expand_tjs04) {
    performExpandTestFromManifest("#tjs04");
}

TEST(JsonLdProcessorTest, expand_tjs05) {
    performExpandTestFromManifest("#tjs05");
}

TEST(JsonLdProcessorTest, expand_tjs06) {
    performExpandTestFromManifest("#tjs06");
}

TEST(JsonLdProcessorTest, expand_tjs07) {
    performExpandTestFromManifest("#tjs07");
}

TEST(JsonLdProcessorTest, expand_tjs08) {
    performExpandTestFromManifest("#tjs08");
}

TEST(JsonLdProcessorTest, expand_tjs09) {
    performExpandTestFromManifest("#tjs09");
}

TEST(JsonLdProcessorTest, expand_tjs10) {
    performExpandTestFromManifest("#tjs10");
}

TEST(JsonLdProcessorTest, expand_tjs11) {
    performExpandTestFromManifest("#tjs11");
}

TEST(JsonLdProcessorTest, expand_tjs12) {
    performExpandTestFromManifest("#tjs12");
}

TEST(JsonLdProcessorTest, expand_tjs13) {
    performExpandTestFromManifest("#tjs13");
}

TEST(JsonLdProcessorTest, expand_tjs14) {
    performExpandTestFromManifest("#tjs14");
}

TEST(JsonLdProcessorTest, expand_tjs15) {
    performExpandTestFromManifest("#tjs15");
}

TEST(JsonLdProcessorTest, expand_tjs16) {
    performExpandTestFromManifest("#tjs16");
}

TEST(JsonLdProcessorTest, expand_tjs17) {
    performExpandTestFromManifest("#tjs17");
}

TEST(JsonLdProcessorTest, expand_tjs18) {
    performExpandTestFromManifest("#tjs18");
}

TEST(JsonLdProcessorTest, expand_tjs19) {
    performExpandTestFromManifest("#tjs19");
}

TEST(JsonLdProcessorTest, expand_tjs20) {
    performExpandTestFromManifest("#tjs20");
}

TEST(JsonLdProcessorTest, expand_tjs21) {
    performExpandTestFromManifest("#tjs21");
}

TEST(JsonLdProcessorTest, expand_tjs22) {
    performExpandTestFromManifest("#tjs22");
}

TEST(JsonLdProcessorTest, expand_tjs23) {
    performExpandTestFromManifest("#tjs23");
}

TEST(JsonLdProcessorTest, expand_tl001) {
    performExpandTestFromManifest("#tl001");
}

TEST(JsonLdProcessorTest, expand_tli01) {
    performExpandTestFromManifest("#tli01");
}

TEST(JsonLdProcessorTest, expand_tli02) {
    performExpandTestFromManifest("#tli02");
}

TEST(JsonLdProcessorTest, expand_tli03) {
    performExpandTestFromManifest("#tli03");
}

TEST(JsonLdProcessorTest, expand_tli04) {
    performExpandTestFromManifest("#tli04");
}

TEST(JsonLdProcessorTest, expand_tli05) {
    performExpandTestFromManifest("#tli05");
}

TEST(JsonLdProcessorTest, expand_tli06) {
    performExpandTestFromManifest("#tli06");
}

TEST(JsonLdProcessorTest, expand_tli07) {
    performExpandTestFromManifest("#tli07");
}

TEST(JsonLdProcessorTest, expand_tli08) {
    performExpandTestFromManifest("#tli08");
}

TEST(JsonLdProcessorTest, expand_tli09) {
    performExpandTestFromManifest("#tli09");
}

TEST(JsonLdProcessorTest, expand_tli10) {
    performExpandTestFromManifest("#tli10");
}

TEST(JsonLdProcessorTest, expand_tm001) {
    performExpandTestFromManifest("#tm001");
}

TEST(JsonLdProcessorTest, expand_tm002) {
    performExpandTestFromManifest("#tm002");
}

TEST(JsonLdProcessorTest, expand_tm003) {
    performExpandTestFromManifest("#tm003");
}

TEST(JsonLdProcessorTest, expand_tm004) {
    performExpandTestFromManifest("#tm004");
}

TEST(JsonLdProcessorTest, expand_tm005) {
    performExpandTestFromManifest("#tm005");
}

TEST(JsonLdProcessorTest, expand_tm006) {
    performExpandTestFromManifest("#tm006");
}

TEST(JsonLdProcessorTest, expand_tm007) {
    performExpandTestFromManifest("#tm007");
}

TEST(JsonLdProcessorTest, expand_tm008) {
    performExpandTestFromManifest("#tm008");
}

TEST(JsonLdProcessorTest, expand_tm009) {
    performExpandTestFromManifest("#tm009");
}

TEST(JsonLdProcessorTest, expand_tm010) {
    performExpandTestFromManifest("#tm010");
}

TEST(JsonLdProcessorTest, expand_tm011) {
    performExpandTestFromManifest("#tm011");
}

TEST(JsonLdProcessorTest, expand_tm012) {
    performExpandTestFromManifest("#tm012");
}

TEST(JsonLdProcessorTest, expand_tm013) {
    performExpandTestFromManifest("#tm013");
}

TEST(JsonLdProcessorTest, expand_tm014) {
    performExpandTestFromManifest("#tm014");
}

TEST(JsonLdProcessorTest, expand_tm015) {
    performExpandTestFromManifest("#tm015");
}

TEST(JsonLdProcessorTest, expand_tm016) {
    performExpandTestFromManifest("#tm016");
}

TEST(JsonLdProcessorTest, expand_tm017) {
    performExpandTestFromManifest("#tm017");
}

TEST(JsonLdProcessorTest, expand_tm018) {
    performExpandTestFromManifest("#tm018");
}

TEST(JsonLdProcessorTest, expand_tm019) {
    performExpandTestFromManifest("#tm019");
}

TEST(JsonLdProcessorTest, expand_tm020) {
    performExpandTestFromManifest("#tm020");
}

TEST(JsonLdProcessorTest, expand_tn001) {
    performExpandTestFromManifest("#tn001");
}

TEST(JsonLdProcessorTest, expand_tn002) {
    performExpandTestFromManifest("#tn002");
}

TEST(JsonLdProcessorTest, expand_tn003) {
    performExpandTestFromManifest("#tn003");
}

TEST(JsonLdProcessorTest, expand_tn004) {
    performExpandTestFromManifest("#tn004");
}

TEST(JsonLdProcessorTest, expand_tn005) {
    performExpandTestFromManifest("#tn005");
}

TEST(JsonLdProcessorTest, expand_tn006) {
    performExpandTestFromManifest("#tn006");
}

TEST(JsonLdProcessorTest, expand_tn007) {
    performExpandTestFromManifest("#tn007");
}

TEST(JsonLdProcessorTest, expand_tn008) {
    performExpandTestFromManifest("#tn008");
}

TEST(JsonLdProcessorTest, expand_tp001) {
    performExpandTestFromManifest("#tp001");
}

TEST(JsonLdProcessorTest, expand_tp002) {
    performExpandTestFromManifest("#tp002");
}

TEST(JsonLdProcessorTest, expand_tp003) {
    performExpandTestFromManifest("#tp003");
}

TEST(JsonLdProcessorTest, expand_tp004) {
    performExpandTestFromManifest("#tp004");
}

TEST(JsonLdProcessorTest, expand_tpi01) {
    performExpandTestFromManifest("#tpi01");
}

TEST(JsonLdProcessorTest, expand_tpi02) {
    performExpandTestFromManifest("#tpi02");
}

TEST(JsonLdProcessorTest, expand_tpi03) {
    performExpandTestFromManifest("#tpi03");
}

TEST(JsonLdProcessorTest, expand_tpi04) {
    performExpandTestFromManifest("#tpi04");
}

TEST(JsonLdProcessorTest, expand_tpi05) {
    performExpandTestFromManifest("#tpi05");
}

TEST(JsonLdProcessorTest, expand_tpi06) {
    performExpandTestFromManifest("#tpi06");
}

TEST(JsonLdProcessorTest, expand_tpi07) {
    performExpandTestFromManifest("#tpi07");
}

TEST(JsonLdProcessorTest, expand_tpi08) {
    performExpandTestFromManifest("#tpi08");
}

TEST(JsonLdProcessorTest, expand_tpi09) {
    performExpandTestFromManifest("#tpi09");
}

TEST(JsonLdProcessorTest, expand_tpi10) {
    performExpandTestFromManifest("#tpi10");
}

TEST(JsonLdProcessorTest, expand_tpi11) {
    performExpandTestFromManifest("#tpi11");
}

TEST(JsonLdProcessorTest, expand_tpr01) {
    performExpandTestFromManifest("#tpr01");
}

TEST(JsonLdProcessorTest, expand_tpr02) {
    performExpandTestFromManifest("#tpr02");
}

TEST(JsonLdProcessorTest, expand_tpr03) {
    performExpandTestFromManifest("#tpr03");
}

TEST(JsonLdProcessorTest, expand_tpr04) {
    performExpandTestFromManifest("#tpr04");
}

TEST(JsonLdProcessorTest, expand_tpr05) {
    performExpandTestFromManifest("#tpr05");
}

TEST(JsonLdProcessorTest, expand_tpr06) {
    performExpandTestFromManifest("#tpr06");
}

TEST(JsonLdProcessorTest, expand_tpr08) {
    performExpandTestFromManifest("#tpr08");
}

TEST(JsonLdProcessorTest, expand_tpr09) {
    performExpandTestFromManifest("#tpr09");
}

TEST(JsonLdProcessorTest, expand_tpr10) {
    performExpandTestFromManifest("#tpr10");
}

TEST(JsonLdProcessorTest, expand_tpr11) {
    performExpandTestFromManifest("#tpr11");
}

TEST(JsonLdProcessorTest, expand_tpr12) {
    performExpandTestFromManifest("#tpr12");
}

TEST(JsonLdProcessorTest, expand_tpr13) {
    performExpandTestFromManifest("#tpr13");
}

TEST(JsonLdProcessorTest, expand_tpr14) {
    performExpandTestFromManifest("#tpr14");
}

TEST(JsonLdProcessorTest, expand_tpr15) {
    performExpandTestFromManifest("#tpr15");
}

TEST(JsonLdProcessorTest, expand_tpr16) {
    performExpandTestFromManifest("#tpr16");
}

TEST(JsonLdProcessorTest, expand_tpr17) {
    performExpandTestFromManifest("#tpr17");
}

TEST(JsonLdProcessorTest, expand_tpr18) {
    performExpandTestFromManifest("#tpr18");
}

TEST(JsonLdProcessorTest, expand_tpr19) {
    performExpandTestFromManifest("#tpr19");
}

TEST(JsonLdProcessorTest, expand_tpr20) {
    performExpandTestFromManifest("#tpr20");
}

TEST(JsonLdProcessorTest, expand_tpr21) {
    performExpandTestFromManifest("#tpr21");
}

TEST(JsonLdProcessorTest, expand_tpr22) {
    performExpandTestFromManifest("#tpr22");
}

TEST(JsonLdProcessorTest, expand_tpr23) {
    performExpandTestFromManifest("#tpr23");
}

TEST(JsonLdProcessorTest, expand_tpr24) {
    performExpandTestFromManifest("#tpr24");
}

TEST(JsonLdProcessorTest, expand_tpr25) {
    performExpandTestFromManifest("#tpr25");
}

TEST(JsonLdProcessorTest, expand_tpr26) {
    performExpandTestFromManifest("#tpr26");
}

TEST(JsonLdProcessorTest, expand_tpr27) {
    performExpandTestFromManifest("#tpr27");
}

TEST(JsonLdProcessorTest, expand_tpr28) {
    performExpandTestFromManifest("#tpr28");
}

TEST(JsonLdProcessorTest, expand_tpr29) {
    performExpandTestFromManifest("#tpr29");
}

TEST(JsonLdProcessorTest, expand_tpr30) {
    performExpandTestFromManifest("#tpr30");
}

TEST(JsonLdProcessorTest, expand_tpr31) {
    performExpandTestFromManifest("#tpr31");
}

TEST(JsonLdProcessorTest, expand_tpr32) {
    performExpandTestFromManifest("#tpr32");
}

TEST(JsonLdProcessorTest, expand_tpr33) {
    performExpandTestFromManifest("#tpr33");
}

TEST(JsonLdProcessorTest, expand_tpr34) {
    performExpandTestFromManifest("#tpr34");
}

TEST(JsonLdProcessorTest, expand_tpr35) {
    performExpandTestFromManifest("#tpr35");
}

TEST(JsonLdProcessorTest, expand_tpr36) {
    performExpandTestFromManifest("#tpr36");
}

TEST(JsonLdProcessorTest, expand_tpr37) {
    performExpandTestFromManifest("#tpr37");
}

TEST(JsonLdProcessorTest, expand_tpr38) {
    performExpandTestFromManifest("#tpr38");
}

TEST(JsonLdProcessorTest, expand_tpr39) {
    performExpandTestFromManifest("#tpr39");
}

TEST(JsonLdProcessorTest, expand_tpr40) {
    performExpandTestFromManifest("#tpr40");
}

TEST(JsonLdProcessorTest, expand_tso01) {
    performExpandTestFromManifest("#tso01");
}

TEST(JsonLdProcessorTest, expand_tso02) {
    performExpandTestFromManifest("#tso02");
}

TEST(JsonLdProcessorTest, expand_tso03) {
    performExpandTestFromManifest("#tso03");
}

TEST(JsonLdProcessorTest, expand_tso05) {
    performExpandTestFromManifest("#tso05");
}

TEST(JsonLdProcessorTest, expand_tso06) {
    performExpandTestFromManifest("#tso06");
}

TEST(JsonLdProcessorTest, expand_tso07) {
    performExpandTestFromManifest("#tso07");
}

TEST(JsonLdProcessorTest, expand_tso08) {
    performExpandTestFromManifest("#tso08");
}

TEST(JsonLdProcessorTest, expand_tso09) {
    performExpandTestFromManifest("#tso09");
}

TEST(JsonLdProcessorTest, expand_tso10) {
    performExpandTestFromManifest("#tso10");
}

TEST(JsonLdProcessorTest, expand_tso11) {
    performExpandTestFromManifest("#tso11");
}

TEST(JsonLdProcessorTest, expand_tso12) {
    performExpandTestFromManifest("#tso12");
}

TEST(JsonLdProcessorTest, expand_tso13) {
    performExpandTestFromManifest("#tso13");
}

TEST(JsonLdProcessorTest, expand_ttn01) {
    performExpandTestFromManifest("#ttn01");
}

TEST(JsonLdProcessorTest, expand_ttn02) {
    performExpandTestFromManifest("#ttn02");
}

TEST(JsonLdProcessorTest, expand_0300) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0008
    performExpandTestFromManifest("#t0300", "expand-manifest-extra.jsonld");
}

TEST(JsonLdProcessorTest, expand_0301) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0020
    performExpandTestFromManifest("#t0301", "expand-manifest-extra.jsonld");
}

TEST(JsonLdProcessorTest, expand_0302) {
    // this is an extra test Dan added while trying to debug issues with normalize test 0044
    performExpandTestFromManifest("#t0302", "expand-manifest-extra.jsonld");
}

// This test would run everything in the manifest all at once, but is disabled for now
// until after we get everything working using the single test functions above.
TEST(JsonLdProcessorTest, DISABLED_expand_with_manifest) {

    ManifestLoader manifestLoader(resolvePath("test/testjsonld-cpp/test_data/"), "expand-manifest.jsonld");
    std::map<std::string, TestCase> testCases = manifestLoader.load();

    for(auto & testCaseEntry : testCases) {
        std::cout << testCaseEntry.first << std::endl;

        auto testCase = testCaseEntry.second;

        performExpandTest(testCase);
    }

}
