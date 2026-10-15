#pragma once

#include "matrix.h"
#include "matrix_method.h"
#include <memory>
class Matrix;
class Mask;
class Matrix_view : public Matrix_base
{   
    friend class Matrix;
    friend class Matrix_method;
    private:
        double* view_ptr{};
        size_t start_row{};
        size_t start_col{};
        size_t end_row{};
        size_t end_col{};

        size_t base_rows{};
        size_t base_cols{};
    protected:
        
        Matrix_view(double* start, Slice r_slice, Slice c_slice, const size_t total_rows, const size_t total_cols);
        Matrix_view(const Matrix_view &other);
        Matrix_view(Matrix_view &&other)noexcept;

    public:
        ~Matrix_view()override = default;
        //overrides
        
        inline size_t get_rows()const override {return end_row - start_row;}
        inline size_t get_cols()const override {return end_col - start_col;}

        double& operator()(const size_t row, const size_t col)override;
        double operator()(const size_t row, const size_t col)const override;
        Matrix_view operator()(Slice r_slice, Slice c_slice);

        inline size_t matrix_rows()const override {return base_rows;}
        inline size_t matrix_cols()const override {return base_cols;}
        inline size_t begin_row()const override {return start_row;}
        inline size_t begin_col()const override {return start_col;}
        
        inline double* matrix_start_ptr()const override{return view_ptr-(start_row*base_rows+start_col);}
        inline double* start_ptr()const override{return view_ptr;}

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
        friend Mask operator>(const Matrix_base &mxb1, const double val);
        friend Mask operator<(const Matrix_base &mxb1, const double val);
        friend Mask operator>=(const Matrix_base &mxb1, const double val);
        friend Mask operator<=(const Matrix_base &mxb1, const double val);
        friend Mask operator==(const Matrix_base &mxb1, const double val);
        friend Mask operator!=(const Matrix_base &mxb1, const double val);

        friend Mask operator>(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Mask operator<(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Mask operator>=(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Mask operator<=(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Mask operator==(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Mask operator!=(const Matrix_base &mxb1, const Matrix_base &mxb2);

};

