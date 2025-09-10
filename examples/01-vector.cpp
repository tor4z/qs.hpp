#include "qs.hpp"
#include <iostream>


int main()
{
    qs::Vectori<4> v;
    v << 1, 2, 3, 4;
    std::cout << v << "\n";
    return 0;
}
