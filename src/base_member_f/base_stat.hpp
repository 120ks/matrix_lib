#pragma once

#include "non_member_functions.hpp"

namespace machx
{   

    //basic sum/mean/median/avg/std statistics

    template<typename Tmx, typename T>
    T Matrix_base<Tmx, T>::sum()const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        const Matrix_base<Tmx, T>& mx = *this;
        T sum_val{};
        for(long long row{} ; row < rows; row++){
            for(long long col{} ; col < cols ; col++){
                sum_val += mx(row, col);
            }
        }
        return sum_val;
    }

    template<typename Tmx, typename T>
    Matrix<T> Matrix_base<Tmx, T>::row_sum()const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        const Matrix_base<Tmx, T>& mx = *this;
        Matrix<T> sums{rows, 1, 0};

        for(long long row{} ; row < rows ; row++){
            sums(row, 0) = mx.rows_at(row).sum();
        }
        return sums;
    }


    template<typename Tmx, typename T>
    Matrix<T> Matrix_base<Tmx, T>::col_sum()const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        const Matrix_base<Tmx, T>& mx = *this;
        Matrix<T> sums{1, cols, 0};

        for(long long col{} ; col<cols; col++){
            sums(0, col) = mx.cols_at(col).sum();
        }
        return sums;
    }

    template<typename Tmx, typename T>
    double Matrix_base<Tmx, T>::mean()const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        return this->sum() / (rows*cols);
    }
    template<typename Tmx, typename T>
    Matrix<double> Matrix_base<Tmx, T>::row_mean()const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix<T> means = this->row_sum();
        return means /= cols;
    }
    template<typename Tmx, typename T>
    Matrix<double> Matrix_base<Tmx, T>::col_mean()const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix<T> means = this->col_sum();
        return means /= rows;
    }

    template<typename Tmx, typename T>
    T Matrix_base<Tmx, T>::median()const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        auto flat_arr = this->flatten();
        flat_arr.sort();

        return flat_arr(0, static_cast<long long>((rows*cols-1)/2));
    }

    template<typename Tmx, typename T>
    Matrix<T> Matrix_base<Tmx, T>::row_median()const{
        auto rows = this->get_rows();
        Matrix<T> row_med{rows, 1};
        for(long long row{} ; row<rows ; row++){
            row_med(row, 0) = this->rows_at(row).median();
        }
        return row_med;
    }
    template<typename Tmx, typename T>
    Matrix<T> Matrix_base<Tmx, T>::col_median()const{
        auto cols = this->get_cols();
        Matrix<T> col_med{1, cols};
        for(long long col{} ; col<cols ; col++){
            col_med(0, col) = this->cols_at(col).median();
        }
        return col_med;

    }

    template<typename Tmx, typename T>
    auto Matrix_base<Tmx, T>::var(float degree)const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix<double> variance{*this - this->mean()};
        variance.power(2);
        return variance.sum() / (rows*cols - degree);
    }

    template<typename Tmx, typename T>
    Matrix<double> Matrix_base<Tmx, T>::row_var(float degree)const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix<double> variances{*this - this->row_mean()};
        variances.power(2);
        return variances.row_sum() /= (cols-degree);
    }

    template<typename Tmx, typename T>
    Matrix<double> Matrix_base<Tmx, T>::col_var(float degree)const{
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix<double> variances{*this - this->col_mean()};
        variances.power(2);
        return variances.col_sum() /= (rows-degree);
    }

    template<typename Tmx, typename T>
    auto Matrix_base<Tmx, T>::std(float degree)const{
        return std::sqrt(this->var(degree));
    }

    template<typename Tmx, typename T>
    Matrix<double> Matrix_base<Tmx, T>::row_std(float degree)const{
        Matrix<double> sds {this->row_var(degree)};
        sds.root(2);
        return sds;
    }

    template<typename Tmx, typename T>
    Matrix<double> Matrix_base<Tmx, T>::col_std(float degree)const{
        Matrix<double> sds {this->col_var(degree)};
        sds.root(2);
        return sds;
    }
}
