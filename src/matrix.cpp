#include "matrix.h"
#include "matrix_method.h"
#include "matrix_view.h"

class Matrix_view;
class Matrix_method;
class Matrix_mask;

Matrix::Matrix(const size_t row_count, const size_t col_count, const double val)
: Matrix_container(row_count, col_count, val)
{}

Matrix::Matrix(const Matrix &other)
: Matrix_container(other)
{}

Matrix::Matrix(Matrix &&other)noexcept
: Matrix_container(std::move(other))
{}

Matrix::Matrix(const Matrix_view &mxv)
: Matrix(mxv.get_rows(),  mxv.get_cols())
{
    for(size_t r{} ; r<rows; r++){
        for(size_t c{} ; c<cols ; c++){
            (*this)(r, c) = mxv(r, c);
        }
    }
}
Matrix::Matrix(const Matrix_base &mxb)
: Matrix{Matrix_method::copy(mxb)}
{}

double& Matrix::operator()(const size_t row, const size_t col){
    return matrix[row*cols + col];
}
double Matrix::operator()(const size_t row, const size_t col)const{
    return matrix[row*cols + col];
}
Matrix_view Matrix::operator()(Slice r_slice, Slice c_slice)
{   
    r_slice.end = (r_slice.end == 0) ? rows : r_slice.end;
    c_slice.end = (c_slice.end == 0) ? cols : c_slice.end;

    double* start_ptr{&((*this)(r_slice.begin, c_slice.begin))};
    return {start_ptr, r_slice, c_slice, rows, cols};}


Matrix& Matrix::operator=(Matrix other)
{   
    rows = other.rows;
    cols = other.cols;
    std::swap(this->matrix, other.matrix);
    return *this;
}

Matrix& Matrix::operator=(const Matrix_view &mxv)
{
    *this = Matrix{mxv};
    return *this;
}

Matrix& Matrix::operator=(const Matrix_base &mxb1)
{
    *this = Matrix_method::copy(mxb1);
    return *this;
}

//
//MEMBER ARITHMETIC OPERATORS
//
Matrix& Matrix::operator+=(const Matrix_base& other)
{
    Matrix_method::broadcast(*this, other, Operation::add);
    return *this;
}

Matrix& Matrix::operator+=(const double val)
{
    Matrix_method::broadcast(*this, val, Operation::add);
    return *this;
}

Matrix& Matrix::operator-=(const Matrix_base& other)
{
    Matrix_method::broadcast(*this, other, Operation::substract);
    return *this;
}

Matrix& Matrix::operator-=(const double val)
{
    Matrix_method::broadcast(*this, val, Operation::substract);
    return *this;
}

Matrix& Matrix::operator*=(const Matrix_base& other)
{
    Matrix_method::broadcast(*this, other, Operation::multiply);
    return *this;
}

Matrix& Matrix::operator*=(const double val)
{
    Matrix_method::broadcast(*this, val, Operation::multiply);
    return *this;
}

Matrix& Matrix::operator/=(const Matrix_base& other)
{
    Matrix_method::broadcast(*this, other, Operation::divide);
    return *this;
}


Matrix& Matrix::operator/=(const double val)
{
    Matrix_method::broadcast(*this, val, Operation::divide);
    return *this;
}

//
//FRIEND ARITHMETIC OPERATORS
//

Matrix operator+(const Matrix_base &mxb1, const Matrix_base &mxb2)
{
    Matrix mx{mxb1};
    Matrix_method::broadcast(mx, mxb2, Operation::add);
    return mx;
}

Matrix operator+(const Matrix_base &mxb1, const double val)
{
    Matrix mx{mxb1};
    Matrix_method::broadcast(mx, val, Operation::add);
    return mx;
}

Matrix operator-(const Matrix_base &mxb1, const Matrix_base &mxb2)
{
    Matrix mx{mxb1};
    Matrix_method::broadcast(mx, mxb2, Operation::substract);
    return mx;
}

Matrix operator-(const Matrix_base &mxb1, const double val)
{
    Matrix mx{mxb1};
    Matrix_method::broadcast(mx, val, Operation::substract);
    return mx;
}

Matrix operator*(const Matrix_base &mxb1, const Matrix_base &mxb2)
{
    Matrix mx{mxb1};
    Matrix_method::broadcast(mx, mxb2, Operation::multiply);
    return mx;
}

Matrix operator*(const Matrix_base &mxb1, const double val)
{
    Matrix mx{mxb1};
    Matrix_method::broadcast(mx, val, Operation::multiply);
    return mx;
}

Matrix operator/(const Matrix_base &mxb1, const Matrix_base &mxb2)
{
    Matrix mx{mxb1};
    Matrix_method::broadcast(mx, mxb2, Operation::divide);
    return mx;
}

Matrix operator/(const Matrix_base &mxb1, const double val)
{
    Matrix mx{mxb1};
    Matrix_method::broadcast(mx, val, Operation::divide);
    return mx;
}



