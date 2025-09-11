#include "qs.hpp"
#include <cstdlib>
#include <iostream>


int main()
{
    // f(x) = x^TAx
    qs::Matrixf<3, 3> A;
    qs::Vectorf<3> x;
    A << 8, 2, 3,
         2, 9, 5,
         3, 5, 6;
    x << 4.0, 6.0, 9.0;
    std::cout << A << "\n";
    std::cout << "init x: " << x << "\n";

    auto AAt{A + A.t()};
    float last_val{(x.t() * A * x).at(0)};
    while (1) {
        auto J{AAt * x};
        auto H{AAt};
        auto theta{-1.f * H.inv() * J};
        x = (x + theta);
        float this_val{(x.t() * A * x).scalar()};
        if (std::abs(last_val - this_val) < 1.0e-5) {
            break;
        }
        last_val = this_val;
    }
    std::cout << "=============\nresult: " << x << "\n";

    return 0;
}
