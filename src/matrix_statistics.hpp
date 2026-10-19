#pragma once

#include "matrix_base.hpp"
#include "matrix_method.hpp"

template<typename T>
class Matrix;

namespace matrix_statistics
{   template<typename T>
    T minimum(const Matrix_base<T>& mxb1)
    {   
        auto min{mxb1(0,0)};
        for(size_t r{} ; r<mxb1.get_rows() ; r++){
            for(size_t c{} ; c<mxb1.get_cols ; c++){
                if(mxb1(r, c)<min){min = mxb1(r, c);}
            }
        }
    }

    template<typename T>
    T maximum(const Matrix_base<T>& mxb1)
    {   
        auto max{mxb1(0,0)};
        for(size_t r{} ; r<mxb1.get_rows() ; r++){
            for(size_t c{} ; c<mxb1.get_cols ; c++){
                if(mxb1(r, c)>max){max = mxb1(r, c);}
            }
        }
    }

    template<typename T>
    double sum(const Matrix_base<T> &mxb1)
    {
        Matrix<T> mx{1, 1, 0};
        matrix_method::broadcast(mx, mxb1, Operation::add);
        return mx(0,0);
    }

    template<typename T>
    Matrix<T> sum(const Matrix_base<T> &mxb1, bool row_wise)
    {   
        Matrix<T> mx{(row_wise)? Matrix{mxb1.get_rows(), 1} : Matrix<T>{1, mxb1.get_cols()}};
        matrix_method::broadcast(mx, mxb1, Operation::add);
        return mx;
    }

    template<typename T>
    double avg(const Matrix_base<T> &mxb1)
    {   
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};

        double avarage{sum(mxb1)};
        return avarage / (r1*c1);
    }

    template<typename T>
    Matrix<T> avg(const Matrix_base<T> &mxb1, bool row_wise)
    {
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};

        Matrix<T> avarage{sum(mxb1, (row_wise)? true : false)};
        matrix_method::broadcast(avarage, (row_wise)? c1 : r1, Operation::divide);
        return avarage;
    }

    template<typename T>
    double variance(const Matrix_base<T> &mxb1, double degree)
    {   
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};
        double mean{avg(mxb1)};

        Matrix<T> var{mxb1};
        var -= mean;
        matrix_method::broadcast(var, 2, Operation::power);
        double var_out{sum(var)};
        return  var_out / (r1*c1-degree);
    }

    template<typename T>
    Matrix<T> variance(const Matrix_base<T> &mxb1, const bool row_wise, double degree)
    {   
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};
        Matrix<T> mean{avg(mxb1, row_wise)};

        Matrix<T> var{mxb1};
        var -= mean;
        matrix_method::broadcast(var, 2, Operation::power);
        Matrix<T> var_out {sum(var, row_wise)};
        matrix_method::broadcast(var_out, (row_wise)? c1-degree : r1-degree, Operation::divide);
        return var_out;
    }

    template<typename T>
    double std(const Matrix_base<T> &mxb1, double degree)
    {   
        return std::sqrt(variance(mxb1, degree));
    }

    template<typename T>
    Matrix<T> std(const Matrix_base<T> &mxb1, const bool row_wise, double degree)
    {   
        Matrix<T> standard_deviation{variance(mxb1, row_wise, degree)};
        matrix_method::broadcast(standard_deviation, 2.0, Operation::root);
        return standard_deviation;
    }

    template<typename T>
    Matrix<T> normal_distribution(const Matrix_base<T> &mxb1, const bool row_wise)
    {   
        double small_num {0.001};
        Matrix<T> n_dist{mxb1 - avg(mxb1, row_wise)};

        Matrix<T> variance_squared{
            matrix_method::broadcast(variance(mxb1, row_wise) + small_num, 2.0, Operation::root)
        };

        return n_dist / variance_squared;
    }

    template<typename T>
    Matrix<T> normal_distribution(const Matrix_base<T> &mxb1); //not yet

    template<typename T>
    Matrix<T> normal_distribution(const Matrix_base<T> &mxb1, const bool row_wise);

    template<typename T>
    Matrix<T> min_max_norm(const Matrix_base<T> &mxb1); //not yet

    template<typename T>
    Matrix<T> min_max_norm(const Matrix_base<T> &mxb1, const bool row_wise); //not yet

    template<typename T>
    Matrix<T> robust_scale();

    template<typename T>
    Matrix<T> z_score();

};

