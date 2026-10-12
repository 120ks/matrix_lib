#include "matrix_view.h"


Matrix_view::Matrix_view(Matrix &mx, const Slice r_slice, const Slice c_slice)
: view_ptr{&mx}
, row_slice{r_slice}
, col_slice{c_slice}
{
    row_slice.total = (row_slice.total==0)? mx.get_rows() : row_slice.total;
    col_slice.total = (col_slice.total==0)? mx.get_cols() : col_slice.total;
}

Matrix_view::Matrix_view(const Matrix_view &other)
: view_ptr{other.view_ptr}
, row_slice{other.row_slice}
, col_slice{other.col_slice}
{}

Matrix_view::Matrix_view(Matrix_view &&other)noexcept
: view_ptr{other.view_ptr}
, row_slice{other.row_slice}
, col_slice{other.col_slice}
{other.view_ptr = nullptr;}




double& Matrix_view::operator()(const size_t row, const size_t col)
{   
    assert(view_ptr);
    assert(row<row_slice.total&& col<col_slice.total);
    return (*view_ptr)(row+row_slice.begin, col+col_slice.begin);
}

double Matrix_view::operator()(const size_t row, const size_t col)const
{   
    assert(view_ptr);
    assert(row<row_slice.total&& col<col_slice.total);
    return (*view_ptr)(row+row_slice.begin, col+col_slice.begin);
}

Matrix_view Matrix_view::operator()(const Slice r_slice, const Slice c_slice)
{   
    return {*view_ptr, r_slice+row_slice, c_slice+col_slice};
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



