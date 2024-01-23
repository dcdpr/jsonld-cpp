#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#pragma clang diagnostic push
#pragma GCC diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#include <rapidcheck/gtest.h>
#include <fstream>

#pragma clang diagnostic pop
#pragma GCC diagnostic pop

namespace {

    class Base {
    protected:
        std::map<std::string, std::string> map;

    public:
        virtual ~Base() = default;

        virtual bool isA() = 0;
        virtual bool isB() = 0;
        virtual bool isC() = 0;

        std::string getValue() const {
            return map.at("value");
        }

        friend bool operator==(const Base &lhs, const Base &rhs);
        friend bool operator!=(const Base &lhs, const Base &rhs);
    };

    bool operator==(const Base &lhs, const Base &rhs) {
        return lhs.map == rhs.map;
    }

    bool operator!=(const Base &lhs, const Base &rhs) {
        return !(rhs == lhs);
    }



    class A : public Base {
    public:
        explicit A(const std::string & value) {
            map["type"] = "A";
            map["value"] = value;
        }
        ~A() override = default;

        bool isA() override {
            return true;
        }

        bool isB() override {
            return false;
        }

        bool isC() override {
            return false;
        }
    };

    class B : public Base {
    public:
        explicit B(const std::string & value) {
            map["type"] = "B";
            map["value"] = value;
        }
        ~B() override = default;

        bool isA() override {
            return false;
        }

        bool isB() override {
            return true;
        }

        bool isC() override {
            return false;
        }
    };

    class C : public Base {
    public:
        explicit C(const std::string & value) {
            map["type"] = "C";
            map["value"] = value;
        }
        ~C() override = default;

        bool isA() override {
            return false;
        }

        bool isB() override {
            return false;
        }

        bool isC() override {
            return true;
        }
    };

    // sort order should be: A < C < B ... similar to literals < blanknode < IRI
    struct BasePtrLess :
            public std::binary_function<const std::shared_ptr<Base>, const std::shared_ptr<Base>, bool> {
        bool operator()(const std::shared_ptr<Base> & ps1, const std::shared_ptr<Base> & ps2) const {
            if(ps1->isB()) {
                if(!ps2->isB())
                    return false;
            }
            else if (ps1->isC()) {
                if(ps2->isB())
                    return true;
                else if(ps2->isA())
                    return false;
            }
            else if(ps1->isA()) {
                if(ps2->isB() || ps2->isC())
                    return true;
            }
            return ps1->getValue() < ps2->getValue();
        }
    };

    struct APtrLess :
            public std::binary_function<const std::shared_ptr<A>, const std::shared_ptr<A>, bool> {
        bool operator()(const std::shared_ptr<A> & ps1, const std::shared_ptr<A> & ps2) const {
            return ps1->getValue() < ps2->getValue();
        }
    };

    struct BPtrLess :
            public std::binary_function<const std::shared_ptr<B>, const std::shared_ptr<B>, bool> {
        bool operator()(const std::shared_ptr<B> & ps1, const std::shared_ptr<B> & ps2) const {
            return ps1->getValue() < ps2->getValue();
        }
    };

    struct CPtrLess :
            public std::binary_function<const std::shared_ptr<C>, const std::shared_ptr<C>, bool> {
        bool operator()(const std::shared_ptr<C> & ps1, const std::shared_ptr<C> & ps2) const {
            return ps1->getValue() < ps2->getValue();
        }
    };

}




TEST(ObjectComparisonsTest, As_withSameValue_areEqual) {
    A l1("Same");
    A l2("Same");
    EXPECT_EQ(l1, l2);
}

TEST(ObjectComparisonsTest, As_withDifferentValue_areNotEqual) {
    A l1("Same");
    A l2("Not Same");
    EXPECT_NE(l1, l2);
}

TEST(ObjectComparisonsTest, sortedSetOfAs) {

    std::set<std::shared_ptr<A>, APtrLess> set;

    set.insert(std::make_shared<A>("a"));
    set.insert(std::make_shared<A>("z"));
    set.insert(std::make_shared<A>("y"));
    set.insert(std::make_shared<A>("b"));

    EXPECT_EQ(set.size(), 4);
    auto ci = set.begin();
    EXPECT_EQ((*ci)->getValue(), "a"); ci++;
    EXPECT_EQ((*ci)->getValue(), "b"); ci++;
    EXPECT_EQ((*ci)->getValue(), "y"); ci++;
    EXPECT_EQ((*ci)->getValue(), "z"); ci++;

}

TEST(ObjectComparisonsTest, sortedSetOfBs) {

    std::set<std::shared_ptr<B>, BPtrLess> set;

    set.insert(std::make_shared<B>("a"));
    set.insert(std::make_shared<B>("z"));
    set.insert(std::make_shared<B>("y"));
    set.insert(std::make_shared<B>("b"));

    EXPECT_EQ(set.size(), 4);
    auto ci = set.begin();
    EXPECT_EQ((*ci)->getValue(), "a"); ci++;
    EXPECT_EQ((*ci)->getValue(), "b"); ci++;
    EXPECT_EQ((*ci)->getValue(), "y"); ci++;
    EXPECT_EQ((*ci)->getValue(), "z"); ci++;

}

TEST(ObjectComparisonsTest, sortedSetOfCs) {

    std::set<std::shared_ptr<C>, CPtrLess> set;

    set.insert(std::make_shared<C>("a"));
    set.insert(std::make_shared<C>("z"));
    set.insert(std::make_shared<C>("y"));
    set.insert(std::make_shared<C>("b"));

    EXPECT_EQ(set.size(), 4);
    auto ci = set.begin();
    EXPECT_EQ((*ci)->getValue(), "a"); ci++;
    EXPECT_EQ((*ci)->getValue(), "b"); ci++;
    EXPECT_EQ((*ci)->getValue(), "y"); ci++;
    EXPECT_EQ((*ci)->getValue(), "z"); ci++;

}

TEST(ObjectComparisonsTest, sortedSetOfBases) {

    std::set<std::shared_ptr<Base>, BasePtrLess> set;

    set.insert(std::make_shared<A>("a2"));
    set.insert(std::make_shared<B>("b2"));
    set.insert(std::make_shared<C>("c4"));
    set.insert(std::make_shared<C>("c3"));
    set.insert(std::make_shared<A>("a1"));
    set.insert(std::make_shared<B>("b1"));
    set.insert(std::make_shared<C>("c1"));
    set.insert(std::make_shared<C>("c2"));

    // sort order should be: A < C < B ... similar to literals < blanknode < IRI

    EXPECT_EQ(set.size(), 8);
    auto ci = set.begin();
    EXPECT_TRUE((*ci)->isA());
    EXPECT_EQ((*ci)->getValue(), "a1");
    ci++;
    EXPECT_TRUE((*ci)->isA());
    EXPECT_EQ((*ci)->getValue(), "a2");
    ci++;
    EXPECT_TRUE((*ci)->isC());
    EXPECT_EQ((*ci)->getValue(), "c1");
    ci++;
    EXPECT_TRUE((*ci)->isC());
    EXPECT_EQ((*ci)->getValue(), "c2");
    ci++;
    EXPECT_TRUE((*ci)->isC());
    EXPECT_EQ((*ci)->getValue(), "c3");
    ci++;
    EXPECT_TRUE((*ci)->isC());
    EXPECT_EQ((*ci)->getValue(), "c4");
    ci++;
    EXPECT_TRUE((*ci)->isB());
    EXPECT_EQ((*ci)->getValue(), "b1");
    ci++;
    EXPECT_TRUE((*ci)->isB());
    EXPECT_EQ((*ci)->getValue(), "b2");
    ci++;

}

// checking to make sure we always call the custom operator==
TEST(ObjectComparisonsTest, As_callOperator) {
    A l1("Same");
    A l2("Same");
    EXPECT_EQ(l1, l2);
}

TEST(ObjectComparisonsTest, APointers_callOperator) {
    A *l1 = new A("Same");
    A *l2 = new A("Same");
    EXPECT_EQ(*l1, *l2);
    delete l1;
    delete l2;
}

