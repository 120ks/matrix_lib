#pragma once
#include "matrix.hpp"
#include "matrix_view.hpp"
#include <iostream>
#include <iomanip>
#include <type_traits>



namespace machine
{
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
    Matrix<boolean> operator>(const Matrix_base<T> &mxb1, const R val)
    {   
        return  machine::make_mask(mxb1, Condition::over, val);
    }

    template<typename T, typename R>
    Matrix<boolean> operator<(const Matrix_base<T> &mxb1, const R val)
    {  
        return machine::make_mask(mxb1, Condition::under, val);
    }

    template<typename T, typename R>
    Matrix<boolean> operator>=(const Matrix_base<T> &mxb1, const R val)
    {
        return machine::make_mask(mxb1, Condition::over_or, val);
    }

    template<typename T, typename R>
    Matrix<boolean> operator<=(const Matrix_base<T> &mxb1, const R val)
    {
        return machine::make_mask(mxb1, Condition::under_or, val);
    }

    template<typename T, typename R>
    Matrix<boolean> operator==(const Matrix_base<T> &mxb1, const R val)
    {
        return machine::make_mask(mxb1, Condition::is, val);
    }

    template<typename T, typename R>
    Matrix<boolean> operator!=(const Matrix_base<T> &mxb1, const R val)
    {
        return machine::make_mask(mxb1, Condition::is_not, val);
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
    requires std::is_arithmetic_v<T>
    std::ostream& operator<<(std::ostream& os, const Matrix_base<T> &mxb1)
    {   
        
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};
        for(size_t r{} ; r<r1 ; r++){
            os<<"| ";
            for(size_t c{} ; c<c1 ; c++){
                //auto s = std::to_string(mxb1(r, c)); //make show only N chars for uniform view
                if(c<c1-1){os<<std::fixed<<std::setprecision(2)<<mxb1(r, c)<<" | ";}
                else{os<<mxb1(r, c);}
            }
            os<<" |\n";
        }
        os<<'\n';
        return os;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Matrix_base<T> &mxb1)
    {
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};

        for(size_t r{} ; r<r1 ; r++){
            os<<"| ";
            for(size_t c{} ; c<c1 ; c++){
                if(c<c1-1){os<<std::fixed<<std::setprecision(2)<<mxb1(r, c)<<" | ";}
                else{os<<mxb1(r, c);}
            }
            os<<" |\n";
        }
        os<<'\n';
        return os;
    }
}
namespace machine
{   
    template<typename T, typename R>
    auto concat(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2, const axis ax)
    {   
        auto mxb1_rows{mxb1.get_rows()};
        auto mxb1_cols{mxb1.get_cols()};
        auto mxb2_rows{mxb2.get_rows()};
        auto mxb2_cols{mxb2.get_cols()};

        if(ax==axis::Y){
            if(mxb1_cols != mxb2_cols){
                throw dimension_mismatch(std::format("stacking_rows = true, cols must match, mxb1: {}, mxb2: {}", mxb1_cols, mxb2_cols));
            }
        }
        else{
            if(mxb1_rows != mxb2_rows){
                throw dimension_mismatch(std::format("stacking_rows = false, rows must match, mxb1: {}, mxb2: {}", mxb1_rows, mxb2_rows));
            }
        }

        using common_type = std::common_type_t<T, R>;
        Matrix<common_type> mx{(ax == axis::Y)? Matrix<common_type>{mxb1_rows+mxb2_rows, mxb1_cols} : Matrix<common_type>{mxb1_rows, mxb1_cols+mxb2_cols}};

        if(ax == axis::Y)
        {
            
            for(size_t c{} ; c<mxb1_cols ; c++)
            {   
                size_t iter_row{0};
                for(size_t r{} ; r<mxb1_rows+mxb2_rows ; r++)
                {
                    mx(r, c) = (r<mxb1_rows) ?  mxb1(r, c) : mxb2(iter_row++, c);
                }
            }
        }
        else
        {   
            
            for(size_t r{} ; r<mxb1_rows ; r++)
            {
                size_t iter_col{0};
                for(size_t c{} ; c<mxb1_cols+mxb2_cols ; c++)
                {
                    mx(r, c) = (c<mxb1_cols) ? mxb1(r, c) : mxb2(r, iter_col++);
                }
            }
        }
        return mx;
    }
}

