#pragma once
#include "matrix_statistics.hpp"

namespace mx_math
{
    template<typename T, typename R>
    auto dot(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
    {   
        auto rows_1{mxb1.get_rows()};
        auto cols_1{mxb1.get_cols()};

        auto rows_2{mxb1.get_rows()};
        auto cols_2{mxb2.get_cols()};
        
        if(cols_1 != rows_2)
        {
            throw dimension_mismatch(std::format("mxb1({}x{}) cols must match mxb2({}x{}) rows", rows_1, cols_1, rows_2, cols_2));
        }
        auto common_rc{mxb2.get_rows()};

        Matrix<std::common_type_t<T, R>> mx{rows_1, cols_2};
        for(size_t r{} ; r<rows_1 ; r++){
            for(size_t c{} ; c<cols_2 ; c++){
                T dot_sum{};
                for(size_t k{} ; k<common_rc ; k++){
                    dot_sum += mxb1(r, k) * mxb2(k, c);
                }
                mx(r, c) = dot_sum;
            }
        }
        return mx;
    }
    template<typename T, typename R>
    double euclidean_distance(const Matrix_base<T> &point1, const Matrix_base<R> &point2)
    {   
        auto rows_1{point1.get_rows()};
        auto cols_1{point1.get_cols()};

        auto rows_2{point2.get_rows()};
        auto cols_2{point2.get_cols()};

        if(!matrix_math::same_dims(point1, point2)){
            throw dimension_mismatch(std::format("point1({}x{}) point2({}x{}) not same", rows_1, cols_1, rows_2, cols_2));
        }

        auto distance = method::broadcast(point1-point2, 2, Operation::power);
        return std::sqrt(mx_math::sum(distance));
    }
}
