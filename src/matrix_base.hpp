#pragma once
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>

template<typename T>
class Matrix_base
{
    protected:
        Matrix_base() = default;
        virtual ~Matrix_base() = default;

    public:
        virtual T& operator()(const size_t, const size_t) = 0;
        virtual T operator()(const size_t, const size_t) const = 0;

        virtual inline size_t get_rows()const = 0;
        virtual inline size_t get_cols()const = 0;
        virtual inline size_t matrix_rows()const = 0;
        virtual inline size_t matrix_cols()const = 0;


        virtual inline T* start_ptr()const = 0;
        
};

