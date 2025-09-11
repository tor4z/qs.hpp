#include "qs.hpp"
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

template<typename T>
T soft_thresholding(T x, float lambda, float tau)
{
    return x;
}

int main()
{
    // minimize 1/2 ‖Ax − b‖^2 + \lambda ‖x‖^1
    qs::Matrixf<3, 3> A;
    qs::Vectorf<3> x;
    qs::Vectorf<3> b;
    A << 8, 2, 3,
         2, 9, 5,
         3, 5, 6;
    x << 4.0, 6.0, 9.0;
    b << 3.0, 10.0, 5.0;

    std::cout << "A: " << A << "\n";
    std::cout << "b: " << b << "\n";
    std::cout << "init x: " << x << "\n";

    auto lambda{0.5f};
    auto tau_inv{1.0f};
    auto AtA_tauI{A.t() * A + tau_inv * qs::Matrixf<3, 3>::eye()};
    auto AtA_tauI_inv{AtA_tauI.inv()};
    auto Atb{A.t() * b};

    auto z = x.rand();
    auto y = x.rand();

    while (1) {
        x = AtA_tauI_inv * (Atb + tau_inv * (z - y));
        z = soft_thresholding(x + y, lambda, 1.0 / tau_inv);
        y = y + tau_inv * (x - z);

        auto fx{(A * x - b).norm2() + lambda * x.norm1()};
        std::cout << "fx: " << fx << "\n";
        // std::cout << "x: " << x << "\n";
        std::this_thread::sleep_for(100ms);
    }

    return 0;
}
