#pragma once

#include "matrix_exceptions.hpp"

//
//BROADCAST OPERATIONS
//
namespace machine
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

        auto rows1{mxb1.get_rows()}, rows2{mxb2.get_rows()};
        auto cols1{mxb1.get_cols()}, cols2{mxb2.get_cols()};

        if((rows1 == 0) || (cols1 == 0)) {return;}
        if((rows2 == 0) || (cols2 == 0)) {return;}
        
        if(op == Operation::divide || op == Operation::root){
            if(has_zero(mxb2)){throw std::runtime_error("division/root of mxb1 with mxb2-> contains 0");}
        }

        //check that dims are divisible
        if((cols1 % cols2 != 0) && (cols2%cols1 != 0)){
            throw dimension_mismatch(mxb1, mxb2, "cols must be divisible");
        }
        if((rows1 % rows2 != 0) && (rows2%rows1 != 0)){
            throw dimension_mismatch(mxb1, mxb2, "rows must be divisible");
        }
        size_t max_row{std::max(rows1, rows2)};
        size_t max_col{std::max(cols1, cols2)};
        
        size_t mxb1_row{}, mxb2_row{};
        size_t mxb1_col{}, mxb2_col{};

        if(same_dims(mxb1, mxb2)){
            for(size_t r{0} ; r<max_row ; r++){  
                for(size_t c{0} ; c<max_col ; c++){
                    switch(op){
                    case(Operation::multiply)   : mxb1(r, c) *= mxb2(r, c); break;
                    case(Operation::divide)     : mxb1(r, c) /= mxb2(r, c); break;
                    case(Operation::add)        : mxb1(r, c) += mxb2(r, c); break;
                    case(Operation::substract)  : mxb1(r, c) -= mxb2(r, c); break;
                    case(Operation::equal)      : mxb1(r, c) =  mxb2(r, c); break;


                    case(Operation::power)      : mxb1(r, c) =  std::pow(static_cast<double>(mxb1(r, c)), mxb2(r, c)); break;
                    case(Operation::root)       : mxb1(r, c) =  std::pow(static_cast<double>(mxb1(r, c)), 1.0 / mxb2(r, c)); break;
                    }                
                }
            }
            return;
        }


        for(size_t r{0} ; r<max_row ; r++){  
            mxb1_row = r % rows1;
            mxb2_row = r % rows2; 

            for(size_t c{0} ; c<max_col ; c++){
                mxb1_col = c % cols1;
                mxb2_col = c % cols2;
                switch(op){
                case(Operation::multiply)   : mxb1(mxb1_row, mxb1_col) *= mxb2(mxb2_row, mxb2_col); break;
                case(Operation::divide)     : mxb1(mxb1_row, mxb1_col) /= mxb2(mxb2_row, mxb2_col); break;
                case(Operation::add)        : mxb1(mxb1_row, mxb1_col) += mxb2(mxb2_row, mxb2_col); break;
                case(Operation::substract)  : mxb1(mxb1_row, mxb1_col) -= mxb2(mxb2_row, mxb2_col); break;
                case(Operation::equal)      : mxb1(mxb1_row, mxb1_col) =  mxb2(mxb2_row, mxb2_col); break;


                case(Operation::power)      : mxb1(mxb1_row, mxb1_col) =  std::pow(static_cast<double>(mxb1(mxb1_row, mxb1_col)), mxb2(mxb2_row, mxb2_col)); break;
                case(Operation::root)       : mxb1(mxb1_row, mxb1_col) =  std::pow(static_cast<double>(mxb1(mxb1_row, mxb1_col)), 1.0 / mxb2(mxb2_row, mxb2_col)); break;
                }                
            }
        }
    }
    template<typename T, typename R>
    requires (std::integral<R> || std::floating_point<R>)
    void broadcast(Matrix_base<T> &mxb1, const R &value, const Operation op)
    {   
        
        if((mxb1.get_rows() == 0) || (mxb1.get_cols() == 0)) {return;}

        if(op == Operation::divide || op == Operation::root){
            if(value == 0) {
                throw std::runtime_error("division/root by 0");
            }
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


                case(Operation::power)      : mxb1(r, c) =  std::pow(static_cast<double>(mxb1(r, c)), value); break;
                case(Operation::root)       : mxb1(r, c) =  std::pow(static_cast<double>(mxb1(r, c)), 1.0 / value); break;
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

namespace machine
{   
    class lifetime_table
    {   
        template<typename>
        friend class Matrix;

        template<typename>
        friend class assign_flag;

        inline static std::vector<long long> matrix_ids{0};
        inline static std::mutex mxt;

        inline static void no_data(size_t id)
        {   
            if(id>= matrix_ids.size())
                {throw indexing_mismatch("id index too large in no_data()");
            }
            std::lock_guard<std::mutex> lock(mxt);
            matrix_ids[id] = -std::abs(matrix_ids[id]);
        }

        inline static void remove_id(size_t id)
        {   
            if(id>= matrix_ids.size())
                {throw indexing_mismatch("id index too large in remove_id()");
            }
            std::lock_guard<std::mutex> lock(mxt);
            matrix_ids[id] = 0;
        }

        inline static size_t get_id(const long long gen = 1)
        {   
            std::lock_guard<std::mutex> lock(mxt);
            for(size_t i{} ; i<matrix_ids.size() ; i++)
            {
                if(matrix_ids[i] == 0){
                    matrix_ids[i] = gen;
                    return i;
                }
            }

            auto i_size = matrix_ids.size();
            matrix_ids.resize(i_size*2, 0);
            matrix_ids[i_size] = gen;
            return i_size;
        }

        inline static void new_generation(size_t id){
            std::lock_guard<std::mutex> lock(mxt);

            if(id>= matrix_ids.size())
                {throw indexing_mismatch("id index too large in new_generation()");
            }
            if(matrix_ids[id]<0){
                matrix_ids[id] = std::abs(matrix_ids[id]);
            }
            matrix_ids[id] = matrix_ids[id]+1;
        };

        public:
            inline static bool check_id(size_t id)
            {   
                std::lock_guard<std::mutex> lock(mxt);
                if(id >= matrix_ids.size()){throw indexing_mismatch("id index too large in check_id()");}
                return matrix_ids[id]>0;
            }

            inline static long long get_gen(size_t id)
            {   
                std::lock_guard<std::mutex> lock(mxt);
                if(id >= matrix_ids.size()){throw indexing_mismatch("id index too large get_gen()");}
                return matrix_ids[id];
            }
    };
}
