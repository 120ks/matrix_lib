#include "matrix_view.h"
#include "matrix_method.h"

Matrix_view::Matrix_view(Matrix &mx, size_t r_begin, size_t r_end, size_t c_begin, size_t c_end)
: view_ptr{&mx}
, row_begin{r_begin}
, col_begin{c_begin}
, rows{r_end-r_begin}
, cols{c_end-c_begin}
{
}

Matrix_view::Matrix_view(const Matrix_view &other)
: view_ptr{other.view_ptr}
, row_begin{other.row_begin}
, col_begin{other.col_begin}
, rows{other.rows}
, cols{other.cols}
{}

Matrix_view::Matrix_view(Matrix_view &&other)noexcept
: view_ptr{other.view_ptr}
, row_begin{other.row_begin}
, col_begin{other.col_begin}
, rows{other.rows}
, cols{other.cols}
{other.view_ptr = nullptr;}

double& Matrix_view::operator()(const size_t row, const size_t col)
{   
    assert(view_ptr);
    assert(row<rows&& col<cols);
    return (*view_ptr)(row+row_begin, col+col_begin);
}

double Matrix_view::operator()(const size_t row, const size_t col)const
{   
    assert(view_ptr);
    assert(row<rows&& col<cols);
    return (*view_ptr)(row+row_begin, col+col_begin);
}

Matrix_view Matrix_view::operator()(size_t r_begin, size_t r_end, size_t c_begin, size_t c_end)
{   
    assert(view_ptr);
    return{(*view_ptr), r_begin+row_begin, r_end+row_begin, c_begin+col_begin, c_end+col_begin};
}

Matrix_view& Matrix_view::operator=(const Matrix_base &mxb1)
{
    Matrix_method::broadcast(*this, mxb1, Operation::equal);
    return *this;
}

Matrix_view& Matrix_view::operator=(const double val)
{
    Matrix_method::broadcast(*this, val, Operation::equal);
    return *this;
}

Matrix_view& Matrix_view::operator=(const Matrix_view &mxv1)
{
    Matrix_method::broadcast(*this, mxv1, Operation::equal);
    return *this;
}

//
//MEMBER ARITHMETIC OPERATORS
//
Matrix_view& Matrix_view::operator+=(const Matrix_base& other)
{
    Matrix_method::broadcast(*this, other, Operation::add);
    return *this;
}

Matrix_view& Matrix_view::operator+=(const double val)
{
    Matrix_method::broadcast(*this, val, Operation::add);
    return *this;
}

Matrix_view& Matrix_view::operator-=(const Matrix_base& other)
{
    Matrix_method::broadcast(*this, other, Operation::substract);
    return *this;
}

Matrix_view& Matrix_view::operator-=(const double val)
{
    Matrix_method::broadcast(*this, val, Operation::substract);
    return *this;
}

Matrix_view& Matrix_view::operator*=(const Matrix_base& other)
{
    Matrix_method::broadcast(*this, other, Operation::multiply);
    return *this;
}

Matrix_view& Matrix_view::operator*=(const double val)
{
    Matrix_method::broadcast(*this, val, Operation::multiply);
    return *this;
}

Matrix_view& Matrix_view::operator/=(const Matrix_base& other)
{
    Matrix_method::broadcast(*this, other, Operation::divide);
    return *this;
}

Matrix_view& Matrix_view::operator/=(const double val)
{
    Matrix_method::broadcast(*this, val, Operation::divide);
    return *this;
}

