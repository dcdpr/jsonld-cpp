#pragma ide diagnostic ignored "cert-err58-cpp"
#include <jsonld-cpp/RDFRegex.h>
#include <regex>

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>

#pragma clang diagnostic pop
#pragma GCC diagnostic pop


TEST(RegexTest, split_lines) {
    std::string blob = "one\n\ntwo\n \nthree";
    std::vector<std::string> lines;

    std::regex rgx(RDFRegex::EOLN);
    std::sregex_token_iterator i(blob.begin(), blob.end(), rgx,-1);
    std::sregex_token_iterator end;

    for ( ; i != end; ++i)
        lines.push_back(*i);

    EXPECT_EQ(lines.size(), 5);
    EXPECT_EQ(lines[0], "one");
    EXPECT_EQ(lines[1], "");
    EXPECT_EQ(lines[2], "two");
    EXPECT_EQ(lines[3], " ");
    EXPECT_EQ(lines[4], "three");
}

TEST(RegexTest, skip_empty_lines) {
    std::string blob = "one\n\ntwo\n \nthree";
    std::vector<std::string> lines;

    {
        std::regex rgx(RDFRegex::EOLN);
        std::sregex_token_iterator i(blob.begin(), blob.end(), rgx, -1);
        std::sregex_token_iterator end;

        for (; i != end; ++i)
            lines.push_back(*i);

        EXPECT_EQ(lines.size(), 5);
    }

    {
        std::regex rgx(RDFRegex::EMPTY);
        std::smatch match;
        int count = 0;

        for (const std::string &line : lines) {
            if (std::regex_match(line, match, rgx))
                continue;
            count++;
        }
        EXPECT_EQ(count, 3);
    }
}

TEST(RegexTest, match_quads) {
    std::string blob
            = "<http://example.com/Subj1> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.com/Type> .\n"
              "<http://example.com/Subj1> <http://example.com/prop1> <http://example.com/Obj1> .\n"
              "<http://example.com/Subj1> <http://example.com/prop2> \"Plain\" .\n"
              "<http://example.com/Subj1> <http://example.com/prop2> \"2012-05-12\"^^<http://www.w3.org/2001/XMLSchema#date> .\n"
              "<http://example.com/Subj1> <http://example.com/prop2> \"English\"@en .\n"
              "<http://example.com> <http://example.com/property> _:a .\n"
              "_:a <http://www.w3.org/1999/02/22-rdf-syntax-ns#first> \"a\" .\n"
              "_:a <http://www.w3.org/1999/02/22-rdf-syntax-ns#rest> \"b\" .\n"
              "<http://example.com/Subj1> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://example.com/Type> .\n"
              "<http://example.com/Type> <http://www.w3.org/1999/02/22-rdf-syntax-ns#label> \"myLabel\" .\n"
              "<http://example.com/Type> <http://example.com/prop2> \"2012-05-12\"^^<http://www.w3.org/2001/XMLSchema#date> .\n"
              "<http://example.com/Subj1> <http://example.com/prop1> <http://example.com/Obj1> .\n"
              "<http://example.com/Subj1> <http://example.com/prop2> \"Plain\" .\n"
              "<http://example.com/Subj1> <http://example.com/prop2> \"2012-05-12\"^^<http://www.w3.org/2001/XMLSchema#date> .\n"
              "<http://example.com/Subj1> <http://example.com/prop2> \"English\"@en .";
    std::vector<std::string> lines;

    {
        std::regex rgx(RDFRegex::EOLN);
        std::sregex_token_iterator i(blob.begin(), blob.end(), rgx, -1);
        std::sregex_token_iterator end;

        for (; i != end; ++i)
            lines.push_back(*i);

        EXPECT_EQ(lines.size(), 15);
    }

    {
        std::regex rgx(RDFRegex::QUAD);
        std::smatch match;
        int count = 0;

        for (const std::string &line : lines) {
            if (std::regex_match(line, match, rgx))
                count++;
        }
        EXPECT_EQ(count, 15);
    }

}
