#include <jsonld-cpp/sha256.h>

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

// expected results generated using 'sha256sum' from GNU coreutils

TEST(Sha256Test, singleChar) {
    std::string plain = "a";
    EXPECT_EQ(sha256(plain), "ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb");
}

TEST(Sha256Test, fewChars) {
    std::string plain = "abcd";
    EXPECT_EQ(sha256(plain), "88d4266fd4e6338d13b845fcf289579d209c897823b9217da3e161936f031589");
}

TEST(Sha256Test, long_string) {
    std::string plain = "The sky above the port was the color of television, tuned to a dead channel.";
    EXPECT_EQ(sha256(plain), "5041821981ec48d8db280ff293c35de17ef5dbfbac25adc81ff272d0fc22b2ae");
}

TEST(Sha256Test, really_long_string) {
    std::string plain = "At vero eos et accusamus et iusto odio dignissimos ducimus qui blanditiis praesentium voluptatum deleniti atque corrupti quos dolores et quas molestias excepturi sint occaecati cupiditate non provident, similique sunt in culpa qui officia deserunt mollitia animi, id est laborum et dolorum fuga.";
    EXPECT_EQ(sha256(plain), "bccdfac0ab918728f8ac0055f9fdb2bf7e2043c0d7481a72cc2570f2a993e2fd");
}

TEST(Sha256Test, vector_of_strings) {
    std::vector<std::string> v = {"red", "green", "blue"};
    EXPECT_EQ(sha256(v), "348ec859e74cd0935faa6922caefa9d4606c6f31c86d2f586821b266dac31259");
    // should be the same as when manually concatenating
    EXPECT_EQ(sha256("redgreenblue"), "348ec859e74cd0935faa6922caefa9d4606c6f31c86d2f586821b266dac31259");
}

TEST(Sha256Test, vector_of_long_strings) {
    std::vector<std::string> v = {
            "abcdefghijklmnopqrstuvwxyz",
            "abcdefghijklmnopqrstuvwxyz",
            "abcdefghijklmnopqrstuvwxyz",
            "abcdefghijklmnopqrstuvwxyz",
            "abcdefghijklmnopqrstuvwxyz"};
    EXPECT_EQ(sha256(v), "06f9b1a7ac97bc8e6a835c08986fe538f0478b03826efb4eed35dc517b433b8a");
    // should be the same as when manually concatenating
    EXPECT_EQ(sha256("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"), "06f9b1a7ac97bc8e6a835c08986fe538f0478b03826efb4eed35dc517b433b8a");
}

TEST(Sha256Test, vector_of_strings_manually_updated) {
    std::vector<std::string> v = {"red", "green", "blue"};

    SHA256 md;

    for(const auto& s : v) {
        md.update(s);
    }

    std::string result = md.digest();

    EXPECT_EQ(result, "348ec859e74cd0935faa6922caefa9d4606c6f31c86d2f586821b266dac31259");
}
