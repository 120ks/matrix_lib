#pragma once


#include "matrix_exceptions.hpp"
#include <cmath>
#include "micros_pd.hpp"
#include "micros_ps.hpp"
#include <type_traits>

namespace machx
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
    template<typename T>
    requires (std::is_same_v<T, float>)
    void broadcast(Matrix<T>& mx1, const Matrix<T>& mx2, const Operation op, int)
    {   
        if(same_dims(mx1, mx2)){
            broadcast_ps_direct(mx1.begin(), mx2.begin(), mx1.get_rows(), mx2.get_cols(), op);
            return;
        }

        if(divisible(mx1, mx2))
        {
            tile_broadcast(mx1, mx2, op);
            return;
        }
    }
    template<typename T>
    requires (std::is_same_v<T, double>)
    void broadcast(Matrix<T>& mx1, const Matrix<T>& mx2, const Operation op)
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

    template<typename Tmx, typename T, typename Rmx, typename R>
    void broadcast(Matrix_base<Tmx, T>& mxb1, const Matrix_base<Rmx, R>& mxb2, const Operation op)
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
    template<typename Tmx, typename T, typename R>
    requires (std::integral<R> || std::floating_point<R>)
    void broadcast(Matrix_base<Tmx, T> &mxb1, const R &value, const Operation op)
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
    template<typename Tmx, typename T, typename Rmx, typename R>
    Matrix<T> broadcast(Matrix_base<Tmx, T> &&mxb1, const Matrix_base<Rmx, R> &mxb2, const Operation op)
    {   
        broadcast(mxb1, mxb2, op);
        return {std::move(mxb1)};
    }

    template<typename Tmx, typename T, typename R>
    requires (std::integral<R> || std::floating_point<R>)
    Matrix<T> broadcast(Matrix_base<Tmx, T> &&mxb1, const R &value, const Operation op)
    {   
        broadcast(mxb1, value, op);
        return {std::move(mxb1)};
    }
}

//SIMD BROADCASTING

namespace machx{
    inline void broadcast_ps_direct(float* __restrict mx1, const float* __restrict mx2, int rows, int cols, const Operation op){
        //assumes dimensions same for both
        constexpr int COL_M = 256; //div by 8
        constexpr int ROW_M = 64; //div by 8 (4 if changing equal to use 4x8)
        const int row_step = ((op == Operation::equal) ? 8: 4);

        auto mul = [&](int &row, int row_end, int col_batch, int local_col)mutable{
            for(; row <= row_end-row_step ; row += row_step){
                machx::micro::prec1::broadcast_ps_4x8_mul(&mx1[row*cols + col_batch], &mx2[row*cols + col_batch], local_col, cols);
            }
        };
        auto add = [&](int &row, int row_end, int col_batch, int local_col)mutable{
            for(; row <= row_end-row_step ; row += row_step){
                machx::micro::prec1::broadcast_ps_4x8_add(&mx1[row*cols + col_batch], &mx2[row*cols + col_batch], local_col, cols);
            }
        };
        auto sub = [&](int &row, int row_end, int col_batch, int local_col)mutable{
            for(; row <= row_end-row_step ; row += row_step){
                machx::micro::prec1::broadcast_ps_4x8_sub(&mx1[row*cols + col_batch], &mx2[row*cols + col_batch], local_col, cols);
            }
        };
        auto div = [&](int &row, int row_end, int col_batch, int local_col)mutable{
            for(; row <= row_end-row_step ; row += row_step){
                machx::micro::prec1::broadcast_ps_4x8_div(&mx1[row*cols + col_batch], &mx2[row*cols + col_batch], local_col, cols);
            }
        };
        auto equal = [&](int &row, int row_end, int col_batch, int local_col)mutable{
            for(; row <= row_end-row_step ; row += row_step){
                machx::micro::prec1::broadcast_ps_8x8_equal(&mx1[row*cols + col_batch], &mx2[row*cols + col_batch], local_col, cols);
            }
        };

        #pragma omp parallel for collapse(2) schedule(dynamic, 8)
        for(int row_batch = 0; row_batch<rows; row_batch+=ROW_M){
            for(int col_batch = 0; col_batch<cols; col_batch += COL_M){
                int row_end = std::min(row_batch + ROW_M, rows);
                int local_col = std::min(COL_M, cols-col_batch);

                int row = row_batch;
                switch (op)
                {
                    case Operation::multiply: mul(row, row_end, col_batch, local_col); break;
                    case Operation::add: add(row, row_end, col_batch, local_col); break;
                    case Operation::substract: sub(row, row_end, col_batch, local_col); break;
                    case Operation::divide: div(row, row_end, col_batch, local_col); break;
                    case Operation::equal: equal(row, row_end, col_batch, local_col); break;
                }
                
                for(; row<row_end; row++){
                    for(int col = col_batch; col<col_batch+local_col; col++){
                        float& m1 = mx1[row*cols + col];
                        const float m2 = mx2[row*cols + col];
                        switch (op) {
                        case Operation::multiply:  m1 *= m2; break;
                        case Operation::add:       m1 += m2; break;
                        case Operation::substract: m1 -= m2; break;
                        case Operation::divide:    m1 /= m2; break;
                        case Operation::equal:     m1 = m2;  break;
                        }
                    }   
                }
            }
        }   
    }
}



//
//BROADCAST machx END
//
namespace machx
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
        switch(op){
        case(Operation::add): add(); return;
        case(Operation::substract): substract(); return;
        case(Operation::multiply): multiply(); return;
        case(Operation::divide): divide(); return;
        case(Operation::equal): equal(); return;
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
        switch(op){
        case(Operation::add): add(); return;
        case(Operation::substract): substract(); return;
        case(Operation::multiply): multiply(); return;
        case(Operation::divide): divide(); return;
        case(Operation::equal): equal(); return;
        }
    }
    
}

//
//BROADCAST OPERATIONS
//
namespace machx
{    
    template<typename Tmx, typename T, typename Rmx, typename R>
    void tile_broadcast(Matrix_base<Tmx, T> &mxb1, const Matrix_base<Rmx, R> &mxb2, const Operation op)
    {   

        auto rows1{mxb1.get_rows()}, rows2{mxb2.get_rows()};
        auto cols1{mxb1.get_cols()}, cols2{mxb2.get_cols()};

        if((rows1 == 0) || (cols1 == 0)) {return;}
        if((rows2 == 0) || (cols2 == 0)) {return;}

        long long max_row{std::max(rows1, rows2)};
        long long max_col{std::max(cols1, cols2)};
        
        long long mx1_row{}, mx2_row{};
        long long mx1_col{}, mx2_col{};
//
//LAMBDAS
//
        auto add = [&]()->void {
            for(long long r{0} ; r<max_row ; r++){  
                mx1_row = r % rows1;
                mx2_row = r % rows2; 
                for(long long c{0} ; c<max_col ; c++){
                    mx1_col = c % cols1;
                    mx2_col = c % cols2;
                    mxb1(mx1_row, mx1_col) += mxb2(mx2_row, mx2_col);
                }
            }
        };
        auto substract = [&]()->void {
            for(long long r{0} ; r<max_row ; r++){  
                mx1_row = r % rows1;
                mx2_row = r % rows2; 
                for(long long c{0} ; c<max_col ; c++){
                    mx1_col = c % cols1;
                    mx2_col = c % cols2;
                    mxb1(mx1_row, mx1_col) -= mxb2(mx2_row, mx2_col);
                }
            }
        };
        auto multiply = [&]()->void {
            for(long long r{0} ; r<max_row ; r++){  
                mx1_row = r % rows1;
                mx2_row = r % rows2; 
                for(long long c{0} ; c<max_col ; c++){
                    mx1_col = c % cols1;
                    mx2_col = c % cols2;
                    mxb1(mx1_row, mx1_col) *= mxb2(mx2_row, mx2_col);
                }
            }
        };
        auto divide = [&]()->void {
            for(long long r{0} ; r<max_row ; r++){  
                mx1_row = r % rows1;
                mx2_row = r % rows2; 
                for(long long c{0} ; c<max_col ; c++){
                    mx1_col = c % cols1;
                    mx2_col = c % cols2;
                    mxb1(mx1_row, mx1_col) /= mxb2(mx2_row, mx2_col);
                }
            }
        };

        auto equal = [&]()->void {
            for(long long r{0} ; r<max_row ; r++){  
                mx1_row = r % rows1;
                mx2_row = r % rows2; 
                for(long long c{0} ; c<max_col ; c++){
                    mx1_col = c % cols1;
                    mx2_col = c % cols2;
                    mxb1(mx1_row, mx1_col) = mxb2(mx2_row, mx2_col);
                }
            }
        };
        switch(op){
        case(Operation::multiply)   : multiply(); return;
        case(Operation::divide)     : divide(); return;
        case(Operation::add)        : add(); return;
        case(Operation::substract)  : substract(); return;
        case(Operation::equal)      : equal(); return;
        }
        
    }

    template<typename Tmx, typename T, typename R>
    requires (std::integral<R> || std::floating_point<R>)
    void tile_broadcast(Matrix_base<Tmx, T> &mxb1, const R &value, const Operation op)
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
            for(long long r{0} ; r<max_row ; r++){  
                for(long long c{0} ; c<max_col ; c++){
                    mxb1(r, c) += value;
                }
            }
        };
        auto substract = [&]()->void {
            for(long long r{0} ; r<max_row ; r++){  
                for(long long c{0} ; c<max_col ; c++){
                    mxb1(r, c) -= value;
                }
            }
        };
        auto multiply = [&]()->void {
            for(long long r{0} ; r<max_row ; r++){  
                for(long long c{0} ; c<max_col ; c++){
                    mxb1(r, c) *= value;
                }
            }
        };
        auto divide = [&]()->void {
            for(long long r{0} ; r<max_row ; r++){  
                for(long long c{0} ; c<max_col ; c++){
                    mxb1(r, c) /= value;
                }
            }
        };
        auto equal = [&]()->void {
            for(long long r{0} ; r<max_row ; r++){  
                for(long long c{0} ; c<max_col ; c++){
                    mxb1(r, c) = value;
                }
            }
        };
        switch(op){
        case(Operation::multiply)   : multiply(); return;
        case(Operation::divide)     : divide(); return;
        case(Operation::add)        : add(); return;
        case(Operation::substract)  : substract(); return;
        case(Operation::equal)      : equal(); return;
        }
    }
}
