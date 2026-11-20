#pragma once

#include "non_member_functions.hpp"

namespace machx
{
    template<typename Tmx, typename T>
    template<typename R>
    requires std::is_same_v<R, bool>
    Matrix<bool> Matrix_base<Tmx, T>::row_mask(bool logical_and)const{
        if(logical_and){
            return this->copy_as<int>().row_mean() == 1;
        }
        return this->copy_as<int>().row_sum()>0;
    }

    template<typename Tmx, typename T>
    template<typename R>
    requires std::is_same_v<R, bool>
    Matrix<bool> Matrix_base<Tmx, T>::col_mask(bool logical_and)const{
        if(logical_and){
            return this->copy_as<int>().col_mean() == 1;
        }
        return this->copy_as<int>().col_sum()>0;
    }
    
    //NAN HANDLING
    template<typename Tmx, typename T>
    Matrix<bool> Matrix_base<Tmx, T>::mask_nan()const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        const Matrix_base<Tmx, T>& mx = *this;
        Matrix<bool> mask{rows, cols, false};
        T sum_val{};
        for(long long row{} ; row < rows; row++){
            for(long long col{} ; col < cols ; col++){
                mask(row, col) = std::isnan(mx(row, col));
            }
        }
        return mask;
    }

    template<typename Tmx, typename T>
    Matrix<T> Matrix_base<Tmx, T>::drop_nan_rows()const{
        auto mask = this->mask_nan();
        auto copy_mx = this->copy();
        return copy_mx[!mask.row_mask(true)];
    }

    template<typename Tmx, typename T>
    Matrix<T> Matrix_base<Tmx, T>::drop_nan_cols()const{
        auto mask = this->mask_nan();
        auto copy_mx = this->copy();
        return copy_mx[!mask.col_mask(true)];
    }
}
