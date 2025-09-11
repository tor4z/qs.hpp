#ifndef QS_HPP_
#define QS_HPP_

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <iomanip>
#include <ostream>

#define QS_PRINT_PRECISION 2

namespace qs {

template<typename T, int R, int C>
struct Matrix;

template<typename T>
struct MatrixX;

template<typename T>
struct Array
{
    Array(int size);
    Array(const Array& other);
    Array(Array&& other);
    Array& operator=(const Array& other);
    Array& operator=(Array&& other);

    bool operator==(const Array& other) const;
    Array operator*(const Array& other) const;
    Array operator*(T v) const;
    Array operator+(const Array& other) const;
    Array operator-(const Array& other) const;
    Array operator+(T v) const;
    Array operator-(T v) const;

    inline T at(int i) const { return data_.at(i); };
    inline T& at(int i) { return data_.at(i); };
    inline int size() const { return data_.size(); };

    Array max(T v) const;
    Array abs() const;
    Array sign();
    void max_(T v);
    void abs_();
private:
    friend struct MatrixX<T>;

    std::vector<T> data_;
}; // struct Array

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

    Array<T> array_;
    int col_;
    int row_;

    void eye_();
public:
    static MatrixX<T> eye(int row_col);
    inline int row() const { return row_; };
    inline int col() const { return col_; };
    inline int size() const { return array_.size(); };
    inline T& at(int i) { return array_.at(i); };
    inline T at(int i) const { return array_.at(i); };
    inline T& at(int r, int c) { return at(r * col() + c); };
    inline T at(int r, int c) const { return at(r * col() + c); };
    inline T scalar() const { assert(is_scalar()); return array_.at(0); }
    inline bool is_scalar() const { return size() == 1; }
    inline const std::vector<T>& data() const { return array_.data_; }
    inline const Array<T>& array() const { return array_; }
    inline Array<T>& array() { return array_; }
    inline bool is_pd() const { return is_pd_psd(false); }
    inline bool is_psd() const { return is_pd_psd(true); }
    inline bool operator==(const MatrixX& other) const { return array_ == other.array_; }
    inline MatrixX operator*(T v) const { return MatrixX<T>{row(), col(), array_ * v}; }
    inline MatrixX operator+(const MatrixX& other) const { return MatrixX<T>{row(), col(), array_ + other.array_}; }
    inline MatrixX operator-(const MatrixX& other) const { return MatrixX<T>{row(), col(), array_ - other.array_}; }

    MatrixX(int row, int col);
    MatrixX(const MatrixX& other);
    MatrixX(MatrixX&& other);
    MatrixX& operator=(const MatrixX& other);
    MatrixX& operator=(MatrixX&& other);

    MatrixX(int row, int col, const Array<T>& other);
    MatrixX(int row, int col, Array<T>&& other);
    MatrixX& operator=(const Array<T>& other);
    MatrixX& operator=(Array<T>&& other);

    MatrixX<T> t() const;
    MatrixX<T> inv() const;
    MatrixX<T> sub(int sr, int sc, int r, int c) const;
    T det() const;
    T trace() const;
    T norm2() const;
    T norm1() const;
    void fill_rand_();
    void fill_0_();
    void fill_1_();
    void resize_(int r, int c);
    bool is_sym() const;

    MatrixInitalizer operator<<(T v);
    MatrixX operator*(const MatrixX& m) const;
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
    Matrix(const MatrixX<T>& other);
    Matrix(MatrixX<T>&& other);
    Matrix& operator=(const MatrixX<T>& other);
    Matrix& operator=(MatrixX<T>&& other);

    Matrix(const Array<T>& other);
    Matrix(Array<T>&& other);
    Matrix& operator=(const Array<T>& other);
    Matrix& operator=(Array<T>&& other);

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
Matrix<T, R, C>::Matrix(const MatrixX<T>& other)
    : MatrixX<T>(other)
{}

template<typename T, int R, int C>
Matrix<T, R, C>::Matrix(MatrixX<T>&& other)
    : MatrixX<T>(std::move(other))
{}

template<typename T, int R, int C>
Matrix<T, R, C>& Matrix<T, R, C>::operator=(const MatrixX<T>& other)
{
    assert(R == other.row() && C == other.col());
    this->array_ = other.array_;
    return *this;
}

template<typename T, int R, int C>
Matrix<T, R, C>& Matrix<T, R, C>::operator=(MatrixX<T>&& other)
{
    assert(R == other.row() && C == other.col());
    this->array_ = std::move(other.array());
    return *this;
}

template<typename T, int R, int C>
Matrix<T, R, C>::Matrix(const Array<T>& other)
    : MatrixX<T>(R, C, other)
{}

template<typename T, int R, int C>
Matrix<T, R, C>::Matrix(Array<T>&& other)
    : MatrixX<T>(R, C, std::move(other))
{}

template<typename T, int R, int C>
Matrix<T, R, C>& Matrix<T, R, C>::operator=(const Array<T>& other)
{
    assert(R * C == other.size());
    this->array_ = other;
    return *this;
}

template<typename T, int R, int C>
Matrix<T, R, C>& Matrix<T, R, C>::operator=(Array<T>&& other)
{
    assert(R * C == other.size());
    this->array_ = std::move(other);
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
Array<T>::Array(int size)
{
    assert(size > 0);
    data_.resize(size);
}

template<typename T>
Array<T>::Array(const Array& other)
    : data_(other.data_)
{
}

template<typename T>
Array<T>::Array(Array&& other)
    : data_(std::move(other.data_))
{
}

template<typename T>
Array<T>& Array<T>::operator=(const Array& other)
{
    if (this != &other) {
        data_ = other.data_;
    }
    return *this;
}

template<typename T>
Array<T>& Array<T>::operator=(Array&& other)
{
    data_ = std::move(other.data_);
    return *this;
}

template<typename T>
bool Array<T>::operator==(const Array<T>& other) const
{
    const auto matrix_size{size()};
    if (matrix_size == other.size()) return false;
    for (int i = 0; i < matrix_size; ++i) {
        if (at(i) != other.at(i)) return false;
    }
    return true;
}

template<typename T>
Array<T> Array<T>::operator*(const Array<T>& other) const
{
    assert(other.size() == size());
    Array<T> out{*this};
    const auto matrix_size{size()};
    for (int i = 0; i < matrix_size; ++i) {
        out.at(i) *= other.at(i);
    }
    return out;
}

template<typename T>
Array<T> Array<T>::operator*(T v) const
{
    Array<T> out{*this};
    auto matrix_size{size()};
    for (int i = 0; i < matrix_size; ++i) {
        out.at(i) *= v;
    }
    return out;
}

template<typename T>
Array<T> Array<T>::operator+(const Array<T>& other) const
{
    assert(other.size() == size());
    Array<T> out{*this};
    const auto matrix_size{size()};
    for (int i = 0; i < matrix_size; ++i) {
        out.at(i) += other.at(i);
    }
    return out;
}

template<typename T>
Array<T> Array<T>::operator-(const Array<T>& other) const
{
    assert(other.size() == size());
    Array<T> out{*this};
    const auto matrix_size{size()};
    for (int i = 0; i < matrix_size; ++i) {
        out.at(i) -= other.at(i);
    }
    return out;
}

template<typename T>
Array<T> Array<T>::operator+(T v) const
{
    Array<T> out{*this};
    const auto matrix_size{size()};
    for (int i = 0; i < matrix_size; ++i) {
        out.at(i) += v;
    }
    return out;
}

template<typename T>
Array<T> Array<T>::operator-(T v) const
{
    Array<T> out{*this};
    const auto matrix_size{size()};
    for (int i = 0; i < matrix_size; ++i) {
        out.at(i) -= v;
    }
    return out;
}

template<typename T>
Array<T> Array<T>::sign()
{
    Array<T> out(size());
    const auto array_size{size()};
    for (int i = 0; i < array_size; ++i) {
        out.at(i) = at(i) > 0 ? 1 : -1;
    }
    return out;
}

template<typename T>
void Array<T>::max_(T v)
{
    const auto array_size{size()};
    for (int i = 0; i < array_size; ++i) {
        at(i) = std::max(at(i), v);
    }
}

template<typename T>
void Array<T>::abs_()
{
    const auto array_size{size()};
    for (int i = 0; i < array_size; ++i) {
        at(i) = std::abs(at(i));
    }
}

template<typename T>
Array<T> Array<T>::max(T v) const
{
    Array<T>out(*this);
    out.max_(v);
    return out;
}

template<typename T>
Array<T> Array<T>::abs() const
{
    Array<T>out(*this);
    out.abs_();
    return out;
}

template<typename T>
typename MatrixX<T>::MatrixInitalizer MatrixX<T>::operator<<(T v)
{
    MatrixInitalizer mi(this);
    array_.at(0) = v;
    return mi;
}

template<typename T>
typename MatrixX<T>::MatrixInitalizer& MatrixX<T>::MatrixInitalizer::operator,(T v)
{
    m->array_.at(i++) = v;
    return *this;
}

template<typename T>
MatrixX<T>::MatrixX(int row, int col)
    : array_(row * col)
    , row_(row)
    , col_(col)
{}

template<typename T>
MatrixX<T>::MatrixX(const MatrixX& other)
    : MatrixX(other.row_, other.col_, other.array_)
{
}

template<typename T>
MatrixX<T>::MatrixX(int row, int col, const Array<T>& other)
    : array_(other)
    , row_(row)
    , col_(col)
{}

template<typename T>
MatrixX<T>::MatrixX(int row, int col, Array<T>&& other)
    : array_(std::move(other))
    , row_(row)
    , col_(col)
{}

template<typename T>
MatrixX<T>::MatrixX(MatrixX&& other)
    : MatrixX(other.row_, other.col_, std::move(other.array_))
{
}

template<typename T>
MatrixX<T>& MatrixX<T>::operator=(const MatrixX& other)
{
    if (this != &other) {
        array_ = other.array_;
    }
    return *this;
}

template<typename T>
MatrixX<T>& MatrixX<T>::operator=(MatrixX&& other)
{
    array_ = std::move(other.array_);
    return *this;
}

template<typename T>
MatrixX<T>& MatrixX<T>::operator=(const Array<T>& other)
{
    array_ = other;
    return *this;
}

template<typename T>
MatrixX<T>& MatrixX<T>::operator=(Array<T>&& other)
{
    array_ = std::move(other);
    return *this;
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
    assert(row() == col());
    for (int i = 0; i < row(); ++i) {
        at(i, i) = 1;
    }
}

template<typename T>
MatrixX<T> MatrixX<T>::t() const
{
    MatrixX<T> m(col(), row());
    for (int r = 0; r < row(); ++r) {
        for (int c = 0; c < col(); ++c) {
            m.at(c, r) = at(r, c);
        }
    }
    return m;
}

template<typename T>
MatrixX<T> MatrixX<T>::inv() const
{
    // Gauss Jordan algorithm
    auto out{MatrixX<T>::eye(row())};
    auto tmp_m{*this};

    auto row_swap{[] (MatrixX<T>& m, int r1, int r2) -> void {
        for (int c = 0; c < m.col(); ++c) {
            T tmp{m.at(r1, c)};
            m.at(r1, c) = m.at(r2, c);
            m.at(r2, c) = tmp;
        }
    }};

    for (int c = 0; c < out.col(); ++c) {
        if (tmp_m.at(c, c) == 0) {
            // swap row
            for (int r1 = c + 1; r1 < out.row(); ++r1) {
                if (tmp_m.at(r1, c) != 0) {
                    row_swap(tmp_m, c, r1);
                    row_swap(out, c, r1);
                }
            }
        }

        auto tmp{tmp_m.at(c, c)};
        for (int c1 = 0; c1 < out.col(); ++c1) {
            tmp_m.at(c, c1) /= tmp;
            out.at(c, c1) /= tmp;
        }

        for (int r = 0; r < out.row(); ++r) {
            if (r == c) continue;
            const auto t{tmp_m.at(r, c)};
            for (int c1 = 0; c1 < out.col(); ++c1) {
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
    assert(row() == col());

    if (row() == 1) {
        return array_.at(0);
    } else if (row() == 2) {
        return at(0, 0) * at(1, 1) - at(1, 0) * at(0, 1);
    }

    // calculate determinant with cofactor expansions
    T result{0};
    MatrixX<T> minor(row() - 1, col() - 1);
    for (int c = 0; c < col(); ++c) {
        T sign{(1 + 1 + c) % 2 == 0 ? static_cast<T>(1) : static_cast<T>(-1)};
        T pivot{at(0, c)};

        // expansion for first row
        for (int mr = 1; mr < row(); ++mr) {
            int cc{0};
            for (int mc = 0; mc < col(); ++mc) {
                if (c == mc) continue;
                minor.at(mr - 1, cc++) = at(mr, mc);
            }
        }

        result += sign * pivot * minor.det();
    }
    return result;
}

template<typename T>
T MatrixX<T>::trace() const
{
    assert(row() == col());
    T result{0};
    for (int r = 0; r < row(); ++r) {
        result += at(r, r);
    }
    return result;
}

template<typename T>
T MatrixX<T>::norm2() const
{
    assert(col() == 1);

    const auto matrix_size{size()};
    T result{0};
    for (int i = 0; i < matrix_size; ++i) {
        result += at(i) * at(i);
    }
    return std::sqrt(result);
}

template<typename T>
T MatrixX<T>::norm1() const
{
    assert(col() == 1);

    const auto matrix_size{size()};
    T result{0};
    for (int i = 0; i < matrix_size; ++i) {
        result += std::abs(at(i));
    }
    return result;
}

template<typename T>
MatrixX<T> MatrixX<T>::sub(int sr, int sc, int r, int c) const
{
    // check
    assert(sr >= 0 && sc >= 0);
    assert(sr + r <= row() && sc + c <= col());

    if (sr == 0 && sc == 0 && r == row() && c == col()) {
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
    std::fill(array_.data_.begin(), array_.data_.end(), 0);
}

template<typename T>
void MatrixX<T>::fill_1_()
{
    const auto matrix_size{size()};
    std::fill(array_.data_.begin(), array_.data_.end(), 1);
}

template<typename T>
void MatrixX<T>::resize_(int r, int c)
{
    array_.col_ = c;
    array_.row_ = r;
    array_.data_.resize(c * r);
}

template<typename T>
MatrixX<T> MatrixX<T>::operator*(const MatrixX& m) const
{
    MatrixX<T> out{row(), m.col()};
    for (int r = 0; r < out.row(); ++r) {
        for (int c = 0; c < out.col(); ++c) {
            T v{0};
            for (int c1 = 0; c1 < col(); ++c1) {
                v += at(r, c1) * m.at(c1, c);
            }
            out.at(r, c) = v;
        }
    }
    return out;
}

template<typename T>
MatrixX<T> operator*(T v, const MatrixX<T>& m)
{
    MatrixX<T> out{m};
    auto matrix_size{m.size()};
    for (int i = 0; i < matrix_size; ++i) {
        out.at(i) *= v;
    }
    return out;
}

template<typename T>
bool MatrixX<T>::is_pd_psd(bool psd) const
{
    // Sylvester's criterion
    assert(col() == row());

    for (int i = 0; i < col(); ++i) {
        T d{sub(i, i, row() - i, col() - i).det()};
        if (d < 0 || (!psd && d == 0)) {
            return false;
        }
    }
    return true;
}

template<typename T>
bool MatrixX<T>::is_sym() const
{
    if (row() != col()) return false;
    if (row() == 1) return true;

    for (int r = 0; r < row(); ++r) {
        for (int c = r + 1; c < col(); ++c) {
            if (at(r, c) != at(c, r)) return false;
        }
    }
    return true;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const MatrixX<T>& m)
{
    os << "Matrix" << typeid(T).name() << "<" << m.row() << ", " << m.col() << ">{";
    os << std::fixed << std::setprecision(QS_PRINT_PRECISION);
    for (int i = 0; i < m.size(); ++i) {
        if (i % m.col() == 0) { os << "\n  "; }
        os << m.at(i) << ", ";
    }
    os << "\n}";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Array<T>& a)
{
    os << "Array" << typeid(T).name() << "<" << a.size() << ">{";
    os << std::fixed << std::setprecision(QS_PRINT_PRECISION);
    const auto array_size{a.size()};
    for (int i = 0; i < array_size; ++i) {
        os << a.at(i);
        if (i != array_size - 1) os << ", ";
    }
    os << "}";
    return os;
}

} // namespace qs

#endif // QS_HPP_
