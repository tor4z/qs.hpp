#include "qs.hpp"
#include <iostream>


int main()
{
    qs::Matrixf<3, 3> m1;
    qs::Matrixf<3, 3> m2;
    qs::Vectorf<3> f;
    m1 << 1, 2, 3, 4, 5, 6, 7, 8, 9;
    std::cout << m1 << "\n";
    m2 << 1, 2, 3, 4, 5, 6, 7, 8, 9;
    f << 1, 2, 3;
    std::cout << m2 << "\n";

    std::cout << "matmul = " << m1 * m2 << "\n";
    std::cout << "x' * M * x = " << f.t() * m1 * f << "\n";

    return 0;
}
