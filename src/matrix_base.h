#pragma once
#include <vector>
#include <iostream>

class Matrix_view;



class Matrix_base
{
    protected:
        Matrix_base() = default;
        virtual ~Matrix_base() = default;

    public:
        virtual double& operator()(const size_t, const size_t) = 0;
        virtual double operator()(const size_t, const size_t) const = 0;
        virtual Matrix_view operator()(size_t r_begin, size_t r_end, size_t c_begin, size_t c_end) = 0;

        virtual inline size_t get_rows()const = 0;
        virtual inline size_t get_cols()const = 0;

};
