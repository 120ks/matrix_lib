#pragma once

#include "matrix_base.hpp"
#include "matrix_statistics.hpp"
#include "matrix_lin.hpp"
#include "matrix_helper.hpp"

template<typename T>
class Matrix;

namespace mx_math
{   
    static const double E{2.718281828459};

    template<typename T>
    Matrix<T> softmax(const Matrix_base<T> &mxb1)
    {   
        
        Matrix<T> probabilities{method::broadcast(mxb1, E, Operation::equal)};
        probabilities /= mx_math::sum(probabilities);
        
        return probabilities;
    }

    template<typename T>
    Matrix<T> softmax(const Matrix_base<T> &mxb1, const bool row_wise)
    {   
        Matrix<T> probabilities{method::broadcast(mxb1, E, Operation::equal)};
        probabilities /= mx_math::sum(probabilities, row_wise);

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
        return mx_math::dot(mxb1, loss);
    }

    template<typename T, typename R>
    double cross_entropy_loss(const Matrix_base<T> &prediction, const Matrix_base<R> &hot_encoded)
    {
        if(!(prediction.get_rows()==1 || prediction.get_cols()==1))
        {
            std::cerr<<"either cols or rows must be 1";
        }
        if(!(hot_encoded.get_rows()==1 || hot_encoded.get_cols()==1))
        {
            std::cerr<<"either cols or rows must be 1";
        }
        if(!matrix_math::same_dims(prediction, hot_encoded))
        {
            std::cerr<<"prediction dimensions do not match with hot encoded";
        }
        return -std::log((prediction[hot_encoded>=0.99])(0,0));
    }
}


