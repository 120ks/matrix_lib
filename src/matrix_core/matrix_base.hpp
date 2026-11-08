#pragma once

#include <random>
#include <mutex>
#include <cassert>
#include "matrix_subtypes.hpp"


namespace machine
{   
    template<typename T>
    class Matrix;

    template<typename T>
    class Matrix_view;

    template<typename T>
    class Mask_view;

    template<typename Tmx, typename T>
    class Matrix_base
    {   
    protected:
        Matrix_base() = default;
        virtual ~Matrix_base() = default;


    public:
        inline T& operator()(long long row, long long col){
            auto& mx = static_cast<Tmx&>(*this);
            return mx(row, col);
        }
        inline const T& operator()(long long row, long long col)const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx(row, col);
        }
        inline long long get_rows(){
            auto& mx = static_cast<Tmx&>(*this);
            return mx.get_rows();
        }
        inline long long get_cols(){
            auto& mx = static_cast<Tmx&>(*this);
            return mx.get_cols();
        }
        inline long long get_rows()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx.get_rows();
        }
        inline long long get_cols()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx.get_cols();
        }
        inline operator bool()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx;
        }
        inline long long get_id()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx.get_id();
        };
        inline long long get_gen()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx.get_gen();
        }
        T* begin(){
            auto& mx = static_cast<Tmx&>(*this);
            return mx.begin();
        }
        T* end(){
            auto& mx = static_cast<Tmx&>(*this);
            return mx.end();
        }
        const T * begin()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx.begin();
        }
        const T* end()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx.end();
        }
        Matrix<T> copy()const{
            return {*this};
        }
        Matrix<T> transpose()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx.transpose();
        }


        template<typename Rmx, typename R, typename U>
        Tmx& operator+=(Matrix_base<Rmx, R){

        }


















    };
}


//
//FLAG SKELETONS
//
namespace machine
{
    template<typename Tmx, typename T>
    class mask_flag
    {   
        friend class Matrix<T>;
        friend class Matrix_view<T>;
    public:
        Matrix_base<Tmx, T>* ptr{};
        mask_flag(Matrix_base<Tmx, T> &mxb1) : ptr{&mxb1}{}
        mask_flag(const mask_flag<Tmx, T>&) = delete;
        mask_flag(mask_flag<Tmx, T> &&other) : ptr{other.ptr} {other.ptr = nullptr;}
        mask_flag<Tmx, T>& operator=(mask_flag<Tmx, T>&&) = delete;
        mask_flag<Tmx, T>& operator=(const mask_flag<Tmx, T>&) = delete;
        ~mask_flag() = default;
        
        friend mask_flag<Matrix<T>, T> Matrix<T>::M();
        friend mask_flag<Matrix_view<T>, T> Matrix_view<T>::M();
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
        assign_flag& operator= (const assign_flag&) = delete;
        assign_flag& operator= (assign_flag&&) = delete;
        
        template<typename Rmx, typename R>
        Matrix<T>& operator=(const Matrix_base<Rmx, R>&);
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
        view_assign_flag<T>& operator= (const view_assign_flag<T>&) = delete;
        view_assign_flag<T>& operator= (view_assign_flag<T>&&) = delete;
        
        Matrix_view<T>& operator=(Matrix_view<T>);
        Matrix_view<T>& operator=(Matrix<T> &matrix);
    };  
}
