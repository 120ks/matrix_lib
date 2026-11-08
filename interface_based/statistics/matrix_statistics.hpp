#pragma once

#include "non_member_functions.hpp"
#include "matrix_base.hpp"
#include "broadcast.hpp"


template<typename T>
class Matrix;

namespace machine
{   
    template<typename Tmx, typename T>
    T minimum(const Matrix_base<Tmx, T>& mxb1)
    {      
        auto min{mxb1(0,0)};
        for(long long r{} ; r<mxb1.get_rows() ; r++){
            for(long long c{} ; c<mxb1.get_cols ; c++){
                if(mxb1(r, c)<min){min = mxb1(r, c);}
            }
        }
    }

    template<typename Tmx, typename T>
    T maximum(const Matrix_base<Tmx, T>& mxb1)
    {   
        auto max{mxb1(0,0)};
        for(long long r{} ; r<mxb1.get_rows() ; r++){
            for(long long c{} ; c<mxb1.get_cols() ; c++){
                if(mxb1(r, c)>max){max = mxb1(r, c);}
            }
        }
        return max;
    }
    template<typename Tmx, typename T>
    Matrix<T> argmax(const Matrix_base<Tmx, T> &mxb1, const axis ax)
    {   
        auto rows{mxb1.get_rows()};
        auto cols{mxb1.get_cols()};

        if(ax == axis::X){
            Matrix<T> mx{rows, 1};
            for(long long row{} ; row<rows ; row++){
                for(long long col{} ; col<cols ; col++){
                    if(mxb1(row, col) > mx(row, 0)){
                        mx(row, 0) = mxb1(row, col);
                    }
                }
            }
            return mx;
        }
        else{
            if(ax == axis::X){
                Matrix<T> mx{1, cols};
                for(long long row{} ; row<rows ; row++){
                    for(long long col{} ; col<cols ; col++)
                    {
                        if(mxb1(row, col) > mx(col, 0)){
                            mx(0, col) = mxb1(row, col);
                        }
                    }
                }
            return mx;
            }    
        }
    }
    //
    //SUM AND AVARAGE
    //
    template<typename Tmx, typename T>
    double sum(const Matrix_base<Tmx, T> &mxb1)
    {
        Matrix<double> mx{1, 1, 0};
        machine::broadcast(mx, mxb1, Operation::add);
        return mx(0,0);
    }

    template<typename Tmx, typename T>
    Matrix<double> sum(const Matrix_base<Tmx, T> &mxb1, const axis ax)
    {   
        Matrix<double> mx = (ax == axis::X)? Matrix<double>{mxb1.get_rows(), 1} : Matrix<double>{1, mxb1.get_cols()};
        machine::broadcast(mx, mxb1, Operation::add);
        return mx;
    }
    template<typename Tmx, typename T>
    double average(const Matrix_base<Tmx, T> &mxb1)
    {   
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};

        if(r1*c1 == 0){
            throw std::runtime_error("cols or rows are 0 in average");
        }

        double average{sum(mxb1)};
        return average / (r1*c1);
    }
    template<typename Tmx, typename T>
    Matrix<double> average(const Matrix_base<Tmx, T> &mxb1, const axis ax)
    {
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};

        Matrix<double> average{sum(mxb1, (ax==axis::X)? axis::X : axis::Y)};
        machine::broadcast(average, (ax==axis::X)? c1 : r1, Operation::divide);
        return average;
    }
    //
    //VARIANCE AND DEVIATION
    //
    template<typename Tmx, typename T>
    double variance(const Matrix_base<Tmx, T> &mxb1, double degree = 0)
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
    template<typename Tmx, typename T>
    Matrix<double> variance(const Matrix_base<Tmx, T> &mxb1, const axis ax, double degree = 0)
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
    template<typename Tmx, typename T>
    double standard_dev(const Matrix_base<Tmx, T> &mxb1, double degree = 0)
    {   
        return std::sqrt(variance(mxb1, degree));
    }
    template<typename Tmx, typename T>
    Matrix<double> standard_dev(const Matrix_base<Tmx, T> &mxb1, const axis ax, double degree = 0)
    {   
        Matrix<double> standard_deviation{variance(mxb1, ax, degree)};
        machine::broadcast(standard_deviation, 2.0, Operation::root);
        return standard_deviation;
    }
    //
    //NORMALIZATION
    //
    template<typename Tmx, typename T>
    Matrix<T> normal_distribution(const Matrix_base<Tmx, T> &mxb1, const axis ax)
    {   
        double small_num {0.001};
        Matrix<double> n_dist{mxb1 - average(mxb1, ax)};

        Matrix<double> variance_squared{
            machine::broadcast(variance(mxb1, ax) + small_num, 2.0, Operation::root)
        };

        return n_dist / variance_squared;
    }
    template<typename Tmx, typename T>
    double z_score(const Matrix_base<Tmx, T> &mxb1, double percentage);
    

    template<typename Tmx, typename T>
    Matrix<T> normal_distribution(const Matrix_base<Tmx, T> &mxb1)
    {
    }

    template<typename Tmx, typename T>
    Matrix<T> normal_distribution(const Matrix_base<Tmx, T> &mxb1, const axis ax);

    template<typename Tmx, typename T>
    Matrix<T> robust_scale();
    //
    //TRIGONOMETRY
    //
    template<typename Tmx, typename T>
    Matrix<T> cos(const Matrix_base<Tmx, T> &mxb1){
        auto rows{mxb1.get_rows()};
        auto cols{mxb1.get_cols()};
        Matrix<T> mx{mxb1};
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){
                mx(row, col) = std::cos(mxb1(row, col));
            }
        }
    }
    template<typename Tmx, typename T>
    Matrix<T> sin(const Matrix_base<Tmx, T> &mxb1){
        auto rows{mxb1.get_rows()};
        auto cols{mxb1.get_cols()};
        Matrix<T> mx{mxb1};
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){
                mx(row, col) = std::sin(mxb1(row, col));
            }
        }
    }
    template<typename Tmx, typename T>
    Matrix<T> tan(const Matrix_base<Tmx, T> &mxb1){
        auto rows{mxb1.get_rows()};
        auto cols{mxb1.get_cols()};
        Matrix<T> mx{mxb1};
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){
                mx(row, col) = std::tan(mxb1(row, col));
            }
        }
    }
    //
    //RANDOMIZE AND INITIALIZE
    //
    template<typename Tmx, typename T, typename R, typename U>
    void randomize(Matrix_base<Tmx, T> &mxb1, const R min, const U max){
        auto r1{mxb1.get_rows()};
        auto c1{mxb1.get_cols()};

        auto min_v = static_cast<T>(min);
        auto max_v = static_cast<T>(max);
        
        for(long long r{} ; r<r1 ; r++){
            for(long long c{} ; c<c1 ; c++)
            {   
                if constexpr(std::is_integral_v<T>){
                    std::uniform_int_distribution distrib(min_v, max_v);
                    mxb1(r, c) = distrib(generator);
                }
                else
                {
                    std::uniform_real_distribution<T> distrib(min_v, max_v);
                    mxb1(r, c) = distrib(generator);
                }
            }
        }
    }
    template<typename Tmx, typename T>
    void initialize(Matrix_base<Tmx, T> &mxb1);
};



