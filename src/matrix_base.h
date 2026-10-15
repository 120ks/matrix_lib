#pragma once
#include <vector>
#include <iostream>
#include <cassert>

class Matrix_view;
class Matrix;




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
        Slice() : begin{0}, end{0}{}
        Slice(const size_t bgn) : begin{bgn}, end{bgn+1}{}
        Slice(const size_t bgn, const size_t ending) : begin{bgn}, end{ending}{}
        Slice(const Slice &other) : begin{other.begin} , end{other.end}{}
        ~Slice() = default;

        friend Slice operator+(const Slice sl1, const Slice sl2)
        {
            return {sl1.begin + sl2.begin, (sl2.end == 0 )? sl1.end : sl2.end+sl1.begin};
        }
};

class BOOL //jotta voidaan kirjoittaa perinteistä c++ koodia eikä bitpäkättyä bool vektoria.
{   
    friend class Mask;
    private:
        bool value{};

    public:
        inline BOOL(const bool val) : value{val}{}
        inline BOOL(const BOOL &other) : value{other.value}{}
        ~BOOL() = default;

        operator bool() const { return value;}
        bool operator!(){return !value;}
        inline BOOL& operator=(bool val){value = val; return *this;}

};


class Matrix_base
{
    protected:
        Matrix_base() = default;
        virtual ~Matrix_base() = default;

    public:
        virtual double& operator()(const size_t, const size_t) = 0;
        virtual double operator()(const size_t, const size_t) const = 0;

        virtual inline size_t get_rows()const = 0;
        virtual inline size_t get_cols()const = 0;

        //these are needed for masking
        virtual inline size_t matrix_rows()const = 0;
        virtual inline size_t matrix_cols()const = 0;
        virtual inline size_t begin_row()const = 0;
        virtual inline size_t begin_col()const = 0;
        virtual inline double* matrix_start_ptr()const = 0;
        virtual inline double* start_ptr()const = 0;
        
};
