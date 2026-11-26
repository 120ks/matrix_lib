#pragma once

#include "non_member_functions.hpp"
#include <cmath>

//
//SCALES AND NORMALIZERS
//
namespace machx
{
    //CLASSIFICATION AND GROUPING

    template<typename Tmx, typename T>
    Matrix<T> k_means(Matrix_base<Tmx, T> &mxb1, long long groups, long long iter = 1000);

    template<typename Tmx, typename T>
    Matrix<T> HMM(Matrix_base<Tmx, T> &observations, long long states);

    template<typename Tmx, typename T>
    Matrix<T> distribution(Matrix_base<Tmx, T> &observations, long long accuracy, bool hide_future = false, long long steps = 10);

    template<typename Tmx, typename T>
    Matrix<T> conditional_distribution(Matrix_base<Tmx, T> &observations, long long states);

}

namespace machx{
    //QUICK ALGOS

}


