#include "qs.hpp"
#include <iostream>


int main()
{
    auto m1{qs::Matrixf<3, 3>::rand()};
    std::cout << m1.array() << "\n";
    qs::Matrixf<3, 3> m2(m1.array());
    std::cout << m2 << "\n";
    return 0;
}
