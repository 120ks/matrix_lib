#pragma once
#include "matrix_base.hpp"

namespace matrix_math
{
    template<typename T, typename R>
    bool same_dims(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
    {
        return (mxb1.get_rows() == mxb2.get_rows()) && (mxb1.get_cols() == mxb2.get_cols());
    }

    template<typename T, typename R>
    bool rows_and_cols_are(const Matrix<T> &mxb1, const size_t rows, const size_t cols)
    {
        return ((mxb1.get_rows() == rows) && mxb1.get_cols() == cols);
    }
}
