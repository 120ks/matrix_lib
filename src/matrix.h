#pragma once

#include "matrix_container.h"
#include "matrix_view.h"
#include "matrix_mask.h"

class Matrix_view;
class Matrix_method;


class Matrix : public Matrix_container
{   
    friend class Matrix_method;
    friend class Matrix_view;
    public:
        explicit Matrix(const size_t row_count = 1, const size_t col_count = 1, const double val = 0);
        Matrix(const Matrix &other);
        Matrix(Matrix &&other) noexcept;
        Matrix(const Matrix_view &mxv);
        Matrix(const Matrix_base &mxb);
        ~Matrix()override = default;

        //overrides
        inline size_t get_cols()const override {return cols;}
        inline size_t get_rows()const override {return rows;}

        double& operator()(const size_t row, const size_t col)override;
        double operator()(const size_t row, const size_t col)const override;
        Matrix_view operator()(const Slice r_slice, const Slice c_slice)override;

        inline size_t begin_row()const override {return 0;};
        inline size_t begin_col()const override {return 0;};

        //operators
        Matrix& operator=(Matrix other);
        Matrix& operator=(const Matrix_view &mxv);
        Matrix& operator=(const Matrix_base &mxb1);

        Matrix& operator+=(const Matrix_base& other);
        Matrix& operator-=(const Matrix_base& other);
        Matrix& operator*=(const Matrix_base& other);
        Matrix& operator/=(const Matrix_base& other);

        Matrix& operator+=(const double val);
        Matrix& operator-=(const double val);
        Matrix& operator*=(const double val);
        Matrix& operator/=(const double val);

        friend Matrix operator+(const Matrix_base& mxb1, const Matrix_base &mxb2);
        friend Matrix operator-(const Matrix_base& mxb1, const Matrix_base &mxb2);
        friend Matrix operator*(const Matrix_base& mxb1, const Matrix_base &mxb2);
        friend Matrix operator/(const Matrix_base& mxb1, const Matrix_base &mxb2);
        
        friend Matrix operator+(const Matrix_base& mxb1, const double val);
        friend Matrix operator-(const Matrix_base& mxb1, const double val);
        friend Matrix operator*(const Matrix_base& mxb1, const double val);
        friend Matrix operator/(const Matrix_base& mxb1, const double val);

        //
        //COMPARISON OPERATORS
        //
        friend Matrix_mask operator>(const Matrix_base &mxb1, const double val);
        friend Matrix_mask operator<(const Matrix_base &mxb1, const double val);
        friend Matrix_mask operator>=(const Matrix_base &mxb1, const double val);
        friend Matrix_mask operator<=(const Matrix_base &mxb1, const double val);
        friend Matrix_mask operator==(const Matrix_base &mxb1, const double val);
        friend Matrix_mask operator!=(const Matrix_base &mxb1, const double val);

        friend Matrix_mask operator>(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Matrix_mask operator<(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Matrix_mask operator>=(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Matrix_mask operator<=(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Matrix_mask operator==(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Matrix_mask operator!=(const Matrix_base &mxb1, const Matrix_base &mxb2);
        

};


