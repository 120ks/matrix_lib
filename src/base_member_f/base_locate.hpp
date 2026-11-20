#pragma once

#include "non_member_functions.hpp"

namespace machx
{   
    //focused on locating elements and returning them as (usually) references or ref matrices (mask_views)

    template<typename Tmx, typename T>
    template<typename R>
    T& Matrix_base<Tmx, T>::closest(const R &value){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix_base<Tmx, T>& mx = *this;
        auto* closest = &mx(0,0);
        auto distance = std::abs(*closest - value);
        for(long long row{} ; row < rows; row++){
            for(long long col{} ; col < cols ; col++){
                if(std::abs(mx(row, col) - value) < distance){
                    closest = &mx(row, col);
                    distance = std::abs(*closest - value);
                }
            }
        }
        return *closest;
    }

    template<typename Tmx, typename T>
    template<typename R>
    const T& Matrix_base<Tmx, T>::closest(const R &value)const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        const Matrix_base<Tmx, T>& mx = *this;
        const auto* closest = &mx(0,0);
        auto distance = std::abs(*closest - value);
        for(long long row{} ; row < rows; row++){
            for(long long col{} ; col < cols ; col++){
                if(std::abs(mx(row, col) - value) < distance){
                    closest = &mx(row, col);
                    distance = std::abs(*closest - value);
                }
            }
        }
        return *closest;
    }

    //MIN AND MAX
    template<typename Tmx, typename T>
    const T& Matrix_base<Tmx, T>::min()const{
        const Matrix_base<Tmx, T>& mx = *this;
        const T* min_val = &mx(0, 0);
        auto rows = this->get_rows();
        auto cols = this->get_cols();

        for(long long row{} ; row < rows; row++){
            for(long long col{} ; col < cols ; col++){
                if(mx(row, col) < *min_val){
                    min_val = &mx(row, col);
                }
            }
        }
        return *min_val;
    }

    template<typename Tmx, typename T>
    const T& Matrix_base<Tmx, T>::max()const{
        const Matrix_base<Tmx, T>& mx = *this;
        const T* max_val = &mx(0, 0);
        auto rows = this->get_rows();
        auto cols = this->get_cols();

        for(long long row{} ; row < rows; row++){
            for(long long col{} ; col < cols ; col++){
                if(mx(row, col) > *max_val){
                    max_val = &mx(row, col);
                }
            }
        }
        return *max_val;
    }
    template<typename Tmx, typename T>
    const Mask_view<T> Matrix_base<Tmx, T>::row_min()const{
        auto rows = this->get_rows();
        Matrix<bool> mask{rows, 1, true};
        Mask_view<T> min_mx{this->cols_at(0), mask};
        auto& mx = const_cast<Matrix_base<Tmx, T>&>(*this);

        for(long long row{} ; row<rows ; row++){
            min_mx.get_ptr_at(row, 0) = &(mx.rows_at(row).min());
        }
        return min_mx;
    }
    template<typename Tmx, typename T>
    const Mask_view<T> Matrix_base<Tmx, T>::row_max()const{
        auto rows = this->get_rows();
        Matrix<bool> mask{rows, 1, true};
        Mask_view<T> max_mx{this->cols_at(0), mask};
        auto& mx = const_cast<Matrix_base<Tmx, T>&>(*this);

        for(long long row{} ; row<rows ; row++){
            max_mx.get_ptr_at(row, 0) = &(mx.rows_at(row).max());
        }
        return max_mx;
    }

    template<typename Tmx, typename T>
    const Mask_view<T> Matrix_base<Tmx, T>::col_min()const{
        auto cols = this->get_cols();
        Matrix<bool> mask{1, cols, true};
        Mask_view<T> min_mx{this->rows_at(0), mask};
        auto& mx = const_cast<Matrix_base<Tmx, T>&>(*this);

        for(long long col{} ; col<cols ; col++){
            min_mx.get_ptr_at(0, col) = &(mx.cols_at(col).min());
        }
        return min_mx;
    }

    template<typename Tmx, typename T>
    const Mask_view<T> Matrix_base<Tmx, T>::col_max()const{
        auto cols = this->get_cols();
        Matrix<bool> mask{1, cols, true};
        Mask_view<T> max_mx{this->rows_at(0), mask};
        auto& mx = const_cast<Matrix_base<Tmx, T>&>(*this);

        for(long long col{} ; col<cols ; col++){
            max_mx.get_ptr_at(0, col) = &(mx.cols_at(col).max());
        }
        return max_mx;
    }

    template<typename Tmx, typename T>
    T& Matrix_base<Tmx, T>::min(){
        Matrix_base<Tmx, T>& mx = *this;
        T* min_val = &mx(0, 0);
        auto rows = this->get_rows();
        auto cols = this->get_cols();

        for(long long row{} ; row < rows; row++){
            for(long long col{} ; col < cols ; col++){
                if(mx(row, col) < *min_val){
                    min_val = &mx(row, col);
                }
            }
        }
        return *min_val;
    }

    template<typename Tmx, typename T>
    T& Matrix_base<Tmx, T>::max(){
        Matrix_base<Tmx, T>& mx = *this;
        T* max_val = &mx(0, 0);
        auto rows = this->get_rows();
        auto cols = this->get_cols();

        for(long long row{} ; row < rows; row++){
            for(long long col{} ; col < cols ; col++){
                if(mx(row, col) > *max_val){
                    max_val = &mx(row, col);
                }
            }
        }
        return *max_val;
    }
    template<typename Tmx, typename T>
    Mask_view<T> Matrix_base<Tmx, T>::row_min(){
        auto rows = this->get_rows();
        Matrix<bool> mask{rows, 1, true};
        Mask_view<T> min_mx{this->cols_at(0), mask};

        for(long long row{} ; row<rows ; row++){
            min_mx.get_ptr_at(row, 0) = &(this->rows_at(row).min());
        }
        return min_mx;
    }
    template<typename Tmx, typename T>
    Mask_view<T> Matrix_base<Tmx, T>::row_max(){
        auto rows = this->get_rows();
        Matrix<bool> mask{rows, 1, true};
        Mask_view<T> max_mx{this->cols_at(0), mask};

        for(long long row{} ; row<rows ; row++){
            max_mx.get_ptr_at(row, 0) = &(this->rows_at(row).max());
        }
        return max_mx;
    }

    template<typename Tmx, typename T>
    Mask_view<T> Matrix_base<Tmx, T>::col_min(){
        auto cols = this->get_cols();
        Matrix<bool> mask{1, cols, true};
        Mask_view<T> min_mx{this->rows_at(0), mask};

        for(long long col{} ; col<cols ; col++){
            min_mx.get_ptr_at(0, col) = &(this->cols_at(col).min());
        }
        return min_mx;
    }

    template<typename Tmx, typename T>
    Mask_view<T> Matrix_base<Tmx, T>::col_max(){
        auto cols = this->get_cols();
        Matrix<bool> mask{1, cols, true};
        Mask_view<T> max_mx{this->rows_at(0), mask};

        for(long long col{} ; col<cols ; col++){
            max_mx.get_ptr_at(0, col) = &(this->cols_at(col).max());
        }
        return max_mx;
    }
}
