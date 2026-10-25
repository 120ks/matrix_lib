#pragma once
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include <random>
#include "matrix_exceptions.hpp"

template<typename T>
class Matrix;

template<typename T>
class Slice_view;

namespace machine
{

    
}
    template<typename T>
    class Matrix_base
    {   

        protected:
            Matrix_base() = default;
            virtual ~Matrix_base() = default;

        public:
            virtual T& operator()(const size_t, const size_t) = 0;
            virtual const T& operator()(const size_t, const size_t) const = 0;

            virtual inline size_t get_rows()const = 0;
            virtual inline size_t get_cols()const = 0;
            virtual Matrix<T> transpose()const = 0;
            virtual Matrix<T> copy()const = 0;
    };


//
//enums and statics
//
enum class Operation{
    multiply,
    divide,
    add,
    substract,
    modulo,
    equal,
    power,
    root
};

enum class axis
{
    X,
    Y
};
namespace mx_math
{
    static std::random_device random_device;
    static std::mt19937 generator(random_device());
}
//
//FLAG SKELETONS
//
template<typename T>
class mask_flag
{   
    friend class Matrix<T>;
    friend class Slice_view<T>;
    public:
        Matrix_base<T>* ptr{};
        mask_flag(Matrix_base<T> &mxb1) : ptr{&mxb1}{}
        mask_flag(const mask_flag<T>&) = delete;
        mask_flag(mask_flag<T> &&other) : ptr{other.ptr} {other.ptr = nullptr;}
        operator=(mask_flag<T>&&) = delete;
        operator=(const mask_flag<T>&) = delete;
    public:
        ~mask_flag() = default;
    //
    //comparison operators
    //  
        friend mask_flag<T> Matrix<T>::M();
        friend mask_flag<T> Slice_view<T>::M();

};

template<typename T>
class assign_flag
{
    friend class Matrix<T>;

    public:
        Matrix<T>* ptr{};
        assign_flag(Matrix<T> &mrx);
        assign_flag(const assign_flag<T>&) = delete;
        assign_flag(assign_flag<T> &&other) : ptr{other.ptr} {other.ptr = nullptr;}
        operator=(assign_flag<T>&&) = delete;
        operator=(const assign_flag<T>&) = delete;

        template<typename R>
        Matrix<T>& operator=(const Matrix_base<R> &mxb1);
        Matrix<T>& operator=(Matrix<T> &&mx);
};

//
//BROADCAST OPERATIONS
//
namespace method
{
    template<typename T>
    bool has_zero(const Matrix_base<T> &mxb1)
    {   
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};

        for(size_t r{} ; r<r1 ; r++){
            for(size_t c{} ; c<c1 ; c++){
                if(mxb1(r, c) == 0){return true;}
            }
        }
        return false;
    }
    template<typename T, typename R>
    void broadcast(Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2, const Operation op)
    {   
        if((mxb1.get_rows() == 0) || (mxb1.get_cols() == 0)) {return;}
        
        auto total_rows1{mxb1.get_rows()}, total_rows2{mxb2.get_rows()};
        auto total_cols1{mxb1.get_cols()}, total_cols2{mxb2.get_cols()};

        if(op == Operation::divide || op == Operation::root){
            if(has_zero(mxb2)){throw std::runtime_error("division/root of mxb1 with mxb2-> contains 0");}
        }

        size_t max_row{std::max(total_rows1, total_rows2)};
        size_t max_col{std::max(total_cols1, total_cols2)};
        
        size_t mxb1_row{}, mxb2_row{};
        size_t mxb1_col{}, mxb2_col{};

        for(size_t r{0} ; r<max_row ; r++)
        {   
            for(size_t c{0} ; c<max_col ; c++)
            {
                switch(op){
                case(Operation::multiply)   : mxb1(mxb1_row, mxb1_col) *= mxb2(mxb2_row, mxb2_col); break;
                case(Operation::divide)     : mxb1(mxb1_row, mxb1_col) /= mxb2(mxb2_row, mxb2_col); break;
                case(Operation::add)        : mxb1(mxb1_row, mxb1_col) += mxb2(mxb2_row, mxb2_col); break;
                case(Operation::substract)  : mxb1(mxb1_row, mxb1_col) -= mxb2(mxb2_row, mxb2_col); break;
                case(Operation::equal)      : mxb1(mxb1_row, mxb1_col) =  mxb2(mxb2_row, mxb2_col); break;
                case(Operation::power)      : mxb1(mxb1_row, mxb1_col) =  std::pow(mxb1(mxb1_row, mxb1_col), mxb2(mxb2_row, mxb2_col)); break;
                case(Operation::root)       : mxb1(mxb1_row, mxb1_col) =  std::pow(mxb1(mxb1_row, mxb1_col), 1.0 / mxb2(mxb2_row, mxb2_col)); break;
                }
                (mxb1_col < total_cols1 - 1) ? mxb1_col++ : mxb1_col = 0; 
                (mxb2_col < total_cols2 - 1) ? mxb2_col++ : mxb2_col = 0;
            }
            (mxb1_row < total_rows1 - 1) ? mxb1_row++ : mxb1_row = 0;
            (mxb2_row < total_rows2 - 1) ? mxb2_row++ : mxb2_row = 0;
        }
    }
    template<typename T, typename R>
    requires (std::integral<R> || std::floating_point<R>)
    void broadcast(Matrix_base<T> &mxb1, const R &value, const Operation op)
    {
        if((mxb1.get_rows() == 0) || (mxb1.get_cols() == 0)) {return;}

        if(op == Operation::divide || op == Operation::root){
            if(value == 0) {throw std::runtime_error("division/root by 0");}
        }
        for(size_t r{0} ; r<mxb1.get_rows(); r++)
        {   
            for(size_t c{0} ; c<mxb1.get_cols() ; c++)
            {
                switch(op){
                case(Operation::multiply)   : mxb1(r, c) *= value; break;
                case(Operation::divide)     : mxb1(r, c) /= value; break;
                case(Operation::add)        : mxb1(r, c) += value; break;
                case(Operation::substract)  : mxb1(r, c) -= value; break;
                case(Operation::equal)      : mxb1(r, c) =  value; break;
                case(Operation::power)      : mxb1(r, c) =  std::pow(mxb1(r, c), value); break;
                case(Operation::root)       : mxb1(r, c) =  std::pow(mxb1(r, c), 1.0 / value); break;
                }
            }
        }
    }
    //
    //R value broadcasting
    //
    template<typename T, typename R>
    Matrix<T> broadcast(Matrix_base<T> &&mxb1, const Matrix_base<R> &mxb2, const Operation op)
    {
        broadcast(mxb1, mxb2, op);
        return {std::move(mxb1)};
    }

    template<typename T, typename R>
    Matrix<T> broadcast(Matrix_base<T> &&mxb1, const R &value, const Operation op)
    {
        broadcast(mxb1, value, op);
        return {std::move(mxb1)};
    }
}
