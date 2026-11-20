#pragma once

#include "non_member_functions.hpp"

namespace machx
{   
    //normalization and scaling

    template<typename Tmx, typename T>
    template<typename R, typename U>
    Tmx& Matrix_base<Tmx, T>::min_max(R min_t, U max_t){
        T min_val = this-> min();
        T max_val = this-> max();

        (*this -= min_val) /= (max_val - min_val);
        (*this *= (max_t - min_t)) += min_t;
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::z_scores(float degree){
        auto mean = this->mean();
        auto sd = this->std(degree);
        (*this -= mean) /= sd;
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::robust_scale(float P1, float P2){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix<T> copy_mx = this->copy();
        copy_mx.sort();
        auto& rank_arr = copy_mx.get_matrix();
        auto Q1 = rank_arr[static_cast<long long>(rows*cols*P1) - 1];
        auto Q3 = rank_arr[static_cast<long long>(rows*cols*P2) - 1];
        auto IQR = Q3 - Q1;
        auto median = rank_arr[rows*cols/2 - 1];

        (*this -= median) /= IQR;
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::robust_z(float P1, float P2, float degree){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix_base<Tmx, T>& mx = *this;
        Matrix<T> copy_mx = this->copy();
        copy_mx.sort();
        auto& rank_arr = copy_mx.get_matrix();
        auto Q1 = rank_arr[static_cast<long long>(rows*cols*P1) - 1];
        auto Q3 = rank_arr[static_cast<long long>(rows*cols*P2) - 1];

        double means = mx[(mx >= Q1 && mx <= Q3)].mean();
        double sd = mx[(mx >= Q1 && mx <= Q3)].std();
        (*this -= means) /= sd;
        return static_cast<Tmx&>(*this);
    }

    //BATCH; LAYER; GROUP NORMs
    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::batch_norm(float degree){
        auto means = this->col_mean();
        auto sd = this->col_std();
        (*this -= means) /= sd;
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::layer_norm(float degree){
        auto means = this->row_mean();
        auto sd = this->row_std();
        (*this -= means) /= sd;
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::group_norm(long long groups, float degree){
        auto cols = this->get_cols();
        if(cols % groups != 0){
            throw std::runtime_error("cols must be divisible by group N, in group norm");
        }
        auto group_members = cols / groups;
        auto start{0};

        for(long long group ; group<groups ; group++){
            auto end = start + group_members;
            this->cols_at(start, end).layer_norm(degree);
            start = end;
        }
        return static_cast<Tmx&>(*this);
    }
}
