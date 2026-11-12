#pragma once

#include "non_member_functions.hpp"
#include "matrix_base.hpp"
#include "broadcast.hpp"
#include <cmath>


template<typename T>
class Matrix;


//
//BASICS
//
namespace machx
{   
    template<typename Tmx, typename T>
    T sum(const Matrix_base<Tmx, T> &mxb1){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();

        T sum_val = T{};
        for(long long row{} ; row<rows; row++){
            for(long long col{} ; col<cols ; col++){
                sum_val += mxb1(row, col);
            }
        }
        return sum_val;
    }
    template<typename Tmx, typename T>
    Matrix<T> sum(const Matrix_base<Tmx, T> &mxb1, const axis ax){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();

        if(ax == axis::X){
            Matrix<T> sum_mx{1, cols};
            return sum_mx + mxb1;
        }
        else{
            Matrix<T> sum_mx{rows, 1};
            return sum_mx + mxb1;
        }
    }

    template<typename Tmx, typename T>
    double mean(const Matrix_base<Tmx, T> &mxb1){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();
        return machx::sum(mxb1) / (rows*cols);
    }

    template<typename Tmx, typename T>
    auto mean(const Matrix_base<Tmx, T> &mxb1, axis ax){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();
        Matrix<T> sum_mx = machx::sum(mxb1, ax);
        return sum_mx / (sum_mx.get_cols() * sum_mx.get_rows());
    }
}
//
//deviations
//
namespace machx
{   
    template<typename Tmx, typename T>
    double variance(const Matrix_base<Tmx, T> &mxb1, double degree = 0){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();

        double mean = machx::mean(mxb1);
        auto variance_mx{mxb1 - mean};
        machx::power(variance_mx, 2);
        double sum = machx::sum(variance_mx);

        return sum / (rows * cols + degree);
    }

    template<typename Tmx, typename T>
    Matrix<double> variance(const Matrix_base<Tmx, T> &mxb1, axis ax, double degree = 0){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();

        auto mean = machx::mean(mxb1, ax);
        auto variance_mx{mxb1 - mean};
        machx::power(variance_mx, 2);
        auto sum = machx::sum(variance_mx, ax);
        
        return sum / ((ax == axis::X) ? cols : rows);
    }

    template<typename Tmx, typename T>
    double standard_deviation(const Matrix_base<Tmx, T> &mxb1, double degree = 0){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();
        double variance = machx::variance(mxb1, degree);
        return std::sqrt(variance);
    }

    template<typename Tmx, typename T>
    Matrix<double> standard_deviation(const Matrix_base<Tmx, T> &mxb1, axis ax, double degree = 0){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();
        auto variance = machx::variance(mxb1, ax, degree);
        machx::root(variance, 2.0);
        return variance;
    }
}

//
//MIN, MAX, ABS
//
namespace machx
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
        
    }
    template<typename Tmx, typename T>
    requires(std::is_arithmetic_v<T>)
    Tmx& abs(Matrix_base<Tmx, T> &mxb1){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();
        for(long long row{} ; row<rows ; row++){
            for(long long col {} ; col<cols ; col++){
                mxb1(row, col) = std::abs(mxb1(row, col));
            }
        }
        return static_cast<Tmx&>(mxb1);
    }

    template<typename Tmx, typename T>
    requires(std::is_arithmetic_v<T>)
    Matrix<T> abs(Matrix_base<Tmx, T> &&mxb1){
        abs(mxb1);
        return {static_cast<Tmx&>(mxb1)};
    }

    template<typename Tmx, typename T>
    requires(std::is_arithmetic_v<T>)
    Tmx& log(Matrix_base<Tmx, T> &mxb1){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();
        for(long long row{} ; row<rows ; row++){
            for(long long col {} ; col<cols ; col++){
                mxb1(row, col) = std::log(mxb1(row, col));
            }
        }
        return static_cast<Tmx&>(mxb1);
    }
}
//
//SCALES AND NORMALIZERS
//
namespace machx
{
    template<typename Tmx, typename T>
    Tmx& z_scores(Matrix_base<Tmx, T> &mxb1, double degree = 0){
        auto mean = machx::mean(mxb1);
        auto std = machx::standard_deviation(mxb1, degree);
        (mxb1 -= mean) /= std;
        return static_cast<Tmx&>(mxb1);
    }

    template<typename Tmx, typename T>
    Tmx& robust_scale(Matrix_base<Tmx, T>& mxb1, double percentile1 = 0.15, double percentile2 = 0.85){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();

        Matrix<T> copy = mxb1.copy();
        std::sort(copy.begin(), copy.end());
        auto median = copy(rows/2, cols/2);
        auto Q1 = copy(static_cast<long long>(rows*percentile1), static_cast<long long>(cols*percentile1));
        auto Q3 = copy(static_cast<long long>(rows*percentile2), static_cast<long long>(cols*percentile2));
        auto IQR = Q3-Q1;
        (mxb1 -= median) /= IQR;
        return static_cast<Tmx&>(mxb1);
    }

    template<typename Tmx, typename T>
    Tmx& robust_z(Matrix_base<Tmx, T>& mxb1, double percentile1 = 0.15, double percentile2 = 0.85, double degree = 0){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();
        Matrix<T> copy = mxb1.copy();
        std::sort(copy.begin(), copy.end());
        Slice row_slice {static_cast<long long>(rows * percentile1), static_cast<long long>(rows * percentile2)};
        Slice col_slice {static_cast<long long>(cols * percentile1), static_cast<long long>(cols * percentile2)};

        auto mean = machx::mean(mxb1(row_slice, col_slice));
        auto std = machx::standard_deviation(mxb1(row_slice, col_slice), degree);
        (mxb1 -= mean) /= std;
        return static_cast<Tmx&>(mxb1);

    }

    template<typename Tmx, typename T>
    Tmx& batch_normalization(Matrix_base<Tmx, T> &mxb1, double degree = 0){
        auto means = machx::mean(mxb1, axis::Y);
        auto std = machx::standard_deviation(mxb1, axis::Y, degree);
        (mxb1 -= mean) /= std;
        return static_cast<Tmx&>(mxb1);
    }

    template<typename Tmx, typename T>
    Tmx& layer_normalization(Matrix_base<Tmx, T> &mxb1, double degree = 0){
        auto means = machx::mean(mxb1, axis::X);
        auto std = machx::standard_deviation(mxb1, axis::X, degree);
        (mxb1 -=mean) /= std;
        return static_cast<Tmx&>(mxb1);
    }

    template<typename Tmx, typename T>
    Tmx& group_normalization(Matrix_base<Tmx, T> &mxb1, long long groups, double degree = 0){
        auto cols = mxb1.get_cols();
        if(cols % groups != 0){
            throw std::runtime_error("cols must be divisible by group number");
        }
        long long group_members = cols / groups;
        long long start = 0;
        for(long long g{} ; g<groups ; g++){
            auto end = start+group_members;
            auto means = machx::mean(mxb1[Slice{start, end}], axis::X);
            auto std = machx::standard_deviation(mxb1, axis::X, degree)
            (mxb1[Slice{start, end}] -= means) /= std;
        }
        return static_cast<Tmx&>(mxb1);
    }
    template<typename Tmx, typename T>
    Tmx& rank(Matrix_base<Tmx, T> &mxb1, bool ascending = true, bool duplicates_share = false){
        auto rows{mxb1.get_rows()};
        auto cols{mxb1.get_cols()};
        long long start{};
        auto rank_f = [&](auto self, machx::Mask_view<T> mv) -> void {
            if(mv.get_cols() == 0){
                return;
            }
            if(mv.get_cols() == 1){
                mv(0) = start++;
                return;
            }
            auto pivot = mv(0);
            if(ascending){
                auto left = mv.M() < pivot;
                auto right = mv.M() > pivot;
                auto middle = mv[mv == pivot];
                self(self, left);
                if(duplicates_share){
                    middle = start++;
                }
                else{
                    for(long long col{} ; col<middle.get_cols() ; col++){
                        middle(col) = start++;
                    }
                }
                self(self, right);
                return;
            }
            else{
                auto left = mv.M() > pivot;
                auto right = mv.M() < pivot;
                auto middle = mv[mv == pivot];
                self(self, left);
                if(duplicates_share){
                    middle = start++;
                }
                else{
                    for(long long col{} ; col<middle.get_cols() ; col++){
                        middle(col) = start++;
                    }
                }
                self(self, right);
                return;
            }
        };
        Matrix<bool> mask{rows, cols, true};
        rank_f(rank_f, mxb1[mask]);

        return static_cast<Tmx&>(mxb1);
    }

    template<typename Tmx, typename T>
    Tmx& quantile(Matrix_base<Tmx, T> &mxb1, bool ascending = true){
        auto rows{mxb1.get_rows()};
        auto cols{mxb1.get_cols()};
        machx::rank(mxb1, ascending);
        mxb1 /= ((rows*cols)-1);
        return static_cast<Tmx&>(mxb1);
    }

    template<typename Tmx, typename T>
    Tmx& force_gaussian(Matrix_base<Tmx, T> &mxb1, bool ascending = true){
        auto rows{mxb1.get_rows()};
        auto cols{mxb1.get_cols()};
        (machx::rank(mxb1, ascending) -= 0.625) /= (rows*cols + 0.25);
        Matrix<T> ones_arr{rows, cols, 1};
        mxb1 /= (ones_arr -= machx::abs(mxb1));
        machx::log(mxb1);
        mxb1 /= 1.702;
        
        return static_cast<Tmx&>(mxb1);
    }   

}
namespace machx
{

    
    //
    //TRIGONOMETRY
    //
    template<typename Tmx, typename T>
    Tmx& cos(const Matrix_base<Tmx, T> &mxb1){
        auto rows{mxb1.get_rows()};
        auto cols{mxb1.get_cols()};
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){
                mxb1(row, col) = std::cos(mxb1(row, col));
            }
        }
    }
    template<typename Tmx, typename T>
    Tmx& sin(const Matrix_base<Tmx, T> &mxb1){
        auto rows{mxb1.get_rows()};
        auto cols{mxb1.get_cols()};
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){
                mxb1(row, col) = std::sin(mxb1(row, col));
            }
        }
    }
    template<typename Tmx, typename T>
    Tmx& tan(const Matrix_base<Tmx, T> &mxb1){
        auto rows{mxb1.get_rows()};
        auto cols{mxb1.get_cols()};
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){
                mxb1(row, col) = std::tan(mxb1(row, col));
            }
        }
    }
    //
    //RANDOMIZE AND INITIALIZE
    //
    template<typename Tmx, typename T, typename R, typename U>
    Tmx& randomize(Matrix_base<Tmx, T> &mxb1, const R min, const U max){
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
        return static_cast<Tmx&>(mxb1);
    }



    template<typename Tmx, typename T>
    void initialize(Matrix_base<Tmx, T> &mxb1);
}



