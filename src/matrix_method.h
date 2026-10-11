#pragma once

#include "matrix.h"
#include "matrix_view.h"
#include "cmath"
#include "cassert"
#include <iostream>

class Matrix;

enum class Operation{
    multiply,
    divide,
    add,
    substract,
    equal,
    power,
    root
};

class Matrix_method
{
    private: 
        Matrix_method() = delete;
        Matrix_method(const Matrix_method&) = delete;
        Matrix_method(Matrix_method&&) = delete;

    public:
        static bool has_zero(const Matrix_base &mxb1);
        static void broadcast(Matrix_base &mxb1, const Matrix_base &mxb2, Operation op, const bool tile = false);
        static void broadcast(Matrix_base &mxb1, const double val, Operation op);
        static Matrix copy(const Matrix_base &mxb1);

        //helper functions
        static double sum(const Matrix_base &mxb1);
        static Matrix sum(const Matrix_base &mxb1, bool row_wise);

        static double avg(const Matrix_base &mxb1);
        static Matrix avg(const Matrix_base &mxb1, bool row_wise);
        
        static double std(const Matrix_base &mxb1, int degree = 0);
        static Matrix std(const Matrix_base &mxb1, bool row_wise, int degree = 0);

        static void normalize(Matrix_base &mxb1);
        static void initialize(Matrix_base &mxb1);

        static Matrix get_transposed(const Matrix_base &mxb1);
        static Matrix dot(Matrix_base &mxb1, Matrix_base &mxb2);

        static void print(const Matrix_base &mxb1);
        static void print(std::ostream &os, const Matrix_base &mxb1);
};
