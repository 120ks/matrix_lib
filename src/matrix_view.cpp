#include "matrix_view.h"


Matrix_view::Matrix_view(double* start, Slice r_slice, Slice c_slice, const size_t total_rows, const size_t total_cols)
: view_ptr{start}
, start_row{r_slice.begin}
, start_col{c_slice.begin}
, end_row{r_slice.end}
, end_col{c_slice.end}
, base_rows{total_rows}
, base_cols{total_cols}

{
    end_row = (end_row == 0) ? base_rows : end_row; 
    end_col = (end_col == 0) ? base_cols : end_col; 
}

Matrix_view::Matrix_view(const Matrix_view &other)
: view_ptr{other.view_ptr}
, start_row{other.start_row}
, start_col{other.start_col}
, end_row{other.end_row}
, end_col{other.end_col}
, base_rows{other.base_rows}
, base_cols{other.base_cols}
{}

Matrix_view::Matrix_view(Matrix_view &&other)noexcept
: view_ptr{other.view_ptr}
, start_row{other.start_row}
, start_col{other.start_col}
, end_row{other.end_row}
, end_col{other.end_col}
, base_rows{other.base_rows}
, base_cols{other.base_cols}
{other.view_ptr = nullptr;}



double& Matrix_view::operator()(const size_t row, const size_t col)
{   
    return *(view_ptr+(row*base_cols) + col);
}

double Matrix_view::operator()(const size_t row, const size_t col)const
{   
    return *(view_ptr+(row*base_cols) + col);
}

Matrix_view Matrix_view::operator()(Slice r_slice, Slice c_slice)
{   
    double* new_start{view_ptr+(r_slice.begin * base_cols + c_slice.begin)};

    return {new_start, Slice{start_row+r_slice.begin, r_slice.end}, 
        Slice{start_col + c_slice.begin, c_slice.end}, base_rows, base_cols};
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



