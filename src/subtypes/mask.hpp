#pragma once
#include "matrix_view.hpp"

//
//CREATING BOOLEANS
//
namespace machx
{
    template<typename Tmx, typename T, typename R>
    requires (std::is_arithmetic_v<R>)
    Matrix<bool> make_mask(const Matrix_base<Tmx, T> &mxb1, const Condition condition, const R &threshold)
    {   
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();
        Matrix<bool> mask{rows, cols, false};
        auto N_threshold = threshold;

        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++)
            {
                const auto &current_element = (mxb1(row, col));
                switch(condition)
                {   
                case(Condition::is_true):   mask(row, col) = true; break;
                case(Condition::is_false):  mask(row, col) = false; break;
                case(Condition::over):      mask(row, col) = (current_element >  N_threshold); break;
                case(Condition::over_or):   mask(row, col) = (current_element >= N_threshold); break;
                case(Condition::under):     mask(row, col) = (current_element <  N_threshold); break;
                case(Condition::under_or):  mask(row, col) = (current_element <= N_threshold); break;
                case(Condition::is) :       mask(row, col) = (current_element == N_threshold); break;
                case(Condition::is_not):    mask(row, col) = (current_element != N_threshold); break;
                }
            }
        }
        return mask;
    }
    template<typename Tmx, typename T, typename Rmx, typename R>
    Matrix<bool> make_mask(const Matrix_base<Tmx, T> &mxb1, const Condition condition, const Matrix_base<Rmx, R> &mxb2)
    {   

        quick_throw::throw_if_dimension_mismatch(mxb1, mxb2, "in make_mask()");
        quick_throw::throw_if_no_data(mxb1);
        quick_throw::throw_if_no_data(mxb2);

        long long r1{mxb1.get_rows()};
        long long c1{mxb1.get_cols()};
        Matrix<bool> mask{r1, c1, false};

        for(long long r{} ; r<r1 ; r++){
            for(long long c{} ; c<c1 ; c++){

                const auto& mxb1_current = mxb1(r, c);
                const auto& mxb2_current = mxb2(r, c);

                switch(condition)
                {   
                case(Condition::is_true):   mask(r, c) = true; break;
                case(Condition::is_false):  mask(r, c) = false; break;
                case(Condition::over):      mask(r, c) = (mxb1_current > mxb2_current); break;
                case(Condition::over_or):   mask(r, c) = (mxb1_current >= mxb2_current); break;
                case(Condition::under):     mask(r, c) = (mxb1_current < mxb2_current); break;
                case(Condition::under_or):  mask(r, c) = (mxb1_current <= mxb2_current); break;
                case(Condition::is) :       mask(r, c) = (mxb1_current == mxb2_current); break;
                case(Condition::is_not):    mask(r, c) = (mxb1_current != mxb2_current); break;
                }
            }
        }
        return mask;
    }
}
