#pragma once

#include <thread>
#include <functional>
#include "matrix_statistics.hpp"



namespace machx::threaded
{
    template<typename T>
    Matrix<T> tr_matmul(const Matrix<T> &mx1, const Matrix<T> &mx2, long long threads = 3)
    {   
        auto rows_1{mx1.get_rows()};
        auto cols_1{mx1.get_cols()};

        auto rows_2{mx2.get_rows()};
        auto cols_2{mx2.get_cols()};

        if(threads == 0){threads = 1;}
        if(threads > rows_1){threads = rows_1;}
        if(rows_1 == 0) {return Matrix<T> {0, cols_2, 0};}
        
        if(cols_1 != rows_2)
        {
            throw dimension_mismatch(mx1, mx2, "dot product threaded, mx1 cols must match mx2 rows");
        }
        Matrix<T> mx3{rows_1, cols_2, 0};
        
        auto& mx1_matrix = mx1.get_matrix();
        auto& mx2_matrix = mx2.get_matrix();
        auto& mx3_matrix = mx3.get_matrix();
        auto cols_3 = mx3.get_cols();

        auto matmul_f = [ = ,&mx1_matrix, &mx2_matrix, &mx3_matrix](long long row_start, long long row_end) -> void
        {
            for(long long r{row_start} ; r<row_end ; r++)
            {
                for(long long k{} ; k<cols_1 ; k++){

                    T mx1_val = mx1_matrix[r*cols_1 + k];

                    for(long long c{} ; c<cols_2 ; c++){
                        mx3_matrix[r*cols_3 + c] +=  mx1_val * mx2_matrix[k*cols_2 + c];
                    }
                }
            }
        };

        long long per_thread = (rows_1 / threads);
        long long remainder = rows_1 % threads;

        assert(rows_1 == per_thread*threads + remainder);

        long long start{0};
        std::vector<std::thread> thread_vector;
        thread_vector.reserve(threads);

        for(long long t{0} ; t<threads ; t++)
        {   
            long long end = start + per_thread + ((t<remainder) ? 1 : 0); 
            if (start >= end){
                break;
            }
            thread_vector.emplace_back(matmul_f, start, end);
            start = end;
        }
        
        for(auto &thread : thread_vector)
        {   
            if(thread.joinable())
            {
                thread.join();
            }
        }
        return mx3;
    }

    template<typename Tmx, typename T, typename Rmx, typename R>
    Matrix<T> tr_matmul(Matrix_base<Tmx, T> &mxb1, const Matrix_base<Rmx, R> &mxb2)
    {   
        using common_type = std::common_type_t<T, R>;
        return tr_matmul(Matrix<common_type>(mxb1), Matrix<common_type>(mxb2));
    }


}
