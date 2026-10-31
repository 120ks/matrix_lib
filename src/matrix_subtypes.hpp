#pragma once

#include<cstddef>
#include<iostream>
#include<cassert>
#include<type_traits>
#include<set>

namespace machine
{
    enum class Condition
    {   
        is_true,
        is_false,
        under,
        over,
        over_or,
        under_or,
        is,
        is_not
    };
}
class Slice
{   
    public:
        size_t begin{};
        size_t end{};
        explicit Slice() : begin{0}, end{0}{}
        explicit Slice(const size_t bgn) : begin{bgn}, end{bgn+1}{}
        Slice(const size_t bgn, const size_t ending) : begin{bgn}, end{ending}{} //DO MOT ADD ANY VALID CHECKING 0 = ALL COLS / ROWS
        Slice(const Slice &other) : begin{other.begin} , end{other.end}{}
        ~Slice() = default;

        Slice& operator=(const Slice &other){
            begin = other.begin;
            end = other.end;
            return *this;
        }

        friend Slice operator+(const Slice old_slice, const Slice new_slice)
        {
            return {old_slice.begin + new_slice.begin, (new_slice.end == 0 )? old_slice.end : new_slice.end+old_slice.begin};
        }
};

class boolean //works like bool
{   
    private:
        bool value{};
    public:
        inline boolean(const bool val = false) : value{val}{}
        inline boolean(const boolean &other) : value{other.value}{}
        ~boolean() = default;
        operator bool() const {
            return value;
        }
        inline boolean& operator=(bool val){
            value = val;
            return *this;
        }
        inline boolean& operator=(const boolean &other){
            value = other;
            return *this;
        }
        inline bool operator!(){return !value;}
};

template<typename T>
class mask_ref
{   
    private:
        T* ptr{};
    public: 
        mask_ref(T& value) : ptr{&value}{}
        mask_ref() : ptr{nullptr}{}
        mask_ref(const mask_ref<T> &other) 
        : ptr{const_cast<T*> (other.ptr)}{}

        mask_ref(mask_ref<T> &&other) : ptr{other.ptr}{}
        ~mask_ref() = default;

        mask_ref<T>& operator=(mask_ref<T> other)
        {
            ptr = other.ptr ; other.ptr = nullptr ; return *this;
        }

        template<typename R>
        mask_ref<T>& operator=(R &&value) {*ptr = value; return *this;}
        operator T& ()const noexcept {return *ptr;}
};
//
//check if is type mask_ref
//
template <typename T>
struct is_mask_ref : std::false_type {};

template <typename U>
struct is_mask_ref<mask_ref<U>> : std::true_type {};

template <typename T>
inline constexpr bool is_mask_ref_v = is_mask_ref<T>::value;
//
//mask_ref common type 
//
namespace std{

    template<typename T, typename R>
    struct common_type<mask_ref<T>, R>{
        using type = std::common_type_t<T, R>;
    };
    template<typename T, typename R>
    struct common_type<T, mask_ref<R>>{
        using type = std::common_type_t<T, R>;
    };
    template<typename T, typename R>
    struct common_type<mask_ref<T>, mask_ref<R>>{
        using type = std::common_type_t<T, R>;
    };

    template<>
    struct common_type<boolean, bool>{
        using type = bool;
    };
    template<>
    struct common_type<bool, boolean>{
        using type = bool;
    };
}
