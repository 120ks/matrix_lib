#pragma once
#include "friend_operators.hpp"


template<typename T>
class mask_flag
{   
    friend class Matrix<T>;
    public:
        Matrix_base<T>* ptr{};
        mask_flag(Matrix_base &mxb1) : ptr{&mxb1}{}
        mask_flag(const mask_flag&) = delete;
        mask_flag(mask_flag&&) = delete;
        operator=(mask_flag&&) = delete;
        operator(const mask_flag&) = delete;
    public:
        ~mask_flag() = default;
    //
    //comparison operators
    //  
        template<typename R>
        friend Matrix<mask_ref<T>> operator>(mask_flag<T>&flag, const R &val)
        {
            return {*ptr, boolean_logic::make_mask(*ptr, val)}
        }
        friend mask_flag<T> Matrix<T>::M(){
            return {*this};
        }
};
