#pragma once

#include "matrix.h"
#include "matrix_method.h"
#include <memory>
class Matrix;

class Matrix_view : public Matrix_base
{   
    friend class Matrix;
    friend class Matrix_method;
    private:
        Matrix* view_ptr{};
        Slice row_slice;
        Slice col_slice;

    
    protected:
        
        Matrix_view(Matrix &mx, const Slice r_slice, const Slice c_slice);
        Matrix_view(Matrix_base &mxb1, Condition c, const double filter, const Slice r_slice = {}, const Slice c_slice = {});
        Matrix_view(const Matrix_view &other);
        Matrix_view(Matrix_view &&other)noexcept;

    public:
        ~Matrix_view()override = default;
        //overrides
        inline size_t get_cols()const override {return col_slice.total;}
        inline size_t get_rows()const override {return row_slice.total;}
        double& operator()(const size_t row, const size_t col)override;
        double operator()(const size_t row, const size_t col)const override;

        inline size_t begin_row()const override {return row_slice.begin;}
        inline size_t begin_col()const override {return col_slice.begin;}
        

        //huom, pidetään by val palautuksena kunnes mahd testin avulla todetaan että voi palauttaa by ref
        //muutenkin aika vähän käytettävä operaattori.
        Matrix_view operator()(Slice r_slice, Slice c_slice)override;

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

