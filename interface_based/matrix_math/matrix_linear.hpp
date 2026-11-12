#pragma once
#include "matrix_statistics.hpp"
#include "broadcast.hpp"

namespace machx
{   
    template<typename T>
    Matrix<T> matmul(const Matrix<T> &mx1, const Matrix<T> &mx2)
    {       
        
        auto rows_1{mx1.get_rows()};
        auto cols_1{mx1.get_cols()};

        auto rows_2{mx2.get_rows()};
        auto cols_2{mx2.get_cols()};
        
        if(cols_1 != rows_2)
        {
            throw dimension_mismatch(mx1, mx2, "matrix_multiplication (matmul), mx1 cols must match mx2 rows");
        }
        Matrix<T> mx3{rows_1, cols_2};
        
        auto& mx1_matrix = mx1.get_matrix();
        auto& mx2_matrix = mx2.get_matrix();
        auto& mx3_matrix = mx3.get_matrix();
        auto cols_3 = mx3.get_cols();

        for(long long r{} ; r<rows_1 ; r++){
            for(long long k{} ; k<cols_1 ; k++){

                T mx1_val = mx1_matrix[r*cols_1 + k];

                for(long long c{} ; c<cols_2 ; c++){
                    mx3_matrix[r*cols_3 + c] +=  mx1_val * mx2_matrix[k*cols_2 + c];
                }
                
            }
        }
        return mx3;
    }

    template<typename T, typename R>
    auto matmul(Matrix<T> mx1, Matrix<R> mx2) //copy for better cache when view / slice
    {   
        auto rows_1{mx1.get_rows()};
        auto cols_1{mx1.get_cols()};

        auto rows_2{mx2.get_rows()};
        auto cols_2{mx2.get_cols()};
        

        if(cols_1 != rows_2)
        {
            throw dimension_mismatch(mx1, mx2, "matrix_multiplication (matmul), mx1 cols must match mx2 rows");
        }

        Matrix<std::common_type_t<T, R>> mx3{rows_1, cols_2};

        auto& mx1_matrix = mx1.get_matrix();
        auto& mx2_matrix = mx2.get_matrix();
        auto& mx3_matrix = mx3.get_matrix();
        auto cols_3 = mx3.get_cols();

        for(long long r{} ; r<rows_1 ; r++){
            for(long long k{} ; k<cols_1 ; k++){

                T mx1_val = mx1_matrix[r*cols_1 + k];

                for(long long c{} ; c<cols_2 ; c++){
                    mx3_matrix[r*cols_3 + c] +=  mx1_val * mx2_matrix[k*cols_2 + c];
                }
                
            }
        }
        return mx3;
    }
    template<typename Tmx, typename T>
    auto manhattan_distance(const Matrix_base<Tmx, T> &mxb1){
        auto distances = machx::abs(mxb1.copy());
        return sum(distances);
    }

    template<typename Tmx, typename T, typename Rmx, typename R>
    auto manhattan_distance(const Matrix_base<Tmx, T> &mxb1, const Matrix_base<Rmx, R> &mxb2){
        quick_throw::throw_if_dimension_mismatch(mxb1, mxb2, "manhattan distance");
        auto distances = machx::abs(mxb1-mxb2);
        return sum(distances);
    }

    template<typename Tmx, typename T>
    auto euclidean_distance(const Matrix_base<Tmx, T> &mxb1)
    {   
        auto rows_1{mxb1.get_rows()};
        auto cols_1{mxb1.get_cols()};

        auto distances = power(mxb1, 2.0);
        return std::sqrt(sum(distances));
    }

    template<typename Tmx, typename T, typename Rmx, typename R>
    auto euclidean_distance(const Matrix_base<Tmx, T> &mxb1, const Matrix_base<Rmx, R> &mxb2)
    {   
        auto rows_1{mxb1.get_rows()};
        auto cols_1{mxb1.get_cols()};

        auto rows_2{mxb2.get_rows()};
        auto cols_2{mxb2.get_cols()};

        quick_throw::throw_if_dimension_mismatch(mxb1, mxb2, "euclidean distance");

        auto distances = power(mxb1 - mxb2, 2);
        return std::sqrt(sum(distances));
    }

    template<typename Tmx, typename T, typename Rmx, typename R>
    auto dot_product(const Matrix_base<Tmx, T> &mxb1, const Matrix_base<Rmx, R> &mxb2){
        return sum(mxb1 * mxb2);
    }
    template<typename Tmx, typename T, typename Rmx, typename R>
    auto cosine_similarity(const Matrix_base<Tmx, T> &mxb1, const Matrix_base<Rmx, R> &mxb2){
        return dot_product(mxb1, mxb2) / (euclidean_distance(mxb1) * euclidean_distance(mxb2));
    }
    template<typename Tmx, typename T, typename Rmx, typename R>
    auto cosine_distance(const Matrix_base<Tmx, T> &mxb1, const Matrix_base<Rmx, R> &mxb2){
        return 1.0 - cosine_similarity(mxb1, mxb2);
    }

    template<typename Tmx, typename T>
    Matrix<T> transpose(const Matrix_base<Tmx, T> &mxb1){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();

        Matrix<T> transposed{cols, rows};
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){
                transposed(col, row) = mxb1(row, col);
            }
        }
        return transposed;
    }
    
}
