#pragma once
#include "friend_operators.hpp"


namespace mx_math
{
    template<typename T, typename R>
    auto dot(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
    {
        assert(mxb1.get_cols() == mxb2.get_rows());

        auto rows_1{mxb1.get_rows()};
        auto cols_2{mxb2.get_cols()};
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
}
