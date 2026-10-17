#pragma once

#include "matrix_base.hpp"
#include "matrix_statistics.hpp"
#include "matrix_method.hpp"

template<typename T>
class Matrix;

namespace matrix_learn
{
    template<typename T>
    Matrix<T> softmax(const Matrix_base<T> &mxb1)
    {   
        double e{2.718281828459};
        Matrix<T> probabilities{matrix_method::broadcast(mxb1, e, Operation::equal)};
        probabilities /= matrix_statistics::sum(probabilities);
        
        return probabilities;
    }

    template<typename T>
    Matrix<T> softmax(const Matrix_base<T> &mxb1, const bool row_wise)
    {   
        double e{2.718281828459};
        Matrix<T> probabilities{matrix_method::broadcast(mxb1, e, Operation::equal)};
        probabilities /= matrix_statistics::sum(probabilities, row_wise);

        return probabilities;
    }

    template<typename T>
    Matrix<T> ReLU_mask(const Matrix_base<T> &mxb1);

    template<typename T>
    Matrix<T> lReLU_mask(const Matrix_base<T> &mxb1);

    template<typename T>
    Matrix<T> GeLU_mask(const Matrix_base<T> &mxb1);
}
