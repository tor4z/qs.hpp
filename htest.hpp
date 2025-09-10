#ifndef HTEST_HPP_
#define HTEST_HPP_

#include <mutex>
#include <vector>

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
        {                                                                       \
            htest::TestCases::instance()->add_case(this);                       \
        }                                                                       \
        virtual void test_body() override;                                      \
    };                                                                          \
    HTestCase_##SUIT_NAME##_##CASE_NAME htestcase_##SUIT_NAME##_##CASE_NAME;    \
    void HTestCase_##SUIT_NAME##_##CASE_NAME::test_body()


namespace htest
{

struct BaseCase
{
    virtual void test_body() = 0;
}; // struct BaseCase

class TestCases
{
    HTEST_DEF_SINGLETON(TestCases);
public:
    void add_case(BaseCase* test_case);
    const std::vector<BaseCase*>& cases() const { return cases_; }
private:
    TestCases() {}
    std::vector<BaseCase*> cases_;
}; // class TestCases

} // namespace htest

#endif // HTEST_HPP_


#ifdef HTEST_IMPLEMENTATION
#ifndef HTEST_CPP_
#define HTEST_CPP_

namespace htest {

void TestCases::add_case(BaseCase* test_case)
{
    cases_.push_back(test_case);
}

} // namespace test

#ifdef HTEST_DEFINE_MAIN
int main()
{
    for (auto c: htest::TestCases::instance()->cases()) {
        c->test_body();
    }
    return 0;
}
#endif // HTEST_DEFINE_MAIN

#endif // HTEST_CPP_
#endif // HTEST_IMPLEMENTATION
