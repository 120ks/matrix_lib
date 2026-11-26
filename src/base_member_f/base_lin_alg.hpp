#pragma once

#include "non_member_functions.hpp"
#include "matmul.hpp" //matmul (impelemtations) included trough this header
namespace machx
{      
    template<typename Tmx, typename T>
    template<typename R>
    requires (std::is_same_v<R, float>)
    Matrix<float> Matrix_base<Tmx, T>::matmul(const Matrix<float>& matrix2)const{
        const auto rows_1 = this->get_rows();
        const auto ks = this->get_cols();
        const auto rows_2 = matrix2.get_rows();
        const auto cols_2 = matrix2.get_cols();


        Matrix<T> matrix3{rows_1, cols_2, 0};
        const auto* base1 = this->begin();
        const auto* base2 = matrix2.begin();
        auto* base3 = matrix3.begin();

        machx::matmul_ps_pack(base1, base2, base3, ks, rows_1, cols_2);
        return matrix3;
    }
    template<typename Tmx, typename T>
    template<typename R>
    requires (std::is_same_v<R, double>)
    Matrix<double> Matrix_base<Tmx, T>::matmul(const Matrix<double>& matrix2)const{
        const auto rows_1 = this->get_rows();
        const auto ks = this->get_cols();
        const auto rows_2 = matrix2.get_rows();
        const auto cols_2 = matrix2.get_cols();


        Matrix<T> matrix3{rows_1, cols_2, 0};
        const auto* base1 = this->begin();
        const auto* base2 = matrix2.begin();
        auto* base3 = matrix3.begin();

        machx::matmul_pd_pack(base1, base2, base3, ks, rows_1, cols_2);
        return matrix3;
    }
    
    template<typename Tmx, typename T>
    template<typename R, typename Umx, typename U>
    requires (!std::is_floating_point_v<R>)
    Matrix<T> Matrix_base<Tmx, T>::matmul(const Matrix_base<Umx, U>& mxb2)const{
        Matrix<double> mx1{*this};
        Matrix<double> mx2{mxb2};
        return mx1.matmul(mx2);
    }

    template<typename Tmx, typename T>
    Matrix<T> Matrix_base<Tmx, T>::matmul_t(const Matrix<T>& matrix2)const{
        const auto rows_1 = this->get_rows();
        const auto ks = this->get_cols();
        const auto rows_2 = matrix2.get_rows();
        const auto cols_2 = matrix2.get_cols();

        Matrix<T> matrix3{rows_1, cols_2, 0};
        const auto* base1 = this->begin();
        const auto* base2 = matrix2.begin();
        auto* base3 = matrix3.begin();



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
