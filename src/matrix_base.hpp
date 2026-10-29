#pragma once
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include <random>
#include<memory>

#include "matrix_exceptions.hpp"



namespace machine
{   
    template<typename T>
    class Matrix;

    template<typename T>
    class Matrix_view;

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
            virtual inline Matrix<T> transpose()const = 0;
            virtual inline Matrix<T> copy()const = 0;
            virtual inline operator bool() const = 0;
            virtual inline size_t get_id()const = 0;
            virtual inline long long get_gen()const = 0;
    };
}
//
//enums and statics
//
namespace machine
{
    enum class Operation
    {
        multiply,
        divide,
        add,
        substract,
        modulo,
        equal,
        power,
        root
    };

    enum axis
    {
        X,
        Y
    };
    
    static std::random_device random_device;
    static std::mt19937 generator(random_device());
}
//
//FLAG SKELETONS
//
namespace machine
{
    template<typename T>
    class mask_flag
    {   
        friend class Matrix<T>;
        friend class Matrix_view<T>;
        public:
            Matrix_base<T>* ptr{};
            mask_flag(Matrix_base<T> &mxb1) : ptr{&mxb1}{}
            mask_flag(const mask_flag<T>&) = delete;
            mask_flag(mask_flag<T> &&other) : ptr{other.ptr} {other.ptr = nullptr;}
            operator=(mask_flag<T>&&) = delete;
            operator=(const mask_flag<T>&) = delete;
            ~mask_flag() = default;
        //
        //comparison operators
        //  
            friend mask_flag<T> Matrix<T>::M();
            friend mask_flag<T> Matrix_view<T>::M();
    };

    
    template<typename T>
    class assign_flag
    {
        friend class Matrix<T>;
        Matrix<T>* ptr{};
    public:
        
        assign_flag(Matrix<T> &mx) : ptr{&mx}{}
        assign_flag(const Matrix<T> &mx) = delete;
        assign_flag(Matrix<T> &&mx) = delete;
        operator= (const assign_flag&) = delete;
        operator= (assign_flag&&) = delete;
        
        template<typename R>
        Matrix<T>& operator=(const Matrix_base<R>&);
        Matrix<T>& operator=(Matrix<T>);
    };  

    template<typename T>
    class view_assign_flag
    {
        friend class Matrix_view<T>;
        Matrix_view<T>* ptr{};
    public:
        
        view_assign_flag(Matrix_view<T> &mxv) : ptr{&mxv}{}
        view_assign_flag(const Matrix_view<T> &mx) = delete;
        view_assign_flag(Matrix_view<T> &&mx) = delete;
        operator= (const view_assign_flag<T>&) = delete;
        operator= (view_assign_flag<T>&&) = delete;
        
        Matrix_view<T>& operator=(Matrix_view<T>);
        
    };  

}





namespace machine
{   
    class lifetime_table
    {   
        template<typename>
        friend class Matrix;

        template<typename>
        friend class assign_flag;

        inline static std::vector<long long> matrix_ids{}; //make this cycle
        inline static size_t count{};

        inline static void has_data(size_t id)
        {   
            if(matrix_ids.size()<1000){
                matrix_ids.resize(1000, -1);
            }
            if(id>=matrix_ids.size()){
                matrix_ids.resize(matrix_ids.size()*2, -1);
            }
            matrix_ids[id] = 1;
        }

        inline static void no_data(size_t id)
        {   
            matrix_ids[id] = -std::abs(matrix_ids[id]);
        }

        inline static size_t get_id()
        {
            return count++;
        }

        inline static void new_generation(size_t id){
            if(matrix_ids[id]<0){
                matrix_ids[id] = std::abs(matrix_ids[id]);
            }
            matrix_ids[id] = matrix_ids[id]+1;
        };

        public:
            inline static bool check_id(size_t id)
            {   
                return matrix_ids[id]>0;
            }

            inline static long long get_gen(size_t id)
            {
                return matrix_ids[id];
            }
    };
}
