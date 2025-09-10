// #include "qs.hpp"
#include "qs.hpp"
#define HTEST_IMPLEMENTATION
#define HTEST_DEFINE_MAIN
#include "htest.hpp"
#include <iostream>


HT_CASE(Matrix, is_sym)
{
    qs::Matrixf<3, 3>  m;
    m << 1, 2, 3,
         2, 4, 5,
         3, 5, 6;

    HT_ASSERT_TRUE(m.is_sym());
    HT_ASSERT_TRUE(false);
}
