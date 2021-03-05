/**
 * @author:      0.382
 * @description: 矩阵的定义以及一些基础的函数
 */

#pragma once
#ifndef UTIL_MATRIX_HPP
#define UTIL_MATRIX_HPP

#include "easyprint.hpp"
#include <iomanip>
#include <tuple>

namespace util
{

template <typename T = double> class Matrix
{
  public:
    using value_type = T;

  private:
    value_type *_data;
    std::size_t _row;
    std::size_t _col;

  public:
    Matrix(std::size_t row, std::size_t col, value_type value = 0) : _row(row), _col(col)
    {
        if (row <= 0 || col <= 0)
            stop("(constructor) invalid matrix size: row = ", row, ", col = ", col);
        _data = new value_type[row * col * sizeof(value_type)];
        for (std::size_t i = 0; i < row * col; ++i)
        {
            _data[i] = value;
        }
    }
    Matrix(std::tuple<std::size_t, std::size_t> _size, value_type value = 0)
        : Matrix(std::get<0>(_size), std::get<1>(_size), value)
    {
    }
    Matrix(const Matrix &m)
    {
        _row = m._row;
        _col = m._col;
        _data = new value_type[_row * _col * sizeof(value_type)];
        for (std::size_t i = 0; i < _row * _col; ++i)
        {
            _data[i] = m._data[i];
        }
    }
    Matrix(Matrix &&m) noexcept
    {
        _row = m._row;
        _col = m._col;
        _data = m._data;
        m._data = nullptr;
    }
    Matrix &operator=(const Matrix &m)
    {
        if (&m == this)
            return *this;
        this->reallocate(m.total_size());
        _row = m._row;
        _col = m._col;
        for (std::size_t i = 0; i < _row * _col; ++i)
        {
            _data[i] = m._data[i];
        }
        return *this;
    }
    Matrix &operator=(Matrix &&m) noexcept
    {
        if (&m == this)
            return *this;
        this->clear();
        _row = m._row;
        _col = m._col;
        _data = m._data;
        m._data = nullptr;
        return *this;
    }
    ~Matrix()
    {
        this->clear();
    }

    void clear()
    {
        delete[] _data;
        _data = nullptr;
    }

    void set_value(std::initializer_list<value_type> list)
    {
        std::size_t i = 0;
        for (auto v : list)
        {
            _data[i] = v;
            ++i;
            if (i > total_size())
                break;
        }
    }

    explicit operator bool()
    {
        return this->_data != nullptr;
    }

    template <typename U> operator Matrix<U>()
    {
        Matrix<U> mu(this->size());
        for (std::size_t i = 0; i < this->total_size(); ++i)
        {
            mu(i) = static_cast<U>(_data[i]);
        }
        return mu;
    }

    std::size_t rows() const
    {
        return _row;
    }
    std::size_t cols() const
    {
        return _col;
    }

    std::tuple<std::size_t, std::size_t> size() const
    {
        return std::make_tuple(_row, _col);
    }

    std::size_t total_size() const
    {
        return _row * _col;
    }

    // 笛卡尔坐标索引
    value_type operator()(std::size_t i, std::size_t j) const
    {
        return _data[i * _col + j];
    }
    value_type &operator()(std::size_t i, std::size_t j)
    {
        return _data[i * _col + j];
    }

    // 线性索引
    value_type operator()(std::size_t n) const
    {
        return _data[n];
    }

    value_type &operator()(std::size_t n)
    {
        return _data[n];
    }

    void resize(std::size_t row, std::size_t col)
    {
        if (row <= 0 || col <= 0)
            stop("(resize) invalid matrix size: row = ", row, ", col = ", col);
        this->reallocate(row * col);
        _row = row;
        _col = col;
    }

    void reshape(std::size_t row, std::size_t col)
    {
        if (row * col != _row * _col)
            stop("(reshape) reshape should not change the total size, with row = ", row, ", col = ", col);
        _row = row;
        _col = col;
    }

    // 比较算符
    friend bool operator==(const Matrix &ma, const Matrix &mb)
    {
        if (ma.size() != mb.size())
            return false;
        for (std::size_t i = 0; i < ma.total_size(); ++i)
        {
            if (ma._data[i] != mb._data[i])
                return false;
        }
        return true;
    }

    friend bool approx(const Matrix &ma, const Matrix &mb, value_type eps)
    {
        if (ma.size() != mb.size())
            return false;
        for (std::size_t i = 0; i < ma.total_size(); ++i)
        {
            if (std::abs(ma._data[i] - mb._data[i]) > eps)
                return false;
        }
        return true;
    }

    // 一元运算符
    friend Matrix operator+(const Matrix &m)
    {
        return m;
    }

    friend Matrix operator-(const Matrix &m)
    {
        Matrix mm(m.size());
        for (std::size_t i = 0; i < m.total_size(); ++i)
        {
            mm._data[i] = -m._data[i];
        }
        return mm;
    }

    // 二元运算符
    friend Matrix operator+(const Matrix &ma, const Matrix &mb)
    {
        if (ma.size() != mb.size())
            stop("(Matrix +) matrix sizes are not equal");
        Matrix mc(ma.size());
        for (std::size_t i = 0; i < ma.total_size(); ++i)
        {
            mc._data[i] = ma._data[i] + mb._data[i];
        }
        return mc;
    }

    friend Matrix operator-(const Matrix &ma, const Matrix &mb)
    {
        if (ma.size() != mb.size())
            stop("(Matrix -) matrix sizes are not equal");
        Matrix mc(ma.size());
        for (std::size_t i = 0; i < ma.total_size(); ++i)
        {
            mc._data[i] = ma._data[i] - mb._data[i];
        }
        return mc;
    }

    // 乘除法默认是按位乘，矩阵乘法另外写成 `matmul`
    friend Matrix operator*(const Matrix &ma, const Matrix &mb)
    {
        if (ma.size() != mb.size())
            stop("(Matrix *) matrix sizes are not equal");
        Matrix mc(ma.size());
        for (std::size_t i = 0; i < ma.total_size(); ++i)
        {
            mc._data[i] = ma._data[i] * mb._data[i];
        }
        return mc;
    }

    friend Matrix operator/(const Matrix &ma, const Matrix &mb)
    {
        if (ma.size() != mb.size())
            stop("(Matrix -) matrix sizes are not equal");
        Matrix mc(ma.size());
        for (std::size_t i = 0; i < ma.total_size(); ++i)
        {
            mc._data[i] = ma._data[i] / mb._data[i];
        }
        return mc;
    }

    friend Matrix matmul(const Matrix &ma, const Matrix &mb)
    {
        if (ma.cols() != mb.rows())
            stop("(matmul) Matrix a's cols should be equal to Matrix b's rows");
        Matrix mc(ma.rows(), mb.cols());
        for (std::size_t i = 0; i < ma.rows(); ++i)
        {
            for (std::size_t k = 0; k < ma.cols(); ++k)
            {
                for (std::size_t j = 0; j < mb.cols(); ++j)
                {
                    mc._data[i * mc._col + j] += ma(i, k) * mb(k, j);
                }
            }
        }
        return mc;
    }

    // 数乘
    friend Matrix operator*(value_type k, const Matrix &m)
    {
        Matrix mr(m.size());
        for (std::size_t i = 0; i < m.total_size(); ++i)
        {
            mr._data[i] = k * m._data[i];
        }
        return mr;
    }

    friend Matrix operator*(const Matrix &m, value_type k)
    {
        Matrix mr(m.size());
        for (std::size_t i = 0; i < m.total_size(); ++i)
        {
            mr._data[i] = k * m._data[i];
        }
        return mr;
    }

    friend Matrix operator/(const Matrix &m, value_type k)
    {
        Matrix mr(m.size());
        for (std::size_t i = 0; i < m.total_size(); ++i)
        {
            mr._data[i] = m._data[i] / k;
        }
        return mr;
    }

    friend void swap(Matrix &ma, Matrix &mb)
    {
        std::swap(ma._row, mb._row);
        std::swap(ma._col, mb._col);
        std::swap(ma._data, mb._data);
    }

  private:
    // 重新申请空间
    void reallocate(std::size_t total_size)
    {
        if (total_size != _row * _col)
        {
            delete[] _data;
            _data = new value_type[total_size * sizeof(value_type)];
        }
    }
};

// 输出到流
template <typename T> std::ostream &operator<<(std::ostream &os, const Matrix<T> &m)
{
    for (std::size_t i = 0; i < m.rows(); ++i)
    {
        for (std::size_t j = 0; j < m.cols(); ++j)
            os << std::setw(10) << m(i, j);
        os << '\n';
    }
    return os;
}

// 单位矩阵
template <typename T> Matrix<T> eye(std::size_t n)
{
    Matrix<T> m(n, n);
    for (std::size_t i = 0; i < n; ++i)
    {
        m(i, i) = 1;
    }
    return m;
}

// 求迹
template <typename T> T trace(const Matrix<T> &m)
{
    T sum = 0;
    for (std::size_t i = 0; i < std::min(m.row(), m.col()); ++i)
    {
        sum += m(i, i);
    }
    return sum;
}

// 转置
template <typename T> Matrix<T> transpose(const Matrix<T> &m)
{
    Matrix<T> mt(m.cols(), m.row());
    for (std::size_t i = 0; i < m.rows(); ++i)
    {
        for (std::size_t j = 0; j < m.cols(); ++j)
        {
            mt(j, i) = m(i, j);
        }
    }
    return mt;
}

} // end namespace util

#endif // UTIL_MATRIX_HPP