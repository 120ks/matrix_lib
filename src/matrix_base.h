#pragma once
#include <vector>
#include <iostream>
#include <cassert>

class Matrix_view;
class Matrix_mask;
class Matrix;

enum class Condition
{   
    na,
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
        size_t total{};
        Slice() : begin{0}, total{0}{}
        Slice(const size_t bgn) : begin{bgn}, total{1}{}
        Slice(const size_t bgn, const size_t end) : begin{bgn}, total{end-bgn}{}
        Slice(const Slice &other) : begin{other.begin} , total{other.total}{}
        ~Slice() = default;


    friend Slice operator+(const Slice sl1, Slice sl2)
    {   
        
        sl2.total == 0 ? sl1.total : sl2.total;
        
        //asettaa aloituksen siihen kohtaa josta alkaa ja säilyttää sl2 total, joka tulisi olla sama
        //ottaen huomioon
        return Slice{sl1.begin + sl2.begin, sl2.total + (sl1.begin + sl2.begin)};
    }
};

class Matrix_base
{
    protected:
        Matrix_base() = default;
        virtual ~Matrix_base() = default;

    public:
        virtual double& operator()(const size_t, const size_t) = 0;
        virtual double operator()(const size_t, const size_t) const = 0;

        virtual Matrix_view operator()(const Slice, const Slice) = 0;

        virtual inline size_t get_rows()const = 0;
        virtual inline size_t get_cols()const = 0;
        
        virtual inline size_t begin_row()const = 0;
        virtual inline size_t begin_col()const = 0;
        
};
