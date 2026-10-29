#pragma once
#include "matrix_statistics.hpp"

namespace machine
{   
    template<typename T>
    Matrix<T> dot(const Matrix<T> &mx1, const Matrix<T> &mx2)
    {   
        auto rows_1{mx1.get_rows()};
        auto cols_1{mx1.get_cols()};

        auto rows_2{mx2.get_rows()};
        auto cols_2{mx2.get_cols()};
        
        if(cols_1 != rows_2)
        {
            throw dimension_mismatch(mx1, mx2, "dot product, mx1 cols must match mx2 rows");
        }
        Matrix<T> mx3{rows_1, cols_2};
        
        auto& mx1_matrix = mx1.get_matrix();
        auto& mx2_matrix = mx2.get_matrix();
        auto& mx3_matrix = mx3.get_matrix();
        auto cols_3 = mx3.get_cols();

        for(size_t r{} ; r<rows_1 ; r++){
            for(size_t k{} ; k<cols_1 ; k++){

                T mx1_val = mx1_matrix[r*cols_1 + k];

                for(size_t c{} ; c<cols_2 ; c++){
                    mx3_matrix[r*cols_3 + c] +=  mx1_val * mx2_matrix[k*cols_2 + c];
                }
                
            }
        }
        return mx3;
    }

    template<typename T, typename R>
    auto dot(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
    {   
        auto rows_1{mxb1.get_rows()};
        auto cols_1{mxb1.get_cols()};

        auto rows_2{mxb1.get_rows()};
        auto cols_2{mxb2.get_cols()};

        //make more cache friendly
        Matrix<T> mx1{mxb1};
        Matrix<T> mx2{mxb2};

        if(cols_1 != rows_2)
        {
            throw dimension_mismatch(mxb1, mxb2, "dot product, mxb1 cols must match mxb2 rows");
        }
        auto common_rc{mxb2.get_rows()};

        Matrix<std::common_type_t<T, R>> mx3{rows_1, cols_2};

        auto& mx1_matrix = mx1.get_matrix();
        auto& mx2_matrix = mx2.get_matrix();
        auto& mx3_matrix = mx3.get_matrix();
        auto cols_3 = mx3.get_cols();

        for(size_t r{} ; r<rows_1 ; r++){
            for(size_t k{} ; k<cols_1 ; k++){

                T mx1_val = mx1_matrix[r*cols_1 + k];

                for(size_t c{} ; c<cols_2 ; c++){
                    mx3_matrix[r*cols_3 + c] +=  mx1_val * mx2_matrix[k*cols_2 + c];
                }
                
            }
        }
        return mx3;
    }

    template<typename T, typename R>
    double euclidean_distance(const Matrix_base<T> &point1, const Matrix_base<R> &point2)
    {   
        auto rows_1{point1.get_rows()};
        auto cols_1{point1.get_cols()};

        auto rows_2{point2.get_rows()};
        auto cols_2{point2.get_cols()};

        if(!machine::same_dims(point1, point2)){
            throw dimension_mismatch(point1, point2, "euclidean distance");
        }

        auto distance = machine::broadcast(point1-point2, 2, Operation::power);
        return std::sqrt(machine::sum(distance));
    }
}
