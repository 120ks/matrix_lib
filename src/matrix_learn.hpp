#pragma once

#include "matrix_base.hpp"
#include "matrix_statistics.hpp"
#include "matrix_method.hpp"

template<typename T>
class Matrix;

namespace matrix_learn
{   
    static const double E{2.718281828459};

    template<typename T>
    Matrix<T> softmax(const Matrix_base<T> &mxb1)
    {   
        
        Matrix<T> probabilities{matrix_method::broadcast(mxb1, E, Operation::equal)};
        probabilities /= matrix_statistics::sum(probabilities);
        
        return probabilities;
    }

    template<typename T>
    Matrix<T> softmax(const Matrix_base<T> &mxb1, const bool row_wise)
    {   
        Matrix<T> probabilities{matrix_method::broadcast(mxb1, E, Operation::equal)};
        probabilities /= matrix_statistics::sum(probabilities, row_wise);

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
        return matrix_method::dot(mxb1, loss);
    }

}
