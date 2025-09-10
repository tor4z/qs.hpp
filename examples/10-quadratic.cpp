#include "qs.hpp"
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main()
{
    qs::Matrixf<3, 3> A;
    qs::Vectorf<3> x;
    A << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
    x << 4.0, 6.0, 9.0;
    std::cout << A << "\n";
    std::cout << x << "\n";

    auto J{(A + A.t()) * x};
    std::cout << J << "\n";

    auto AAt{A + A.t()};
    while (1) {
        auto J{AAt * x};
        x = (x - (J * 0.001));
        std::cout << "x " << x << "\n";
        std::cout << "J " << J << "\n";
        std::cout << "result " << x.t() * A * x << "\n";
        std::this_thread::sleep_for(10ms);
    }

    return 0;
}
