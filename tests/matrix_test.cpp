#include "qs.hpp"
#define HTEST_DEFINE_MAIN
#include "htest.hpp"


HT_CASE(Matrix, is_sym)
{
    qs::Matrixf<3, 3>  m;
    m << 1, 2, 3,
         2, 4, 5,
         3, 5, 6;

    HT_ASSERT_TRUE(m.is_sym());
}


HT_CASE(Matrix, to_failed)
{
    HT_ASSERT_TRUE(1 == 2);
    HT_ASSERT_NE(1, 1);
    HT_ASSERT_NEAR(2.0, 1.0, 0.002);
}
