#pragma once

#include "matrix_base.hpp"
#include "matrix_statistics.hpp"
#include "matrix_linear.hpp"
#include "matrix_helper.hpp"


template<typename T>
class Matrix;

namespace machine
{   
    static const double E{2.718281828459};

    template<typename T>
    Matrix<T> softmax(const Matrix_base<T> &mxb1)
    {   
        
        Matrix<T> probabilities{machine::broadcast(mxb1, E, Operation::equal)};
        probabilities /= machine::sum(probabilities);
        
        return probabilities;
    }

    template<typename T>
    Matrix<T> softmax(const Matrix_base<T> &mxb1, const axis ax)
    {   
        Matrix<T> probabilities{machine::broadcast(mxb1, E, Operation::equal)};
        probabilities /= machine::sum(probabilities, ax);

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
    double cross_entropy_loss(const Matrix_base<T> &prediction, const Matrix_base<R> &hot_encoded)
    {
        if(!machine::same_dims(prediction, hot_encoded))
        {
            throw dimension_mismatch(std::format(
                "prediction({}x{}), must match hot_encoded({},{})", 
                prediction.get_rows(), prediction.get_cols(), hot_encoded.get_rows(), hot_encoded.get_cols()
                ));
        }
        if((prediction.get_rows() != 1) && (prediction.get_cols() != 1))
        {
            throw dimension_mismatch(std::format("one of prediction and hot encoded ({}x{}) must be 1", prediction.get_rows(), prediction.get_rows()));
        }
        return -std::log((prediction[hot_encoded>=0.98])(0,0));
    }

    
    

}


