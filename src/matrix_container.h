#pragma once

#include "matrix_base.h"

class Matrix_container : public Matrix_base
{
    protected:
        size_t rows{};
        size_t cols{};
        std::vector<double> matrix{};
        explicit Matrix_container(const size_t row_count = 0, const size_t col_count = 0, const double val = 0);
        Matrix_container(const Matrix_container &other);
        Matrix_container(Matrix_container &&other) noexcept;
        ~Matrix_container()override = default;

    public:
        double& operator()(const size_t row, const size_t col)override;
        double operator()(const size_t row, const size_t col)const override;
};
