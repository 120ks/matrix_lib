#pragma once
#include "matrix_statistics.hpp"

namespace machine
{   

    template<typename T>
    auto dot(const Matrix_base<T> &mxb1, const Matrix_base<T> &mxb2)
    {   
        auto rows_1{mxb1.get_rows()};
        auto cols_1{mxb1.get_cols()};

        auto rows_2{mxb1.get_rows()};
        auto cols_2{mxb2.get_cols()};
        
        if(cols_1 != rows_2)
        {
            throw dimension_mismatch(mxb1, mxb2, "dot product, mxb1 cols must match mxb2 rows");
        }
        auto common_rc{mxb2.get_rows()};

        Matrix<T> mx{rows_1, cols_2};

        if(same_dims(mxb1, mxb2)){

            //make faster
        }

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
    auto dot(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
    {   
        auto rows_1{mxb1.get_rows()};
        auto cols_1{mxb1.get_cols()};

        auto rows_2{mxb1.get_rows()};
        auto cols_2{mxb2.get_cols()};
        
        if(cols_1 != rows_2)
        {
            throw dimension_mismatch(mxb1, mxb2, "dot product, mxb1 cols must match mxb2 rows");
        }
        auto common_rc{mxb2.get_rows()};

        Matrix<std::common_type_t<T, R>> mx{rows_1, cols_2};

        if(same_dims(mxb1, mxb2)){

            //make faster
        }

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

        if(!machine::same_dims(point1, point2)){
            throw dimension_mismatch(point1, point2, "euclidean distance");
        }

        auto distance = machine::broadcast(point1-point2, 2, Operation::power);
        return std::sqrt(machine::sum(distance));
    }
}
