#pragma once

#include<cstddef>
#include<iostream>

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

class Slice
{   
    public:
        size_t begin{};
        size_t end{};
        explicit Slice() : begin{0}, end{0}{}
        explicit Slice(const size_t bgn) : begin{bgn}, end{bgn+1}{}
        Slice(const size_t bgn, const size_t ending) : begin{bgn}, end{ending}{}
        Slice(const Slice &other) : begin{other.begin} , end{other.end}{}
        ~Slice() = default;

        friend Slice operator+(const Slice old_slice, const Slice new_slice)
        {
            return {old_slice.begin + new_slice.begin, (new_slice.end == 0 )? old_slice.end : new_slice.end+old_slice.begin};
        }
};

Slice operator"" _sl(const size_t val){
    return Slice{val};
}

class boolean //to create a c++ syntax friendly bool
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
};


