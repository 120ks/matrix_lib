#pragma once

#include "matrix_base.hpp"

template<typename T>
class Matrix_container : public Matrix_base<T>
{
    protected:
        size_t rows{};
        size_t cols{};
        std::vector<T> matrix{};
        T* first_val_address{};

        explicit Matrix_container(const size_t row_count, const size_t col_count, T val)
        : rows{row_count}
        , cols{col_count}
        , matrix(rows*cols, val)
        , first_val_address{&matrix[0]}
        {}

        Matrix_container(const Matrix_container &other)
        : rows{other.rows}
        , cols{other.cols}
        , matrix(other.rows * other.cols, 0)
        , first_val_address{&matrix[0]}
        {
            for(size_t r{} ; r<rows ; r++){
                for(size_t c{} ; c<cols ; c++){
                    (*this)(r, c) = other(r, c);
                }
            }
        }

        Matrix_container(Matrix_container &&other) noexcept
        : rows{other.rows}
        , cols{other.cols}
        , matrix{std::move(other.matrix)}
        , first_val_address{other.first_val_address}
        {other.first_val_address = nullptr;}

        ~Matrix_container()override = default;

    public:
        T& operator()(const size_t row, const size_t col)override{return matrix[row*cols + col];}
        T operator()(const size_t row, const size_t col)const override {return matrix[row*cols + col];}

};
