#pragma once

#include "non_member_functions.hpp"

namespace machx
{   
    //
    //LARGE PORTION OF THE FUNCTIONALITY IS FOCUSED ON MOVING DATA AROUND INSIDE OR TO NEW MX

    template<typename Tmx, typename T>
    Matrix<T> Matrix_base<Tmx, T>::transpose()const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix<T> mx_tp{cols, rows};
        auto& mx = *this;
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){
                mx_tp(col, row) = mx(row, col);
            }
        }
        return mx_tp;
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::sort(){
        if(this->begin()){
            std::sort(this->begin(), this->end());
            return static_cast<Tmx&>(*this);
        }
        *this = this->copy().sort();
        return static_cast<Tmx&>(*this);
        
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::reverse_rows()const{
        auto rows = this->get_rows();
        auto copy_mx = this->copy();
        for(long long row{} ; row < rows ; row++){
            this->rows_at(row) = copy_mx.rows_at(-row-1);
        }
        return static_cast<Tmx&>(*this);
    }


    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::reverse_cols()const{
        auto cols = this->get_cols();
        auto copy_mx = this->copy();
        for(long long col{} ; col < cols ; col++){
            this->cols_at(col) = copy_mx.cols_at(-col-1);
        }
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Matrix<T> Matrix_base<Tmx, T>::flatten()const{
        auto flat_size = this->get_rows() * this->get_cols();
        Matrix<T> flat_mx{*this};
        flat_mx.cols = flat_size;
        flat_mx.rows = 1;
        return flat_mx;
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    Matrix<T> Matrix_base<Tmx, T>::stack(const Matrix_base<Rmx, R> &mxb2)const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix<T> stacked{rows+mxb2.get_rows(), cols};
        stacked.rows_at(0, rows) = *this;
        stacked.rows_at(rows, 0) = mxb2;
        return stacked;
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    Matrix<T> Matrix_base<Tmx, T>:: stack_side(const Matrix_base<Rmx, R> &mxb2)const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix<T> stacked{rows, cols + mxb2.get_cols()};
        stacked.cols_at(0, cols) = *this;
        stacked.cols_at(cols, 0) = mxb2;
        return stacked;
    }

    template<typename Tmx, typename T>
    template<typename R>
    Matrix<T> Matrix_base<Tmx, T>::concat_cols(std::initializer_list<R> indexes)const{
        Matrix<T> mx{this->get_rows(), static_cast<long long>(indexes.size())};
        long long mx_i{0};
        for(auto i : indexes){
            mx.cols_at(mx_i++) = this->cols_at(static_cast<long long>(i));
        }
        return mx;
    }
    template<typename Tmx, typename T>
    template<typename R>
    Matrix<T> Matrix_base<Tmx, T>::concat_rows(std::initializer_list<R> indexes)const{
        Matrix<T> mx{static_cast<long long>(indexes.size()), this->get_cols()};
        long long mx_i{0};
        for(auto i : indexes){
            mx.rows_at(mx_i++) = this->rows_at(static_cast<long long>(i));
        }
        return mx;
    }

    template<typename Tmx, typename T>
    template<typename R>
    Mask_view<T> Matrix_base<Tmx, T>::ref_rows(std::initializer_list<R> indexes){
        auto new_rows = indexes.size();
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix<bool> mask{rows, cols, false};
        for(auto i : indexes){
            mask.rows_at(i) = true;
        }
        auto ref = this->operator[](mask);
        ref.cols = cols;
        ref.rows = new_rows;
        return ref;
    }

    template<typename Tmx, typename T>
    template<typename R>
    Mask_view<T> Matrix_base<Tmx, T>::ref_cols(std::initializer_list<R> indexes){
        auto new_cols = indexes.size();
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix<bool> mask{rows, cols, false};
        for(auto i : indexes){
            mask.cols_at(i) = true;
        }
        auto ref = this->operator[](mask);
        ref.cols = new_cols;
        ref.rows = rows;
        return ref;
    }
    //SHIFT
    template<typename Tmx, typename T>
    Matrix<T> Matrix_base<Tmx, T>::shift_rows(long long offset)const{
        auto rows = this->get_rows();
        auto copy_mx = this->copy();
        auto start = (offset<0)? 0 : offset;
        auto end = (offset<0) ? rows+offset : rows;
        copy_mx.rows_at(start-offset, end-offset) = copy_mx.rows_at(start, end).copy();

        start = (offset<0) ? 0 : rows-offset;
        end = (offset<0) ? -offset : rows;
        copy_mx.rows_at(start, end) = std::numeric_limits<T>::quiet_NaN();        
        return copy_mx;
    }

    template<typename Tmx, typename T>
    Matrix<T> Matrix_base<Tmx, T>::shift_cols(long long offset)const{
        auto cols = this->get_cols();
        auto copy_mx = this->copy();
        auto start = (offset>0)? 0 : offset;
        auto end = (offset>0) ? cols-offset : cols;
        copy_mx.cols_at(start+offset, end+offset) = copy_mx.cols_at(start, end).copy();

        start = (offset>0) ? 0 : cols+offset;
        end = (offset>0) ? offset : cols;

        copy_mx.cols_at(start, end) = std::numeric_limits<T>::quiet_NaN();        
        return copy_mx;
    }
}
