#include "matrix_container.h"


Matrix_container::Matrix_container(const size_t row_count, const size_t col_count, const double val)
: rows{row_count}
, cols{col_count}
, matrix(rows*cols, val)
{}

Matrix_container::Matrix_container(const Matrix_container &other)
: rows{other.rows}
, cols{other.cols}
, matrix(other.rows * other.cols, 0)
{
    for(size_t r{} ; r<rows ; r++){
        for(size_t c{} ; c<cols ; c++){
            (*this)(r, c) = other(r, c);
        }
    }
}

Matrix_container::Matrix_container(Matrix_container &&other)noexcept
: rows{other.rows}
, cols{other.cols}
, matrix{std::move(other.matrix)}
{}


double& Matrix_container::operator()(const size_t row, const size_t col){
    return matrix[row*cols + col];
}
double Matrix_container::operator()(const size_t row, const size_t col)const{
    return matrix[row*cols + col];
}
