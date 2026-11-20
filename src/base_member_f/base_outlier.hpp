#pragma once

#include "non_member_functions.hpp"

namespace machx
{   
    //clipping and handling edge values

    template<typename Tmx, typename T>
    template<typename R, typename U>
    Tmx& Matrix_base<Tmx, T>::clip(R min, U max){
        this->M() < min = static_cast<T>(min);
        this->M() > max = static_cast<T>(max);
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::clip_Q(float min_p, float max_p){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix<T> copy_mx = this->copy();
        auto& sorted_arr = copy_mx.sort().get_matrix();
        
        auto min_v = sorted_arr[static_cast<long long>(rows*cols*min_p) - 1];
        auto max_v = sorted_arr[static_cast<long long>(rows*cols*max_p) - 1];
        this->clip(min_v, max_v);
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    template<typename R, typename U>
    Tmx& Matrix_base<Tmx, T>::clip_to_zero(R negative_p, U positive_p){
        auto& mx = static_cast<Tmx&>(*this);
        mx[(mx<positive_p) && (mx>negative_p)] = 0;
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    template<typename R>
    Tmx& Matrix_base<Tmx, T>::exhaust(R reach, float exh){
        reach = abs(reach);
        (*this) /= (reach / exh);

        this->tanh() *= 2;
        
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::exhaust_Q(float p1, float p2, float exh){
        auto flat_arr = this->flatten();
        flat_arr.sort();
        auto cols = flat_arr.get_cols();

        auto Q1 = flat_arr.cols_at(0, static_cast<long long>(cols*p1)).median();
        auto Q3 = flat_arr.cols_at(static_cast<long long>(cols*p2), 0).median();
        auto reach = std::abs(Q1) + std::abs(Q3);
        return this->exhaust(reach/2, exh);
    }
}
