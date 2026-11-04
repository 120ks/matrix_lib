#pragma once

#include "matrix_exceptions.hpp"
#include <cmath>

namespace machine
{   
    //
    //Broadcast Optimizers
    //
    template<typename T, typename R>
    requires (std::integral<R> || std::floating_point<R>)
    void broadcast(Matrix<T>& mx1, const R &value, const Operation op)
    {   
        direct_broadcast(mx1.begin(), mx1.end(), value, op);
        return;
    }

    template<typename T, typename R>
    void broadcast(Matrix<T>& mx1, const Matrix<R>& mx2, const Operation op)
    {   
        if(same_dims(mx1, mx2)){
            direct_broadcast(mx1.begin(), mx1.end(), mx2.begin(), op);
            return;
        }

        if(divisible(mx1, mx2))
        {
            tile_broadcast(mx1, mx2, op);
            return;
        }
    }

    template<typename T, typename R>
    void broadcast(Matrix_base<T>& mxb1, const Matrix_base<R>& mxb2, const Operation op)
    {   
        if(same_dims(mxb1, mxb2) && mxb1.begin() && mxb2.begin()){
            direct_broadcast(mxb1.begin(), mxb1.end(), mxb2.begin(), op);
            return;
        }
        if(divisible(mxb1, mxb2))
        {
            tile_broadcast(mxb1, mxb2, op);
            return;
        }
    }
    template<typename T, typename R>
    requires (std::integral<R> || std::floating_point<R>)
    void broadcast(Matrix_base<T> &mxb1, const R &value, const Operation op)
    {
        
        if(mxb1.begin()){
            direct_broadcast(mxb1.begin(), mxb1.end(), value, op);
            return;
        }
        tile_broadcast(mxb1, value, op);
        return;
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
    requires (std::integral<R> || std::floating_point<R>)
    Matrix<T> broadcast(Matrix_base<T> &&mxb1, const R &value, const Operation op)
    {   
        broadcast(mxb1, value, op);
        return {std::move(mxb1)};
    }
}
//
//BROADCAST MACHINE END
//
namespace machine
{   
    template<typename T, typename R>
    inline void direct_broadcast(T* begin1, T* end1, const R* begin2, const Operation op){ 

        auto add = [&]()->void {
            while (begin1 != end1){
                *(begin1++) += *(begin2++);
            }
        };
        auto substract = [&]()->void {
            while (begin1 != end1){
                *(begin1++) -= *(begin2++);
            }
            
        };
        auto multiply = [&]()->void {
            while (begin1 != end1){
                *(begin1++) *= *(begin2++);
            }
        };
        auto divide = [&]()->void {
            while (begin1 != end1){
                *(begin1++) /= *(begin2++);
            }
        };

        auto equal = [&]()->void {
            while (begin1 != end1){
                *(begin1++) = *(begin2++);
            }
        };

        auto power = [&]()->void {
            while (begin1 != end1){
                auto val1 = static_cast<const double>((*begin1));
                auto val2 = static_cast<const double>((*begin2++));
                *(begin1++) = std::pow(val1, val2);
            }
        };
        auto root = [&]()->void {
            while (begin1 != end1){
                auto val1 = static_cast<const double>((*begin1));
                auto val2 = static_cast<const double>((*begin2++));
                *(begin1++) = std::pow(val1, 1/val2);
            }
        };
        switch(op){
        case(Operation::add): add(); return;
        case(Operation::substract): substract(); return;
        case(Operation::multiply): multiply(); return;
        case(Operation::divide): divide(); return;
        case(Operation::equal): equal(); return;
        case(Operation::power): power(); return;
        case(Operation::root): root(); return;
        }
    }

    template<typename T, typename R>
    requires (!std::is_pointer_v<R>)
    inline void direct_broadcast(T* begin1, T* end1, const R val, const Operation op){  
        
        auto add = [&]()->void {
            while (begin1 != end1){
                *(begin1++) += val;
            }
        };
        auto substract = [&]()->void {
            while (begin1 != end1){
                *(begin1++) -= val;
            }
            
        };
        auto multiply = [&]()->void {
            while (begin1 != end1){
                *(begin1++) *= val;
            }
        };
        auto divide = [&]()->void {
            while (begin1 != end1){
                *(begin1++) /= val;
            }
        };

        auto equal = [&]()->void {
            while (begin1 != end1){
                *(begin1++) = val;
            }
        };

        auto power = [&]()->void {
            while (begin1 != end1){
                auto val1 = static_cast<const double>((*begin1));
                auto val2 = static_cast<const double>(val);
                *(begin1++) = std::pow(val1, val2);
            }
        };
        auto root = [&]()->void {
            while (begin1 != end1){
                auto val1 = static_cast<const double>((*begin1));
                auto val2 = static_cast<const double>(val);
                *(begin1++) = std::pow(val1, 1/val2);
            }
        };
        switch(op){
        case(Operation::add): add(); return;
        case(Operation::substract): substract(); return;
        case(Operation::multiply): multiply(); return;
        case(Operation::divide): divide(); return;
        case(Operation::equal): equal(); return;
        case(Operation::power): power(); return;
        case(Operation::root): root(); return;
        }
    }
}

//
//BROADCAST OPERATIONS
//
namespace machine
{    
    template<typename T, typename R>
    void tile_broadcast(Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2, const Operation op)
    {   
        auto rows1{mxb1.get_rows()}, rows2{mxb2.get_rows()};
        auto cols1{mxb1.get_cols()}, cols2{mxb2.get_cols()};

        if((rows1 == 0) || (cols1 == 0)) {return;}
        if((rows2 == 0) || (cols2 == 0)) {return;}

        size_t max_row{std::max(rows1, rows2)};
        size_t max_col{std::max(cols1, cols2)};
        
        size_t mxb1_row{}, mxb2_row{};
        size_t mxb1_col{}, mxb2_col{};
//
//LAMBDAS
//
        auto add = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                mxb1_row = r % rows1;
                mxb2_row = r % rows2; 
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1_col = c % cols1;
                    mxb2_col = c % cols2;
                    mxb1(mxb1_row, mxb1_col) += mxb2(mxb2_row, mxb2_col);
                }
            }
        };
        auto substract = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                mxb1_row = r % rows1;
                mxb2_row = r % rows2; 
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1_col = c % cols1;
                    mxb2_col = c % cols2;
                    mxb1(mxb1_row, mxb1_col) -= mxb2(mxb2_row, mxb2_col);
                }
            }
        };
        auto multiply = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                mxb1_row = r % rows1;
                mxb2_row = r % rows2; 
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1_col = c % cols1;
                    mxb2_col = c % cols2;
                    mxb1(mxb1_row, mxb1_col) *= mxb2(mxb2_row, mxb2_col);
                }
            }
        };
        auto divide = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                mxb1_row = r % rows1;
                mxb2_row = r % rows2; 
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1_col = c % cols1;
                    mxb2_col = c % cols2;
                    mxb1(mxb1_row, mxb1_col) /= mxb2(mxb2_row, mxb2_col);
                }
            }
        };

        auto equal = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                mxb1_row = r % rows1;
                mxb2_row = r % rows2; 
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1_col = c % cols1;
                    mxb2_col = c % cols2;
                    mxb1(mxb1_row, mxb1_col) =  mxb2(mxb2_row, mxb2_col);
                }
            }
        };

        auto power = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                mxb1_row = r % rows1;
                mxb2_row = r % rows2; 
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1_col = c % cols1;
                    mxb2_col = c % cols2;
                    mxb1(mxb1_row, mxb1_col) =  std::pow(static_cast<double>(mxb1(mxb1_row, mxb1_col)), mxb2(mxb2_row, mxb2_col));
                }
            }
        };
        auto root = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                mxb1_row = r % rows1;
                mxb2_row = r % rows2; 
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1_col = c % cols1;
                    mxb2_col = c % cols2;
                    mxb1(mxb1_row, mxb1_col) =  std::pow(static_cast<double>(mxb1(mxb1_row, mxb1_col)), 1.0 / mxb2(mxb2_row, mxb2_col));
                }
            }
        };
        switch(op){
        case(Operation::multiply)   : multiply(); return;
        case(Operation::divide)     : divide(); return;
        case(Operation::add)        : add(); return;
        case(Operation::substract)  : substract(); return;
        case(Operation::equal)      : equal(); return;
        case(Operation::power)      : power(); return;
        case(Operation::root)       : root(); return;
        }
    }

    template<typename T, typename R>
    requires (std::integral<R> || std::floating_point<R>)
    void tile_broadcast(Matrix_base<T> &mxb1, const R &value, const Operation op)
    {   
        
        if((mxb1.get_rows() == 0) || (mxb1.get_cols() == 0)) {return;}

        if(op == Operation::divide || op == Operation::root){
            if(value == 0) {
                throw std::runtime_error("division/root by 0");
            }
        }
        auto max_row = mxb1.get_rows();
        auto max_col = mxb1.get_cols();

        auto add = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1(r, c) += value;
                }
            }
        };
        auto substract = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1(r, c) -= value;
                }
            }
        };
        auto multiply = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1(r, c) *= value;
                }
            }
        };
        auto divide = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1(r, c) /= value;
                }
            }
        };
        auto equal = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1(r, c) = value;
                }
            }
        };
        auto power = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1(r, c) =  std::pow(static_cast<double>(mxb1(r, c)), value);
                }
            }
        };
        auto root = [&]()->void {
            for(size_t r{0} ; r<max_row ; r++){  
                for(size_t c{0} ; c<max_col ; c++){
                    mxb1(r, c) =  std::pow(static_cast<double>(mxb1(r, c)), 1.0/value);
                }
            }
        };
        switch(op){
        case(Operation::multiply)   : multiply(); return;
        case(Operation::divide)     : divide(); return;
        case(Operation::add)        : add(); return;
        case(Operation::substract)  : substract(); return;
        case(Operation::equal)      : equal(); return;
        case(Operation::power)      : power(); return;
        case(Operation::root)       : root(); return;
        }
    }
}
