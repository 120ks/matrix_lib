#pragma once

#include "matrix_base.hpp"
#include "matrix_method.hpp"
#include "matrix_subtypes.hpp"

template<typename T>
class Slice_view : public Matrix_base<T>
{   
    template<typename>
    friend class Matrix;

    template<typename>
    friend class Mask_view;

    private:
        T* view_ptr{};
        Slice row_slice{};
        Slice col_slice{};

        size_t base_rows{};
        size_t base_cols{};
    protected:
//
//CONSTRUCTORS
//
        Slice_view(T* start, Slice r_slice, Slice c_slice, const size_t total_rows, const size_t total_cols)
        : view_ptr{start}
        , row_slice{r_slice}
        , col_slice{c_slice}
        , base_rows{total_rows}
        , base_cols{total_cols}
        {
            row_slice.end = (row_slice.end == 0) ? base_rows : row_slice.end; 
            col_slice.end = (col_slice.end == 0) ? base_cols : col_slice.end; 
        }


        Slice_view(const Slice_view &other)
        : view_ptr{other.view_ptr}
        , row_slice{other.row_slice}
        , col_slice{other.col_slice}
        , base_rows{other.base_rows}
        , base_cols{other.base_cols}
        {}


        Slice_view(Slice_view &&other)noexcept
        : view_ptr{other.view_ptr}
        , row_slice{other.row_slice}
        , col_slice{other.col_slice}
        , base_rows{other.base_rows}
        , base_cols{other.base_cols}
        {other.view_ptr = nullptr;}

    public:
        ~Slice_view()override = default;
//
// OVERRIDES
//        
        inline size_t get_rows()const override {return row_slice.end - row_slice.begin;}
        inline size_t get_cols()const override {return col_slice.end - col_slice.begin;}
        inline size_t matrix_rows()const override {return base_rows;}
        inline size_t matrix_cols()const override {return base_cols;}
        Type derived_type()const override {return Type::slice;}


        T& operator()(const size_t row, const size_t col)override
        {   
            return *(view_ptr+(row*base_cols) + col);
        }

        const T& operator()(const size_t row, const size_t col)const override
        {   
            return *(view_ptr+(row*base_cols) + col);
        }
//
//SLICING AND MASKING
//      
        Slice_view& operator()(const Slice r_slice, const Slice c_slice)
        {   
            T* new_start{view_ptr+(r_slice.begin * base_cols + c_slice.begin)};
            row_slice = row_slice+r_slice;
            col_slice = col_slice+c_slice;

            return{*this};
        }
        Mask_view<T> operator[](Matrix<boolean> mask){ //note the  []
            return{*this, mask};
        }
        
//
// ASSIGN OPERATORS
//  
        template<typename R>
        Slice_view& operator=(const Matrix_base<R> &mxb1)
        {
            matrix_method::broadcast(*this, mxb1, Operation::equal);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Slice_view& operator=(const R val)
        {
            matrix_method::broadcast(*this, val, Operation::equal);
            return *this;
        }

        template<typename R>
        Slice_view& operator=(const Slice_view<R> &mxv1)
        {
            matrix_method::broadcast(*this, mxv1, Operation::equal);
            return *this;
        }
//
//MEMBER ARITHMETIC OPERATORS
//      
        template<typename R>
        Slice_view& operator+=(const Matrix_base<R>& other)
        {
            matrix_method::broadcast(*this, other, Operation::add);
            return *this;
        }
        
        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Slice_view& operator+=(const R val)
        {
            matrix_method::broadcast(*this, val, Operation::add);
            return *this;
        }

        template<typename R>
        Slice_view& operator-=(const Matrix_base<R>& other)
        {
            matrix_method::broadcast(*this, other, Operation::substract);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Slice_view& operator-=(const R val)
        {
            matrix_method::broadcast(*this, val, Operation::substract);
            return *this;
        }
        template<typename R>
        Slice_view& operator*=(const Matrix_base<R>& other)
        {
            matrix_method::broadcast(*this, other, Operation::multiply);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Slice_view& operator*=(const R val)
        {
            matrix_method::broadcast(*this, val, Operation::multiply);
            return *this;
        }

        template<typename R>
        Slice_view& operator/=(const Matrix_base<R>& other)
        {
            matrix_method::broadcast(*this, other, Operation::divide);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Slice_view& operator/=(const R val)
        {
            matrix_method::broadcast(*this, val, Operation::divide);
            return *this;
        }
};

