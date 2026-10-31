#pragma once

#include "non_member_functions.hpp"
#include "matrix_base.hpp"


template<typename T>
class Matrix;

namespace machine
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
            for(size_t c{} ; c<mxb1.get_cols() ; c++){
                if(mxb1(r, c)>max){max = mxb1(r, c);}
            }
        }
        return max;
    }
    template<typename T>
    Matrix<T> argmax(const Matrix_base<T> &mxb1, const axis ax)
    {   
        auto rows_1{mxb1.get_rows()};
        auto cols_1{mxb1.get_cols()};

        Matrix<T> mx{ (ax == axis::X) ? Matrix<T>{rows_1, 1} : Matrix{1, cols_1}};
        auto mx_rows{mx.get_rows()};
        auto mx_cols{mx.get_cols()};
        
        for(size_t r{} ; rows_1 ; r++){
            for(size_t c{} ; c<cols_1 ; c++){
                if(mxb1(r, c) > mx(r%mx_rows, c%mx_cols)){
                    mx(r%mx_rows, c%mx_cols) = mxb1(r, c);
                }
            }
        }
        return mx;
    }

    template<typename T>
    double sum(const Matrix_base<T> &mxb1)
    {
        Matrix<double> mx{1, 1, 0};
        machine::broadcast(mx, mxb1, Operation::add);
        return mx(0,0);
    }

    template<typename T>
    Matrix<double> sum(const Matrix_base<T> &mxb1, const axis ax)
    {   
        Matrix<double> mx = (ax == axis::X)? Matrix<double>{mxb1.get_rows(), 1} : Matrix<double>{1, mxb1.get_cols()};
        machine::broadcast(mx, mxb1, Operation::add);
        return mx;
    }

    template<typename T>
    double average(const Matrix_base<T> &mxb1)
    {   
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};

        if(r1*c1 == 0){
            throw std::runtime_error("cols or rows are 0 in average");
        }

        double average{sum(mxb1)};
        return average / (r1*c1);
    }

    template<typename T>
    Matrix<double> average(const Matrix_base<T> &mxb1, const axis ax)
    {
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};

        Matrix<double> average{sum(mxb1, (ax==axis::X)? axis::X : axis::Y)};
        machine::broadcast(average, (ax==axis::X)? c1 : r1, Operation::divide);
        return average;
    }

    template<typename T>
    double variance(const Matrix_base<T> &mxb1, double degree = 0)
    {   
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};
        double mean{average(mxb1)};

        Matrix<double> var{mxb1};
        var -= mean;
        machine::broadcast(var, 2, Operation::power);
        double var_out{sum(var)};
        return  var_out / (r1*c1-degree);
    }

    template<typename T>
    Matrix<double> variance(const Matrix_base<T> &mxb1, const axis ax, double degree = 0)
    {   
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};
        Matrix<double> mean{average(mxb1, ax)};
        Matrix<double> var{mxb1};
        var -= mean;
        machine::broadcast(var, 2, Operation::power);
        Matrix<double> var_out {sum(var, ax)};
        machine::broadcast(var_out, (ax == axis::X)? c1-degree : r1-degree, Operation::divide);
        return var_out;
    }

    template<typename T>
    double standard_dev(const Matrix_base<T> &mxb1, double degree = 0)
    {   
        return std::sqrt(variance(mxb1, degree));
    }

    template<typename T>
    Matrix<double> standard_dev(const Matrix_base<T> &mxb1, const axis ax, double degree = 0)
    {   
        Matrix<double> standard_deviation{variance(mxb1, ax, degree)};
        machine::broadcast(standard_deviation, 2.0, Operation::root);
        return standard_deviation;
    }

    template<typename T>
    Matrix<T> normal_distribution(const Matrix_base<T> &mxb1, const axis ax)
    {   
        double small_num {0.001};
        Matrix<double> n_dist{mxb1 - average(mxb1, ax)};

        Matrix<double> variance_squared{
            machine::broadcast(variance(mxb1, ax) + small_num, 2.0, Operation::root)
        };

        return n_dist / variance_squared;
    }
    template<typename T>
    double z_score(const Matrix_base<T> &mxb1, double percentage); //not yet
    

    template<typename T>
    Matrix<T> normal_distribution(const Matrix_base<T> &mxb1)//not yet
    {
    }

    template<typename T>
    Matrix<T> normal_distribution(const Matrix_base<T> &mxb1, const axis ax);

    template<typename T>
    Matrix<T> robust_scale();

    template<typename T>
    void randomize(Matrix_base<T> &mxb1, const T min, const T max){
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};

        for(size_t r{} ; r<r1 ; r++){
            for(size_t c{} ; c<c1 ; c++)
            {   
                if constexpr(std::is_integral_v<T>){
                    std::uniform_int_distribution distrib(min, max);
                    mxb1(r, c) = distrib(generator);
                }
                else
                {
                    std::uniform_real_distribution<T> distrib(min, max);
                    mxb1(r, c) = distrib(generator);
                }
            }
        }
    }

};



