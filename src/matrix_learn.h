#pragma once

#include "matrix_statistic.h"

namespace matrix_learn
{
    Matrix softmax(const Matrix_base &mxb1);
    Matrix softmax(const Matrix_base &mxb1, const bool row_wise);
    Matrix relu_mask(const Matrix_base &mxb1);
    Matrix Lrelu_mask(const Matrix_base &mxb1);
    Matrix gelu_mask(const Matrix_base &mxb1);
}
