#include <jsonld-cpp/sha384.h>

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

// expected results generated using 'sha384sum' from GNU coreutils

TEST(Sha384Test, singleChar) {
    std::string plain = "a";
    EXPECT_EQ(sha384(plain), "54a59b9f22b0b80880d8427e548b7c23abd873486e1f035dce9cd697e85175033caa88e6d57bc35efae0b5afd3145f31");
}

TEST(Sha384Test, fewChars) {
    std::string plain = "abcd";
    EXPECT_EQ(sha384(plain), "1165b3406ff0b52a3d24721f785462ca2276c9f454a116c2b2ba20171a7905ea5a026682eb659c4d5f115c363aa3c79b");
}

TEST(Sha384Test, long_string) {
    std::string plain = "The sky above the port was the color of television, tuned to a dead channel.";
    EXPECT_EQ(sha384(plain), "d082198ec36faa525f1f4724cbb0b88b8c6c3dea34269b8afd84aebd29e3ce63c8e92b3a636fabbe410add8117b0a8ae");
}

TEST(Sha384Test, really_long_string) {
    std::string plain = "At vero eos et accusamus et iusto odio dignissimos ducimus qui blanditiis praesentium voluptatum deleniti atque corrupti quos dolores et quas molestias excepturi sint occaecati cupiditate non provident, similique sunt in culpa qui officia deserunt mollitia animi, id est laborum et dolorum fuga.";
    EXPECT_EQ(sha384(plain), "2a6b2e9b7e5ddc9f7240382d21fa2b9b631713f408184824a53171409a1ef8960cfe9a1d9dd9bbcc8610e058ffd09c91");
}

TEST(Sha384Test, vector_of_strings) {
    std::vector<std::string> v = {"red", "green", "blue"};
    EXPECT_EQ(sha384(v), "2294deff046453e9216952db05d9acece7e21ae3db6700bf186ea0f39d29f97dbcb34d164957ff16cb68ea7da115c886");
    // should be the same as when manually concatenating
    EXPECT_EQ(sha384("redgreenblue"), "2294deff046453e9216952db05d9acece7e21ae3db6700bf186ea0f39d29f97dbcb34d164957ff16cb68ea7da115c886");
}

TEST(Sha384Test, vector_of_long_strings) {
    std::vector<std::string> v = {
            "abcdefghijklmnopqrstuvwxyz",
            "abcdefghijklmnopqrstuvwxyz",
            "abcdefghijklmnopqrstuvwxyz",
            "abcdefghijklmnopqrstuvwxyz",
            "abcdefghijklmnopqrstuvwxyz"};
    EXPECT_EQ(sha384(v), "9c2f48760d13ac42ead196e54dcbaa5e58720662a96b9194e9813329bdb627c7c1ca7715311632c139e7a35914fc1ecd");
    // should be the same as when manually concatenating
    EXPECT_EQ(sha384("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"), "9c2f48760d13ac42ead196e54dcbaa5e58720662a96b9194e9813329bdb627c7c1ca7715311632c139e7a35914fc1ecd");
}

TEST(Sha384Test, vector_of_strings_manually_updated) {
    std::vector<std::string> v = {"red", "green", "blue"};

    SHA384 md;

    for(const auto& s : v) {
        md.update(s);
    }

    std::string result = md.digest();

    EXPECT_EQ(result, "2294deff046453e9216952db05d9acece7e21ae3db6700bf186ea0f39d29f97dbcb34d164957ff16cb68ea7da115c886");
}
