#pragma once

#include "matrix.h"
#include <memory>
class Matrix;

class Matrix_view : public Matrix_base
{   
    friend class Matrix;
    friend class Matrix_method;
    private:
        Matrix* view_ptr{};
        size_t row_begin{};
        size_t col_begin{};
        size_t rows{};
        size_t cols{};

    protected:
        
        Matrix_view(Matrix &mx, size_t r_begin, size_t r_end, size_t c_begin, size_t c_end);
        Matrix_view(const Matrix_view &other);
        Matrix_view(Matrix_view &&other)noexcept;

    public:
        ~Matrix_view()override = default;
        //overrides
        inline size_t get_cols()const override {return cols;}
        inline size_t get_rows()const override {return rows;}
        double& operator()(const size_t row, const size_t col)override;
        double operator()(const size_t row, const size_t col)const override;

        //huom, pidetään by val palautuksena kunnes mahd testin avulla todetaan että voi palauttaa by ref
        //muutenkin aika vähän käytettävä operaattori.
        Matrix_view operator()(size_t r_begin, size_t r_end, size_t c_begin, size_t c_end)override;

        Matrix_view& operator=(const Matrix_base& mxb1);
        Matrix_view& operator=(const Matrix_view& mxv1);
        Matrix_view& operator=(const double val);

        Matrix_view& operator+=(const Matrix_base& other);
        Matrix_view& operator-=(const Matrix_base& other);
        Matrix_view& operator*=(const Matrix_base& other);
        Matrix_view& operator/=(const Matrix_base& other);

        Matrix_view& operator+=(const double val);
        Matrix_view& operator-=(const double val);
        Matrix_view& operator*=(const double val);
        Matrix_view& operator/=(const double val);

        friend Matrix operator+(const Matrix_base& mxb1, const Matrix_base &mxb2);
        friend Matrix operator-(const Matrix_base& mxb1, const Matrix_base &mxb2);
        friend Matrix operator*(const Matrix_base& mxb1, const Matrix_base &mxb2);
        friend Matrix operator/(const Matrix_base& mxb1, const Matrix_base &mxb2);

        friend Matrix operator+(const Matrix_base& mxb1, const double val);
        friend Matrix operator-(const Matrix_base& mxb1, const double val);
        friend Matrix operator*(const Matrix_base& mxb1, const double val);
        friend Matrix operator/(const Matrix_base& mxb1, const double val);
};

