#pragma once


#include "mask.hpp"
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <string>
#include <format>



namespace machx
{
    template<typename T> class Matrix;
    
    template<typename Tmx, typename T, typename Rmx, typename R>
    auto operator+(const Matrix_base<Tmx, T> &mxb1, const Matrix_base<Rmx, R> &mxb2)
    {   
        using common_type = std::common_type_t<T, R>;
        Matrix<common_type>mx{mxb1};
        mx += mxb2;
        return mx;
    }
    template<typename Tmx, typename T, typename Rmx, typename R>
    auto operator-(const Matrix_base<Tmx, T> &mxb1, const Matrix_base<Rmx, R> &mxb2)
    {   
        using common_type = std::common_type_t<T, R>;
        Matrix<common_type>mx{mxb1};
        mx -= mxb2;
        return mx;
    }
    template<typename Tmx, typename T, typename Rmx, typename R>
    auto operator*(const Matrix_base<Tmx, T> &mxb1, const Matrix_base<Rmx, R> &mxb2)
    {   
        using common_type = std::common_type_t<T, R>;
        Matrix<common_type>mx{mxb1};
        
        mx *= mxb2;
        
        return mx;
    }
    template<typename Tmx, typename T, typename Rmx, typename R>
    auto operator/(const Matrix_base<Tmx, T> &mxb1, const Matrix_base<Rmx, R> &mxb2)
    {   
        using common_type = std::common_type_t<T, R>;
        Matrix<common_type>mx{mxb1};
        mx /= mxb2;
        return mx;
    }

    template<typename Tmx, typename T, typename R>
    requires (std::is_arithmetic_v<R>)
    auto operator+(const Matrix_base<Tmx, T> &mxb1, const R &val) //handles both 
    {   
        using common_type = std::common_type_t<T, R>;
        Matrix<common_type>mx{mxb1};
        mx += val;
        return mx;
    }
    template<typename Tmx, typename T, typename R>
    requires (std::is_arithmetic_v<R>)
    auto operator-(const Matrix_base<Tmx, T> &mxb1, const R &val)
    {   
        using common_type = std::common_type_t<T, R>;
        Matrix<common_type>mx{mxb1};
        mx -= val;
        return mx;
    }
    template<typename Tmx, typename T, typename R>
    requires (std::is_arithmetic_v<R>)
    auto operator*(const Matrix_base<Tmx, T> &mxb1, const R &val)
    {   
        using common_type = std::common_type_t<T, R>;
        Matrix<common_type>mx{mxb1};
        
        mx *= val;
        
        return mx;
    }
    template<typename Tmx, typename T, typename R>
    requires (std::is_arithmetic_v<R>)
    auto operator/(const Matrix_base<Tmx, T> &mxb1, const R &val)
    {   
        using common_type = std::common_type_t<T, R>;
        Matrix<common_type>mx{mxb1};
        mx /= val;
        return mx;
    }
    //
    //NON MEMBER COMPARISON OPERATORS
    //
    template<typename Tmx, typename T, typename R>
    Matrix<bool> operator>(const Matrix_base<Tmx, T> &mxb1, const R val)
    {   
        return  machx::make_mask(mxb1, Condition::over, val);
    }

    template<typename Tmx, typename T, typename R>
    Matrix<bool> operator<(const Matrix_base<Tmx, T> &mxb1, const R val)
    {  
        return machx::make_mask(mxb1, Condition::under, val);
    }

    template<typename Tmx, typename T, typename R>
    Matrix<bool> operator>=(const Matrix_base<Tmx, T> &mxb1, const R val)
    {
        return machx::make_mask(mxb1, Condition::over_or, val);
    }

    template<typename Tmx, typename T, typename R>
    Matrix<bool> operator<=(const Matrix_base<Tmx, T> &mxb1, const R val)
    {
        return machx::make_mask(mxb1, Condition::under_or, val);
    }

    template<typename Tmx, typename T, typename R>
    Matrix<bool> operator==(const Matrix_base<Tmx, T> &mxb1, const R val)
    {   
        return machx::make_mask(mxb1, Condition::is, val);
    }

    template<typename Tmx, typename T, typename R>
    Matrix<bool> operator!=(const Matrix_base<Tmx, T> &mxb1, const R val)
    {
        return machx::make_mask(mxb1, Condition::is_not, val);
    }

    Matrix<bool> operator&&(const Matrix<bool> &mask1, const Matrix<bool>&mask2)
    {
        auto rows1{mask1.get_rows()};
        auto cols1{mask1.get_cols()};
        auto rows2{mask2.get_rows()};
        auto cols2{mask2.get_cols()};

        Matrix<bool> mask3{rows1, cols1, false};
        
        assert(rows1 == rows2 && "row_mismatch");
        assert(cols1 == cols2 && "col_mismatch");

        for(long long r{} ; r < rows1 ; r++){
            for(long long c{} ; c<cols1 ; c++){
                mask3(r, c) = (mask1(r, c) && mask2(r, c));
            }
        }
        return mask3;
    }

    Matrix<bool> operator||(const Matrix<bool> &mask1, const Matrix<bool> &mask2)
    {
        auto rows1{mask1.get_rows()};
        auto cols1{mask1.get_cols()};
        auto rows2{mask2.get_rows()};
        auto cols2{mask2.get_cols()};

        Matrix<bool> mask3{rows1, cols1, false};
        
        assert(rows1 == rows2 && "row_mismatch");
        assert(cols1 == cols2 && "col_mismatch");

        for(long long r{} ; r < rows1 ; r++){
            for(long long c{} ; c<cols1 ; c++){
                mask3(r, c) = (mask1(r, c) || mask2(r, c));
            }
        }
        return mask3;
    }

    template<typename Tmx, typename T>
    std::ostream& operator<<(std::ostream& os, const Matrix_base<Tmx, T> &mxb1)
    {   
        auto rows{mxb1.get_rows()};
        auto cols{mxb1.get_cols()};

        if(rows == 0 || cols == 0){
            os<<"[]\n";
        }

        for(long long row{} ; row<rows ; row++){
            os<<"[ ";
            for(long long col{} ; col<cols ; col++){
                auto val = static_cast<float>(mxb1(row, col));
                auto fs = std::format("{}", static_cast<float>(mxb1(row, col)));
                
                if(col<cols-1){os<<fs.substr(0, 6)<<" | ";}
                else{os<<fs.substr(0, 6);}
            }
            os<<" ]\n";
        }
        os<<'\n';
        return os;
    }
}
namespace machx
{   
    template<typename Tmx, typename T, typename Rmx, typename R>
    auto concat_top(const Matrix_base<Tmx, T> &mxb1, const Matrix_base<Rmx, R> &mxb2)
    {   
        auto mxb1_rows{mxb1.get_rows()};
        auto mxb1_cols{mxb1.get_cols()};
        auto mxb2_rows{mxb2.get_rows()};

        using common_type = std::common_type_t<T, R>;
        Matrix<common_type> mx{mxb1_rows+mxb2_rows, mxb1_cols};

        mx.rows_at(0, mxb1_rows) = mxb1;
        mx.rows_at(mxb1_rows, 0) = mxb2;
        return mx;
    }

    template<typename Tmx, typename T, typename Rmx, typename R>
    auto concat_side(const Matrix_base<Tmx, T> &mxb1, const Matrix_base<Rmx, R> &mxb2)
    {   
        auto mxb1_rows{mxb1.get_rows()};
        auto mxb1_cols{mxb1.get_cols()};
        auto mxb2_cols{mxb2.get_cols()};

        using common_type = std::common_type_t<T, R>;
        Matrix<common_type> mx{mxb1_rows, mxb1_cols+mxb2_cols};

        mx.cols_at(0, mxb1_cols) = mxb1;
        mx.rows_at(mxb1_cols, 0) = mxb2;
        return mx;
    }
}

