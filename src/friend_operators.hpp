#pragma once
#include "matrix.hpp"
#include "slice_view.hpp"
#include "mask_view.hpp"

template<typename T> class Matrix;
//
//NON MEMBER ARITHMETIC OPERATORS
//
template<typename T, typename R>
auto operator+(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
{   
    using common_type = std::common_type_t<T, R>;
    Matrix<common_type>mx{mxb1};
    mx += mxb2;
    return mx;
}

template<typename T, typename R>
requires std::integral<R> || std::floating_point<R>
auto operator+(const Matrix_base<T> &mxb1, const R &val)
{   
    using common_type = std::common_type_t<T, R>;
    Matrix<common_type>mx{mxb1};
    mx += val;
    return mx;
}

template<typename T, typename R>
auto operator-(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
{   
    using common_type = std::common_type_t<T, R>;
    Matrix<common_type>mx{mxb1};
    mx -= mxb2;
    return mx;
}

template<typename T, typename R>
requires std::integral<R> || std::floating_point<R>
auto operator-(const Matrix_base<T> &mxb1, const R &val)
{   
    using common_type = std::common_type_t<T, R>;
    Matrix<common_type>mx{mxb1};
    mx -= val;
    return mx;
}

template<typename T, typename R>
auto operator*(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
{   
    using common_type = std::common_type_t<T, R>;
    Matrix<common_type>mx{mxb1};
    mx *= mxb2;
    return mx;
}

template<typename T, typename R>
requires std::integral<R> || std::floating_point<R>
auto operator*(const Matrix_base<T> &mxb1, const R &val)
{   
    using common_type = std::common_type_t<T, R>;
    Matrix<common_type>mx{mxb1};
    mx *= val;


    return mx;
}

template<typename T, typename R>
auto operator/(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
{   
    using common_type = std::common_type_t<T, R>;
    Matrix<common_type>mx{mxb1};
    mx /= mxb2;
    return mx;
}

template<typename T, typename R>
requires std::integral<R> || std::floating_point<R>
auto operator/(const Matrix_base<T> &mxb1, const R &val)
{   
    using common_type = std::common_type_t<T, R>;
    Matrix<common_type>mx{mxb1};
    mx /= val;
    return mx;
}


//
//NON MEMBER COMPARISON OPERATORS
//
template<typename T, typename R>
requires std::integral<R> || std::floating_point<R>
auto operator>(const Matrix_base<T> &mxb1, const R val)
{   
    return boolean_logic::make_mask(mxb1, Condition::over, val);
}

template<typename T, typename R>
requires std::integral<R> || std::floating_point<R>
Matrix<boolean> operator<(const Matrix_base<T> &mxb1, const R val)
{
    return boolean_logic::make_mask(mxb1, Condition::under, val);
}

template<typename T, typename R>
requires std::integral<R> || std::floating_point<R>
Matrix<boolean> operator>=(const Matrix_base<T> &mxb1, const R val)
{
    return boolean_logic::make_mask(mxb1, Condition::over_or, val);
}

template<typename T, typename R>
requires std::integral<R> || std::floating_point<R>
Matrix<boolean> operator<=(const Matrix_base<T> &mxb1, const R val)
{
    return boolean_logic::make_mask(mxb1, Condition::under_or, val);
}

template<typename T, typename R>
requires std::integral<R> || std::floating_point<R>
Matrix<boolean> operator==(const Matrix_base<T> &mxb1, const R val)
{
    return boolean_logic::make_mask(mxb1, Condition::is, val);
}

template<typename T, typename R>
requires std::integral<R> || std::floating_point<R>
Matrix<boolean> operator!=(const Matrix_base<T> &mxb1, const R val)
{
    return boolean_logic::make_mask(mxb1, Condition::is_not, val);
}

template<typename T, typename R>
auto operator>(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
{   
    return boolean_logic::make_mask(mxb1, Condition::over, mxb2);
}

template<typename T, typename R>
Matrix<boolean> operator<(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
{
    return boolean_logic::make_mask(mxb1, Condition::under, mxb2);
}

template<typename T, typename R>
Matrix<boolean> operator>=(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
{
    return boolean_logic::make_mask(mxb1, Condition::over_or, mxb2);
}

template<typename T, typename R>
Matrix<boolean> operator<=(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
{
    return boolean_logic::make_mask(mxb1, Condition::under_or, mxb2);
}

template<typename T, typename R>
Matrix<boolean> operator==(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
{
    return boolean_logic::make_mask(mxb1, Condition::is, mxb2);
}

template<typename T, typename R>
Matrix<boolean> operator!=(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
{
    return boolean_logic::make_mask(mxb1, Condition::is_not, mxb2);
}

Matrix<boolean> operator&&(const Matrix<boolean> &mask1, const Matrix<boolean> &mask2)
{
    size_t rows1{mask1.get_rows()};
    size_t cols1{mask1.get_cols()};
    size_t rows2{mask2.get_rows()};
    size_t cols2{mask2.get_cols()};

    Matrix<boolean> mask3{rows1, cols1, boolean{false}};
    
    assert(rows1 == rows2 && "row_mismatch");
    assert(cols1 == cols2 && "col_mismatch");

    for(size_t r{} ; r < rows1 ; r++){
        for(size_t c{} ; c<cols1 ; c++){
            mask3(r, c) = ((mask1(r, c) == 1) && (mask2(r, c) == 1)) ? 1 : 0;
        }
    }
    return mask3;
}

Matrix<boolean> operator||(const Matrix<boolean> &mask1, const Matrix<boolean> &mask2)
{
    size_t rows1{mask1.get_rows()};
    size_t cols1{mask1.get_cols()};
    size_t rows2{mask2.get_rows()};
    size_t cols2{mask2.get_cols()};

    Matrix<boolean> mask3{rows1, cols1, boolean{false}};
    
    assert(rows1 == rows2 && "row_mismatch");
    assert(cols1 == cols2 && "col_mismatch");

    for(size_t r{} ; r < rows1 ; r++){
        for(size_t c{} ; c<cols1 ; c++){
            mask3(r, c) = ((mask1(r, c) == 1) || (mask2(r, c) == 1)) ? 1 : 0;
        }
    }
    return mask3;
}




template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix_base<T> &mxb1){
    matrix_method::print(os, mxb1);
    return os;
}
