#include "qs.hpp"
#include <cstdlib>
#include <iostream>

template<typename T>
T soft_thresholding(T x, float lambda, float tau)
{
    T out(x.row(), x.col(), x.array().sign() * (x.array().abs() - (lambda * tau)).max(0));
    return out;
}

int main()
{
    // minimize 1/2 ‖Ax − b‖^2 + \lambda ‖x‖^1
    qs::Matrixf<3, 3> A;
    qs::Vectorf<3> x;
    qs::Vectorf<3> b;
    A << 1, 1, -1,
         4, 2, 1,
         1, -2, -2;
    x << 4.0, 6.0, 9.0; // initial
    b << 0, 7, -9;

    std::cout << "A: " << A << "\n";
    std::cout << "b: " << b << "\n";
    std::cout << "init x: " << x << "\n";

    auto lambda{0.5f};
    auto tau_inv{0.001f};
    auto AtA_tauI{A.t() * A + tau_inv * qs::Matrixf<3, 3>::eye()};
    auto AtA_tauI_inv{AtA_tauI.inv()};
    auto Atb{A.t() * b};

    auto z = x.rand();
    auto y = x.rand();

    auto last_fx{(A * x - b).norm2() + lambda * x.norm1()};
    while (1) {
        x = AtA_tauI_inv * (Atb + tau_inv * (z - y));
        z = soft_thresholding(x + y, lambda, 1.0 / tau_inv);
        y = y + tau_inv * (x - z);

        auto fx{(A * x - b).norm2() + lambda * x.norm1()};
        if (std::abs(fx - last_fx) < 1.0e-6) {
            break;
        }
        last_fx = fx;
    }

    std::cout << "fx: " << last_fx << "\n";
    std::cout << "result : " << x << "\n";

    return 0;
}

/* python code for verify

import numpy as np

# Define the 3x3 coefficient matrix A
A = np.array([
    [1, 1, -1],
    [4, 2, 1],
    [1, -2, -2]
])

# Define the constants vector b
b = np.array([0, 7, -9])

# Solve for x
x = np.linalg.solve(A, b)

print("Solution vector x:", x)
# Expected output: Solution vector x: [1. 2. 3.]

# Verify the solution
print("Verification (A @ x):", A @ x)
# Expected output: Verification (A @ x): [0. 7. -9.]

*/
