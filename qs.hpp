#ifndef QS_HPP_
#define QS_HPP_

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <ostream>

namespace qs {

template<typename T, int R, int C>
struct Matrix;

template<typename T>
struct MatrixX
{
protected:
    struct MatrixInitalizer
    {
        MatrixInitalizer(MatrixX* m) : m(m), i(1) {}
        MatrixInitalizer& operator,(T v);
        MatrixX* m;
        int i;
    }; // struct MatrixInitalizer

    std::vector<T> data_;
    int col_;
    int row_;

    void eye_();
public:
    static MatrixX<T> eye(int row_col);
    inline int row() const { return row_; };
    inline int col() const { return col_; };
    inline int size() const { return row_ * col_; };
    inline T& at(int i) { return data_.at(i); };
    inline T at(int i) const { return data_.at(i); };
    inline T& at(int r, int c) { return at(r * col_ + c); };
    inline T at(int r, int c) const { return at(r * col_ + c); };
    inline const std::vector<T>& data() const { return data_; }
    inline bool is_pd() const { return is_pd_psd(false); }
    inline bool is_psd() const { return is_pd_psd(true); }

    MatrixX(const MatrixX& m);
    MatrixX(MatrixX&& m);
    MatrixX(int row, int col);
    MatrixX<T> t() const;
    MatrixX<T> inv() const;
    MatrixX<T> sub(int sr, int sc, int r, int c) const;
    T det() const;
    void fill_rand_();
    void fill_0_();
    void fill_1_();
    void resize_(int r, int c);
    bool is_sym() const;

    MatrixInitalizer operator<<(T v);
    MatrixX& operator=(const MatrixX& m);
    MatrixX& operator=(MatrixX&& m);
    MatrixX operator*(const MatrixX& m) const;
    MatrixX operator*(T v) const;
    MatrixX operator+(const MatrixX& m) const;
    MatrixX operator-(const MatrixX& m) const;
private:
    bool is_pd_psd(bool psd) const;
}; // struct MatrixX

using MatrixXd = MatrixX<double>;
using MatrixXf = MatrixX<float>;
using MatrixXi = MatrixX<int>;

template<typename T, int R, int C>
struct Matrix: public MatrixX<T>
{
    Matrix() : MatrixX<T>(R, C) {}
    Matrix& operator=(const MatrixX<T>& m);
    Matrix& operator=(MatrixX<T>&& m);

    static Matrix<T, R, C> eye();
    static Matrix<T, R, C> rand();
    static Matrix<T, R, C> ones();
    static Matrix<T, R, C> zeros();
}; // struct Matrix

template<int R, int C>
using Matrixd = Matrix<double, R, C>;
template<int R, int C>
using Matrixf = Matrix<float, R, C>;
template<int R, int C>
using Matrixi = Matrix<int, R, C>;

template<typename T, int DIM>
using Vector = Matrix<T, DIM, 1>;
template<int DIM>
using Vectorf = Vector<float, DIM>;
template<int DIM>
using Vectord = Vector<double, DIM>;
template<int DIM>
using Vectori = Vector<int, DIM>;

template<typename T, int R, int C>
Matrix<T, R, C>& Matrix<T, R, C>::operator=(const MatrixX<T>& m)
{
    assert(R == m.row() && C == m.col());
    this->col_ = m.col();
    this->row_ = m.row();
    this->data_ = m.data();
    return *this;
}

template<typename T, int R, int C>
Matrix<T, R, C>& Matrix<T, R, C>::operator=(MatrixX<T>&& m)
{
    assert(R == m.row() && C == m.col());
    this->col_ = m.col();
    this->row_ = m.row();
    this->data_ = std::move(m.data());
    return *this;
}

template<typename T, int R, int C>
Matrix<T, R, C> Matrix<T, R, C>::eye()
{
    static_assert(R == C);
    Matrix<T, R, C> m;
    m.eye_();
    return m;
}

template<typename T, int R, int C>
Matrix<T, R, C> Matrix<T, R, C>::rand()
{
    Matrix<T, R, C> m;
    m.fill_rand_();
    return m;
}

template<typename T, int R, int C>
Matrix<T, R, C> Matrix<T, R, C>::ones()
{
    Matrix<T, R, C> m;
    m.fill_1_();
    return m;
}

template<typename T, int R, int C>
Matrix<T, R, C> Matrix<T, R, C>::zeros()
{
    Matrix<T, R, C> m;
    m.fill_0_();
    return m;
}

template<typename T>
MatrixX<T>::MatrixX(int row, int col)
    : col_(col), row_(row)
{
    assert(row_ > 0 && col_ > 0);
    data_.resize(col_ * row_);
}

template<typename T>
typename MatrixX<T>::MatrixInitalizer MatrixX<T>::operator<<(T v)
{
    MatrixInitalizer mi(this);
    data_.at(0) = v;
    return mi;
}

template<typename T>
typename MatrixX<T>::MatrixInitalizer& MatrixX<T>::MatrixInitalizer::operator,(T v)
{
    m->data_.at(i++) = v;
    return *this;
}

template<typename T>
MatrixX<T>::MatrixX(const MatrixX& m)
    : data_(m.data_)
    , col_(m.col_)
    , row_(m.row_)
{
}

template<typename T>
MatrixX<T>::MatrixX(MatrixX&& m)
    : data_(std::move(m.data_))
    , col_(m.col_)
    , row_(m.row_)
{
    m.col_ = 0;
    m.row_ = 0;
}

template<typename T>
MatrixX<T>& MatrixX<T>::operator=(const MatrixX& m)
{
    data_ = m.data_;
    col_ = m.col_;
    row_ = m.row_;
}

template<typename T>
MatrixX<T>& MatrixX<T>::operator=(MatrixX&& m)
{
    data_ = std::move(m.data_);
    col_ = m.col_;
    row_ = m.row_;
    m.col_ = 0;
    m.row_ = 0;
}

template<typename T>
MatrixX<T> MatrixX<T>::eye(int row_col)
{
    MatrixX<T> m(row_col, row_col);
    m.eye_();
    return m;
}

template<typename T>
void MatrixX<T>::eye_()
{
    assert(row_ == col_);
    for (int i = 0; i < row_; ++i) {
        at(i, i) = 1;
    }
}

template<typename T>
MatrixX<T> MatrixX<T>::t() const
{
    MatrixX<T> m(col_, row_);
    for (int r = 0; r < row_; ++r) {
        for (int c = 0; c < col_; ++c) {
            m.at(c, r) = at(r, c);
        }
    }
    return m;
}

template<typename T>
MatrixX<T> MatrixX<T>::inv() const
{
    // Gauss Jordan algorithm
    auto out{MatrixX<T>::eye(row_)};
    auto tmp_m{*this};

    auto row_swap{[] (MatrixX<T>& m, int r1, int r2) -> void {
        for (int c = 0; c < m.col_; ++c) {
            T tmp{m.at(r1, c)};
            m.at(r1, c) = m.at(r2, c);
            m.at(r2, c) = tmp;
        }
    }};

    for (int c = 0; c < out.col_; ++c) {
        if (tmp_m.at(c, c) == 0) {
            // swap row
            for (int r1 = c + 1; r1 < out.row_; ++r1) {
                if (tmp_m.at(r1, c) != 0) {
                    row_swap(tmp_m, c, r1);
                    row_swap(out, c, r1);
                }
            }
        }

        auto tmp{tmp_m.at(c, c)};
        for (int c1 = 0; c1 < out.col_; ++c1) {
            tmp_m.at(c, c1) /= tmp;
            out.at(c, c1) /= tmp;
        }

        for (int r = 0; r < out.row_; ++r) {
            if (r == c) continue;
            const auto t{tmp_m.at(r, c)};
            for (int c1 = 0; c1 < out.col_; ++c1) {
                out.at(r, c1) -= out.at(c, c1) * t;
                tmp_m.at(r, c1) -= t * tmp_m.at(c, c1);
            }
        }
    }

    return out;
}

template<typename T>
T MatrixX<T>::det() const
{
    assert(row_ == col_);

    if (row_ == 1) {
        return data_.at(0);
    } else if (row_ == 2) {
        return at(0, 0) * at(1, 1) - at(1, 0) * at(0, 1);
    }

    // calculate determinant with cofactor expansions
    T result{0};
    MatrixX<T> minor(row_ - 1, col_ - 1);
    for (int c = 0; c < col_; ++c) {
        T sign{(1 + 1 + c) % 2 == 0 ? static_cast<T>(1) : static_cast<T>(-1)};
        T pivot{at(0, c)};

        // expansion for first row
        for (int mr = 1; mr < row_; ++mr) {
            int cc{0};
            for (int mc = 0; mc < col_; ++mc) {
                if (c == mc) continue;
                minor.at(mr - 1, cc++) = at(mr, mc);
            }
        }

        result += sign * pivot * minor.det();
    }
    return result;
}

template<typename T>
MatrixX<T> MatrixX<T>::sub(int sr, int sc, int r, int c) const
{
    // check
    assert(sr >= 0 && sc >= 0);
    assert(sr + r <= row_ && sc + c <= col_);

    if (sr == 0 && sc == 0 && r == row_ && c == col_) {
        return *this;
    } else {
        MatrixX<T> out(r, c);
        int out_r{0};
        for (int i = sr; i < sr + r; ++i) {
            int out_c{0};
            for (int j = sc; j < sc + c; ++j) {
                out.at(out_r, out_c++) = at(i, j);
            }
            ++out_r;
        }
        return out;
    }
}

template<typename T>
void MatrixX<T>::fill_rand_()
{
    const auto matrix_size{size()};
    if (std::is_integral_v<T>) {
        for (int i = 0; i < matrix_size; ++i) at(i) = rand();
    } else {
        for (int i = 0; i < matrix_size; ++i) at(i) = static_cast<T>(rand()) / static_cast<T>(RAND_MAX);
    }
}

template<typename T>
void MatrixX<T>::fill_0_()
{
    const auto matrix_size{size()};
    std::fill(data_.begin(), data_.end(), 0);
}

template<typename T>
void MatrixX<T>::fill_1_()
{
    const auto matrix_size{size()};
    std::fill(data_.begin(), data_.end(), 1);
}

template<typename T>
void MatrixX<T>::resize_(int r, int c)
{
    col_ = c;
    row_ = r;
    data_.resize(col_ * row_);
}

template<typename T>
MatrixX<T> MatrixX<T>::operator*(const MatrixX& m) const
{
    MatrixX<T> out{row_, m.col_};
    for (int r = 0; r < out.row_; ++r) {
        for (int c = 0; c < out.col_; ++c) {
            T v{0};
            for (int c1 = 0; c1 < col_; ++c1) {
                v += at(r, c1) * m.at(c1, c);
            }
            out.at(r, c) = v;
        }
    }
    return out;
}

template<typename T>
MatrixX<T> MatrixX<T>::operator*(T v) const
{
    MatrixX<T> out{*this};
    auto matrix_size{size()};
    for (int i = 0; i < matrix_size; ++i) {
        out.data_.at(i) *= v;
    }
    return out;
}

template<typename T>
MatrixX<T> MatrixX<T>::operator+(const MatrixX& m) const
{
    assert(m.row_ == row_ && m.col_ == col_);
    MatrixX<T> out{*this};
    const auto matrix_size{size()};
    for (int i = 0; i < matrix_size; ++i) {
        out.data_.at(i) += m.data_.at(i);
    }
    return out;
}

template<typename T>
MatrixX<T> MatrixX<T>::operator-(const MatrixX& m) const
{
    assert(m.row_ == row_ && m.col_ == col_);

    MatrixX<T> out{*this};
    const auto matrix_size{size()};
    for (int i = 0; i < matrix_size; ++i) {
        out.data_.at(i) -= m.data_.at(i);
    }
    return out;
}

template<typename T>
bool MatrixX<T>::is_pd_psd(bool psd) const
{
    // Sylvester's criterion
    assert(col_ == row_);

    for (int i = 0; i < col_; ++i) {
        T d{sub(i, i, row_ - i, col_ - i).det()};
        if (d < 0 || (!psd && d == 0)) {
            return false;
        }
    }
    return true;
}

template<typename T>
bool MatrixX<T>::is_sym() const
{
    if (row_ != col_) return false;
    if (row_ == 1) return true;

    for (int r = 0; r < row_; ++r) {
        for (int c = r + 1; c < col_; ++c) {
            if (at(r, c) != at(c, r)) return false;
        }
    }
    return true;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const MatrixX<T>& m)
{
    os << "Matrix" << typeid(T).name() << "<" << m.row() << ", " << m.col() << ">{";
    os << std::fixed << std::setprecision(2);
    for (int i = 0; i < m.size(); ++i) {
        if (i % m.col() == 0) { os << "\n  "; }
        os << m.at(i) << ", ";
    }
    os << "\n}";
    return os;
}

} // namespace qs

#endif // QS_HPP_
