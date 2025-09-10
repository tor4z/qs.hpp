#include "qs.hpp"
#include <iostream>


int main()
{
    qs::Matrixf<2, 2> m;
    qs::Matrixf<3, 3> m3x3;
    m << 0, 2, 3, 4;
    m3x3 << 0, 2, 3, 4, 5, 6, 7, 8, 9;
    std::cout << m << "\n";

    std::cout << "Transposed = " << m.t() << "\n";
    std::cout << "Inv = " << m.inv() << "\n";
    std::cout << "Eye = " << qs::Matrixi<3, 3>::eye() << "\n";
    std::cout << "Eye * 7 = " << qs::Matrixi<3, 3>::eye() * 7 << "\n";
    std::cout << "Eye + Eye = " << qs::Matrixi<3, 3>::eye() + qs::Matrixi<3, 3>::eye() << "\n";
    std::cout << "Eye - Eye = " << qs::Matrixi<3, 3>::eye() - qs::Matrixi<3, 3>::eye() << "\n";
    std::cout << "det of m = " << m.det() << "\n";
    auto I3x3{qs::Matrixi<3, 3>::eye()};
    auto I7x7{qs::Matrixi<7, 7>::eye()};
    std::cout << "det of eye(3) = " << I3x3.det() << "\n";
    std::cout << "det of eye(7) = " << I7x7.det() << "\n";
    std::cout << "3x3 matrix: " << m3x3 << "\n";
    std::cout << "sub matrix of 3x3(0, 0, 2, 2): " << m3x3.sub(0, 0, 2, 2) << "\n";
    std::cout << "sub matrix of 3x3(1, 1, 2, 2): " << m3x3.sub(1, 1, 2, 2) << "\n";
    qs::Matrixi<7, 7> M7x7i{};
    qs::Matrixf<7, 7> M7x7f{};
    M7x7i.fill_rand_();
    M7x7f.fill_rand_();
    std::cout << "rand 7x7: " << M7x7i << "\n";
    std::cout << "rand 7x7: " << M7x7f << "\n";

    std::cout << "rand 7x7 is psd: " << M7x7f.is_psd() << "\n";
    std::cout << "rand eye(7) is psd: " << I7x7.is_psd() << "\n";

    std::cout << "rand 5x5: " << qs::Matrixf<5, 5>::rand() << "\n";
    return 0;
}
