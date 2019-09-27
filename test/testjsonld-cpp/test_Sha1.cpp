#include "sha1.cpp"
#include "testHelpers.h"

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

// expected results generated using 'sha1sum' from GNU coreutils

TEST(Sha1Test, singleChar) {
    std::string plain = "a";
    EXPECT_EQ(sha1(plain), "86f7e437faa5a7fce15d1ddcb9eaeaea377667b8");
}

TEST(Sha1Test, fewChars) {
    std::string plain = "abcd";
    EXPECT_EQ(sha1(plain), "81fe8bfe87576c3ecb22426f8e57847382917acf");
}

TEST(Sha1Test, long_string) {
    std::string plain = "The sky above the port was the color of television, tuned to a dead channel.";
    EXPECT_EQ(sha1(plain), "49ceb59827e0c76fcf94d5da80b8f4bc0dc94b15");
}

TEST(Sha1Test, vector_of_strings) {
    std::vector<std::string> v = {"red", "green", "blue"};
    EXPECT_EQ(sha1(v), "df34ee3d80c42dbacbdc0a22686d41c5769eeaee");
    // should be the same as when manually concatenating
    EXPECT_EQ(sha1("redgreenblue"), "df34ee3d80c42dbacbdc0a22686d41c5769eeaee");
}

TEST(Sha1Test, vector_of_strings_manually_updated) {
    std::vector<std::string> v = {"red", "green", "blue"};

    SHA1 md;

    for(const auto& s : v) {
        md.update(s);
    }

    std::string result = md.digest();

    EXPECT_EQ(result, "df34ee3d80c42dbacbdc0a22686d41c5769eeaee");
}
