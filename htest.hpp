#ifndef HTEST_HPP_
#define HTEST_HPP_

#include <ios>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

#define HTEST_REPORT_LINE_LEN 48
#define HTEST_TERM_COLOR_B_RED_S    "\033[91m"
#define HTEST_TERM_COLOR_B_GREEN_S  "\033[32m"
#define HTEST_TERM_COLOR_E          "\033[0m"
#define HTEST_TERM_COLOR_SUCC_S HTEST_TERM_COLOR_B_GREEN_S
#define HTEST_TERM_COLOR_FAIL_S HTEST_TERM_COLOR_B_RED_S

#define HTEST_DEF_SINGLETON(classname)                                          \
public:                                                                         \
    static inline classname* instance()                                         \
    {                                                                           \
        static classname *instance_ = nullptr;                                  \
        static std::once_flag flag;                                             \
        if (!instance_) {                                                       \
            std::call_once(flag, [&](){                                         \
                instance_ = new (std::nothrow) classname();                     \
            });                                                                 \
        }                                                                       \
        return instance_;                                                       \
    }                                                                           \
private:                                                                        \
    classname(const classname&) = delete;                                       \
    classname& operator=(const classname&) = delete;                            \
    classname(const classname&&) = delete;                                      \
    classname& operator=(const classname&&) = delete;

#define HTEST_CASE(SUIT_NAME, CASE_NAME)                                        \
    struct HTestCase_##SUIT_NAME##_##CASE_NAME: public htest::BaseCase          \
    {                                                                           \
        HTestCase_##SUIT_NAME##_##CASE_NAME()                                   \
            : htest::BaseCase(#SUIT_NAME, #CASE_NAME)                           \
        {                                                                       \
            htest::TestCases::instance()->add_case(this);                       \
        }                                                                       \
        virtual void test_body() override;                                      \
    };                                                                          \
    HTestCase_##SUIT_NAME##_##CASE_NAME htestcase_##SUIT_NAME##_##CASE_NAME;    \
    void HTestCase_##SUIT_NAME##_##CASE_NAME::test_body()

#define HTEST_ASSERT_TRUE(v)                                                    \
    if ((v)) {                                                                  \
        report_success();                                                       \
    } else {                                                                    \
        report_fail(__LINE__, static_cast<bool>(v), true);                      \
    }

#ifndef HTEST_NO_SHORTCUT
#   define HT_CASE HTEST_CASE
#   define HT_ASSERT_TRUE HTEST_ASSERT_TRUE
#endif // HTEST_NO_SHORTCUT

namespace htest
{

class TestCases;

struct BaseCase
{    
    BaseCase(const std::string& suit_name, const std::string& case_name);
    void report();
    virtual void test_body() = 0;
protected:
    void report_success();
    template<typename T>
    void report_fail(int line, T test_val, T expect_val);
private:
    friend class TestCases;

    std::string suit_name_;
    std::string case_name_;
    int num_tests_;
    int num_failed_;
}; // struct BaseCase

class TestCases
{
    HTEST_DEF_SINGLETON(TestCases);
public:
    void report();
    void add_case(BaseCase* test_case);
    const std::vector<BaseCase*>& cases() const { return cases_; }
private:
    TestCases() {}
    std::vector<BaseCase*> cases_;
}; // class TestCases

template<typename T>
void BaseCase::report_fail(int line, T test_val, T expect_val)
{
    ++num_tests_;
    ++num_failed_;
    std::cout << HTEST_TERM_COLOR_FAIL_S << suit_name_ << "." << case_name_
        << " failed" HTEST_TERM_COLOR_E
        << ": Line " << line << ", expect `"
        << std::boolalpha << expect_val << "` got `" << test_val << "`\n";
}

} // namespace htest

#endif // HTEST_HPP_

#define HTEST_IMPLEMENTATION

#ifdef HTEST_IMPLEMENTATION
#ifndef HTEST_CPP_
#define HTEST_CPP_

namespace htest {

BaseCase::BaseCase(const std::string& suit_name, const std::string& case_name)
    : suit_name_(suit_name)
    , case_name_(case_name)
    , num_tests_(0)
    , num_failed_(0)
{}

void BaseCase::report_success()
{
    ++num_tests_;
}

void BaseCase::report()
{
    std::string suit_case_name{suit_name_ + "." + case_name_};
    std::cout << suit_case_name << " ";
    for (int i = suit_case_name.size(); i < HTEST_REPORT_LINE_LEN; ++i) {
        std::cout << ".";
    }

    if (num_failed_ == 0) {
        std::cout << HTEST_TERM_COLOR_SUCC_S " passed" HTEST_TERM_COLOR_E "\n" ;
    } else {
        std::cout << HTEST_TERM_COLOR_FAIL_S " failed" HTEST_TERM_COLOR_E "\n";
    }
}

void TestCases::add_case(BaseCase* test_case)
{
    cases_.push_back(test_case);
}

void TestCases::report()
{
    int num_asserts{0};
    int num_failed_asserts{0};
    auto num_tests{htest::TestCases::instance()->cases().size()};
    auto num_failed_tests{0};
    for (auto c: htest::TestCases::instance()->cases()) {
        num_asserts += c->num_tests_;
        num_failed_asserts += c->num_failed_;
        if (c->num_failed_ != 0) {
            ++num_failed_tests;
        }
    }

    std::cout << "Number of testing cases: " << num_tests << "\n"
        << "\t" << (num_tests - num_failed_tests) << " successed\n"
        << "\t" << num_failed_tests << " failed\n";
    std::cout << "Number of tested asserts: " << num_asserts << "\n"
        << "\t" << (num_asserts - num_failed_asserts) << " successed\n"
        << "\t" << num_failed_asserts << " failed\n";
}

} // namespace test

#ifdef HTEST_DEFINE_MAIN
int main()
{
    for (auto c: htest::TestCases::instance()->cases()) {
        c->test_body();
        c->report();
    }
    htest::TestCases::instance()->report();
    return 0;
}
#endif // HTEST_DEFINE_MAIN

#endif // HTEST_CPP_
#endif // HTEST_IMPLEMENTATION
