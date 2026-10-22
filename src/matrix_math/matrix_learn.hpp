#pragma once

#include "matrix_base.hpp"
#include "matrix_statistics.hpp"
#include "matrix_lin.hpp"

template<typename T>
class Matrix;

namespace matrix_math
{   
    static const double E{2.718281828459};

    template<typename T>
    Matrix<T> softmax(const Matrix_base<T> &mxb1)
    {   
        
        Matrix<T> probabilities{method::broadcast(mxb1, E, Operation::equal)};
        probabilities /= matrix_math::sum(probabilities);
        
        return probabilities;
    }

    template<typename T>
    Matrix<T> softmax(const Matrix_base<T> &mxb1, const bool row_wise)
    {   
        Matrix<T> probabilities{method::broadcast(mxb1, E, Operation::equal)};
        probabilities /= matrix_math::sum(probabilities, row_wise);

        return probabilities;
    }

    template<typename T, typename R>
    void _ReLU(Matrix_base<T> &mxb1, const R &threshold, const double multiplier = 0)
    {
        auto mask{mxb1<threshold};
        mxb1[mask] *= multiplier;
    }

    template<typename T>
    double sigmoid(double val)
    {
        return 1 / (1 + std::pow(E, -val));
    }

    template<typename T, typename R>
    auto gradients(const Matrix_base<T> &mxb1, const Matrix_base<R> &loss)
    {
        return matrix_math::dot(mxb1, loss);
    }
}


