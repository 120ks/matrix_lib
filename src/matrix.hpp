#pragma once
#include "matrix_subtypes.hpp"
#include "matrix_container.hpp"
#include "matrix_method.hpp"
#include <type_traits>
#include <cassert>

template <typename T>
class Matrix_view;

template<typename T>
class Mask_view;

template<typename T>
class Matrix : public Matrix_container<T>
{   
//
//CONSTRUCTORS
//
    public:
        explicit Matrix(const size_t row_count = 1, const size_t col_count = 1, T val = T{})
        : Matrix_container<T>(row_count, col_count, val)
        {}

        Matrix(const Matrix<T> &other)
        : Matrix_container<T>(other)
        {}  

        Matrix(Matrix<T> &&other) noexcept
        : Matrix_container<T>(std::move(other))
        {}

        Matrix(const Matrix_view<T> &mxv)
        : Matrix(mxv.get_rows(),  mxv.get_cols())
        {
            for(size_t r{} ; r<this->rows; r++){
                for(size_t c{} ; c<this->cols ; c++){
                    (*this)(r, c) = mxv(r, c);
                }
            }
        }

        Matrix(const Matrix_base<T> &mxb)
        : Matrix{matrix_method::copy(mxb)}
        {}
        
        ~Matrix()override = default;
//
//SLICE AND MASK
//
        Matrix_view<T> operator()(Slice r_slice, Slice c_slice)
        {   
            r_slice.end = (r_slice.end == 0) ? this->rows : r_slice.end;
            c_slice.end = (c_slice.end == 0) ? this->cols : c_slice.end;

            T* start_ptr{&((*this)(r_slice.begin, c_slice.begin))};
            return {start_ptr, r_slice, c_slice, this->rows, this->cols};
        }   
        
        Mask_view<T> operator()(Matrix<boolean> mask){
            return {*this, mask};
        }

//
// OVERRIDES
//        
        T& operator()(const size_t row, const size_t col)override{
            return this->matrix[row*this->cols + col];
        }
        
        T operator()(const size_t row, const size_t col)const override{
            return this->matrix[row*this->cols + col];
        }

        inline size_t get_rows()const override {return this->rows;}
        inline size_t get_cols()const override {return this->cols;}
        
        inline size_t matrix_rows()const override {return this->rows;}
        inline size_t matrix_cols()const override {return this->cols;}


        inline T* start_ptr()const override{return this->first_val_address;}
//
// ASSIGN OPERATORS
// 
        Matrix& operator=(Matrix other)
        {   
            this->rows = other.rows;
            this->cols = other.cols;
            std::swap(this->matrix, other.matrix);
            return *this;
        }

        Matrix& operator=(const Matrix_view<T> &mxv)
        {
            *this = Matrix{mxv};
            return *this;
        }

        Matrix& operator=(const Matrix_base<T> &mxb1)
        {
            *this = matrix_method::copy(mxb1);
            return *this;
        }

//
//MEMBER ARITHMETIC OPERATORS
//
        Matrix& operator+=(const Matrix_base<T>& other)
        {
            matrix_method::broadcast(*this, other, Operation::add);
            return *this;
        }

        Matrix& operator+=(const T val)
        {
            matrix_method::broadcast(*this, val, Operation::add);
            return *this;
        }

        Matrix& operator-=(const Matrix_base<T>& other)
        {
            matrix_method::broadcast(*this, other, Operation::substract);
            return *this;
        }

        Matrix& operator-=(const T val)
        {
            matrix_method::broadcast(*this, val, Operation::substract);
            return *this;
        }

        Matrix& operator*=(const Matrix_base<T>& other)
        {
            matrix_method::broadcast(*this, other, Operation::multiply);
            return *this;
        }

        Matrix& operator*=(const T val)
        {
            matrix_method::broadcast(*this, val, Operation::multiply);
            return *this;
        }

        Matrix& operator/=(const Matrix_base<T>& other)
        {
            matrix_method::broadcast(*this, other, Operation::divide);
            return *this;
        }

        Matrix& operator/=(const T val)
        {
            matrix_method::broadcast(*this, val, Operation::divide);
            return *this;
        }
//
//NOT OPERATOR
//      
Matrix<boolean> operator!() = delete; //Only Matrix<boolean> can use
};

template<>
Matrix<boolean> Matrix<boolean>::operator!()
{   
    Matrix<boolean> mask{*this};
    for(auto& val : mask.matrix){
        val = !val;
    }
    return mask;
}


namespace boolean_logic
{   
    template<typename T, typename R>
    boolean check_condition(const T val, const Condition condition, const R threshold)
    {   
        switch(condition)
        {   
            case(Condition::is_true): return true;
            case(Condition::is_false): return false;
            case(Condition::over): return (val > threshold) ? true : false;
            case(Condition::over_or): return (val >= threshold) ? true : false;
            case(Condition::under): return (val < threshold) ? true : false;
            case(Condition::under_or): return (val <= threshold) ? true : false;
            case(Condition::is) : return (val == threshold) ? true : false;
            case(Condition::is_not): return (val != threshold) ? true : false;
        }
        return false;
    }

    template<typename T, typename R>
    Matrix<boolean> make_mask(const Matrix_base<T> &mxb1, const Condition condition, const R threshold)
    {
        size_t r1{mxb1.get_rows()};
        size_t c1{mxb1.get_cols()};
        Matrix<boolean> mask{r1, c1, boolean{false}};

        using common_type = std::common_type_t<T, R>;
        auto N_threshold {static_cast<common_type> (threshold)};

        for(size_t r{} ; r<r1 ; r++){
            for(size_t c{} ; c<c1 ; c++)
            {
                auto current_element = static_cast<common_type>(mxb1(r, c));
                mask(r, c) = check_condition(current_element, condition, N_threshold);
            }
        }
        return mask;
    }

    template<typename T, typename R>
    Matrix<boolean> make_mask(const Matrix_base<T> &mxb1, const Condition condition, const Matrix_base<R> &mxb2)
    {   

        if(mxb1.get_rows() != mxb2.get_rows()){
            std::cerr<<"make_mask row mismatch\n"; //change to throw
        }
        if(mxb1.get_cols() != mxb2.get_cols()){
            std::cerr<<"make_mask col mismatch\n"; //change to throw
        }

        size_t r1{mxb1.get_rows()};
        size_t c1{mxb1.get_cols()};
        Matrix<boolean> mask{r1, c1, boolean{false}};

        using common_type = std::common_type_t<T, R>;

        for(size_t r{} ; r<r1 ; r++){
            for(size_t c{} ; c<c1 ; c++){

                auto mxb1_current = static_cast<common_type>(mxb1(r, c));
                auto mxb2_current = static_cast<common_type>(mxb2(r, c));
                mask(r, c) = check_condition(mxb1_current, condition, mxb2_current);
            }
        }
        return mask;
    }
}


