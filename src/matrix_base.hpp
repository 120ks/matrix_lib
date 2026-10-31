#pragma once
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include <random>
#include <memory>
#include <mutex>




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
        virtual inline std::mutex &get_mutex() = 0;

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
        
        view_assign_flag(Matrix_view<T> &mxv1) : ptr{&mxv1}{}
        view_assign_flag(const view_assign_flag<T>&) = delete;
        view_assign_flag(view_assign_flag<T>&&) = delete;
        operator= (const view_assign_flag<T>&) = delete;
        operator= (view_assign_flag<T>&&) = delete;
        
        Matrix_view<T>& operator=(Matrix_view<T>);
        Matrix_view<T>& operator=(Matrix<T> &matrix);
    };  
    
    template<typename T>
    class expression_lock_matrix
    {   
        Matrix<T>* ptr{};
        public:
            expression_lock_matrix(Matrix<T> &matrix);
            expression_lock_matrix(const expression_lock_matrix<T>&) = delete;
            expression_lock_matrix(expression_lock_matrix<T>&&) = delete;
            ~expression_lock_matrix();
            operator= (const expression_lock_matrix<T>&) = delete;
            operator= (expression_lock_matrix<T>&&) = delete;
            inline Matrix<T>& L() {return *ptr;}
        
    };

    template<typename T>
    class expression_lock_view
    {   
        Matrix_view<T>* ptr{};
        public:
            expression_lock_view(Matrix_view<T> &mxv);
            expression_lock_view(const expression_lock_view<T>&) = delete;
            expression_lock_view(expression_lock_view<T>&&) = delete;
            ~expression_lock_view();
            operator= (const expression_lock_view<T>&) = delete;
            operator= (expression_lock_view<T>&&) = delete;
            inline Matrix_view<T>& L() {return *ptr;}
            operator Matrix_view<T>&() {return *ptr;}
    };


}


