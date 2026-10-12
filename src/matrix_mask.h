
#pragma once
#include "matrix.h"
class Matrix_mask
{
    private:
        size_t rows{};
        size_t cols{};
        std::vector<char> mask{};
    public:
        Matrix_mask(const Matrix_base& mxb1, Condition cond, const double threshold);
        Matrix_mask(const Matrix_base& mxb1, Condition cond, const Matrix_base& mxb2);
        Matrix_mask(const Matrix_mask &other);
        Matrix_mask(Matrix_mask &&other);

        Matrix_mask& operator!();
        friend Matrix_mask operator&&(const Matrix_mask &mask1, const Matrix_mask mask2);
        friend Matrix_mask operator||(const Matrix_mask &mask1, const Matrix_mask mask2);

        inline char& operator()(const size_t row, const size_t col){
            return mask[row*cols + col];
        }
        inline char operator()(const size_t row, const size_t col)const{
            return mask[row*col + col];
        }
};
