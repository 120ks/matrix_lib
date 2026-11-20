#pragma once

#include "non_member_functions.hpp"

namespace machx
{   
    //focused on a degree more compilated statistics, covariance, correlation

    template<typename Tmx, typename T>
    double Matrix_base<Tmx, T>::auto_corr(long long lag, float degree){
        Matrix<T> flat_mx= this->flatten();
        Matrix<T> lag_mx = flat_mx.cols_at(0, -lag);
        flat_mx.A() = flat_mx.cols_at(lag, 0);  
        return lag_mx.corr(flat_mx, degree);
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    double Matrix_base<Tmx, T>::corr(const Matrix_base<Rmx, R>& mxb2, float degree){
        auto covar = this->cov(mxb2, degree);
        return covar / (this->std(degree) * mxb2.std(degree));
    }
    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    Matrix<double> Matrix_base<Tmx, T>::row_corr(const Matrix_base<Rmx, R>& mxb2, float degree){
        auto rows = this->get_rows();
        auto rows_mxb2 = mxb2.get_rows();
        if(this->get_cols() != mxb2.get_cols()){
            throw machx::dimension_mismatch("row_corr col mismatch");
        }
        Matrix<double> cmx{rows, rows_mxb2};
        Matrix_base<Tmx, T>& mx = *this;
        for(long long row{} ; row < rows ; row++){
            for(long long row2{} ; row2 < rows_mxb2 ; row2++){
                cmx(row, row2) = mx.rows_at(row).corr(mxb2.rows_at(row2), degree);  
            }
        }
        return cmx;
    }
    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    Matrix<double> Matrix_base<Tmx, T>::col_corr(const Matrix_base<Rmx, R>& mxb2, float degree){
        auto cols = this->get_cols();
        auto cols_mxb2 = mxb2.get_cols();
        if(this->get_rows() != mxb2.get_rows()){
            throw machx::dimension_mismatch("col_corr row mismatch");
        }

        Matrix<double> cmx{cols, cols_mxb2};
        Matrix_base<Tmx, T>& mx = *this;
        for(long long col{} ; col < cols ; col++){
            for(long long col2{} ; col2 < cols_mxb2 ; col2++){
                cmx(col, col2) = mx.cols_at(col).corr(mxb2.cols_at(col2), degree);  
            }
        }
        return cmx;
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    double Matrix_base<Tmx, T>::cov(const Matrix_base<Rmx, R>& mxb2, float degree){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        auto copy_this = this->copy();
        auto copy_other = mxb2.copy();
        quick_throw::throw_if_dimension_mismatch(*this, mxb2, "in cov");
        
        copy_this -= this->mean();
        copy_other -= mxb2.mean();
        copy_this*=copy_other;
        return copy_this.sum() / (rows*cols-degree);
    }

    template<typename Tmx, typename T>
    Matrix<double> Matrix_base<Tmx, T>::cov_matrix(float degree){
        auto cols = this->get_cols();
        Matrix<double> cmx{cols, cols};
        Matrix_base<Tmx, T>& mx = *this;
        for(long long col{} ; col < cols ; col++){
            for(long long col2{} ; col2 < cols ; col2++){
                cmx(col, col2) = mx.cols_at(col).cov(mx.cols_at(col2), degree);  
            }
        }
        return cmx;
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    Matrix<double> Matrix_base<Tmx, T>::row_cov(const Matrix_base<Rmx, R>&mxb2, float degree){ //each row = feature
        auto rows = this->get_rows();
        auto rows_mxb2 = mxb2.get_rows();
        if(this->get_cols() != mxb2.get_cols()){
            throw machx::dimension_mismatch("row_cov col mismatch");
        }

        Matrix<double> cmx{rows, rows_mxb2};
        Matrix_base<Tmx, T>& mx = *this;
        for(long long row{} ; row < rows ; row++){
            for(long long row2{} ; row2 < rows_mxb2 ; row2++){
                cmx(row, row2) = mx.rows_at(row).cov(mxb2.rows_at(row2), degree);  
            }
        }
        return cmx;
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    Matrix<double> Matrix_base<Tmx, T>::col_cov(const Matrix_base<Rmx, R>&mxb2, float degree){
        auto cols = this->get_cols();
        auto cols_mxb2 = mxb2.get_cols();
        if(this->get_rows() != mxb2.get_rows()){
            throw machx::dimension_mismatch("col_cov row mismatch");
        }

        Matrix<double> cmx{cols, cols_mxb2};
        Matrix_base<Tmx, T>& mx = *this;
        for(long long col{} ; col < cols ; col++){
            for(long long col2{} ; col2 < cols_mxb2 ; col2++){
                cmx(col, col2) = mx.cols_at(col).cov(mxb2.cols_at(col2), degree);  
            }
        }
        return cmx;
    }
}
