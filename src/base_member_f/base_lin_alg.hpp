#pragma once

#include "non_member_functions.hpp"

namespace machx
{   
    template<typename Tmx, typename T>
    template<typename R>
    requires (std::is_same_v<Tmx, Matrix<T>>)
    Matrix<T> Matrix_base<Tmx, T>::matmul(const Matrix<R>& matrix2)const{
        auto rows_1 = this->get_rows();
        auto cols_1 = this->get_cols();
        auto rows_2 = matrix2.get_rows();
        auto cols_2 = matrix2.get_cols();

        const Matrix<T>& matrix1 = static_cast<const Tmx&>(*this);
        Matrix<T> matrix3{rows_1, cols_2, 0};

        const auto& arr1 = matrix1.get_matrix();
        const auto& arr2 = matrix2.get_matrix();
        auto& arr3 = matrix3.get_matrix();

        for(long long row{} ; row<rows_1 ; row++){
            for(long long k{} ; k < cols_1 ; k++){
                auto val = arr1[row*cols_1 + k];
                for(long long col{} ; col<cols_2 ; col++){
                    arr3[row * cols_2 + col] += val * arr2[k * cols_2 + col];
                }
            }
        }
        return matrix3;
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    requires (!std::is_same_v<Tmx, Matrix<T>>)
    Matrix<T> Matrix_base<Tmx, T>::matmul(const Matrix_base<Rmx, R>& mxb2)const{
        auto rows_1 = this->get_rows();
        auto cols_1 = this->get_cols();
        auto rows_2 = mxb2.get_rows();
        auto cols_2 = mxb2.get_cols();

        auto matrix1 = this->copy();
        auto matrix2 = mxb2.copy();
        Matrix<T> matrix3{rows_1, cols_2, 0};

        auto& arr1 = matrix1.get_matrix();
        auto& arr2 = matrix2.get_matrix();
        auto& arr3 = matrix3.get_matrix();

        for(long long row{} ; row<rows_1 ; row++){
            for(long long k{} ; k < cols_1 ; k++){
                auto val = arr1[row*cols_1 + k];
                for(long long col{} ; col<cols_2 ; col++){
                    arr3[row * cols_2 + col] += val * arr2[k * cols_2 + col];
                }
            }
        }
        return matrix3;
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    double Matrix_base<Tmx, T>::dot(const Matrix_base<Rmx, R>& mxb2)const{
        return ((*this) * mxb2).sum();
    }

    template<typename Tmx, typename T>
    double Matrix_base<Tmx, T>::manhattan()const{
        return this->copy().abs().sum();
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    double Matrix_base<Tmx, T>::manhattan(const Matrix_base<Rmx, R>& mxb2)const{
        return ((*this) - mxb2).abs().sum();
    }

    template<typename Tmx, typename T>
    double Matrix_base<Tmx, T>::euclidean()const{
        return std::sqrt(this->copy().power(2).sum());
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    double Matrix_base<Tmx, T>::euclidean(const Matrix_base<Rmx, R>& mxb2)const{
        return std::sqrt(((*this) - mxb2).power(2).sum());
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    double Matrix_base<Tmx, T>::cosine_sim(const Matrix_base<Rmx, R>&mxb2)const{
        this->dot(*mxb2) / (this->euclidean() * mxb2.euclidean());
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    double Matrix_base<Tmx, T>::cosine_dist(const Matrix_base<Rmx, R>&)const{
        return 1.0 - this->cosine_sim();
    }
}
