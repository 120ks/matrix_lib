#pragma once
#include "matrix_statistics.hpp"
#include "Matrix_view.hpp"
#include <functional>


namespace machx
{   

    template<typename T>
    class trailing
    {   
        //stacks flags with syntax eg. matrix.cols_at(2).trailing(10).std().offset(1).cut(n = trail)
        //when destroyed at end of expression, performs full operations, saves functions to list to be used at the end.
        private:
            Matrix_view<T> mxv{};
            long long trailing_n{};
            axis ax{axis::Y};
            long long shift_by{0};
            std::function<Matrix<T>(Matrix_view<T>, axis)> func{};

            trailing(Matrix_view<T> mxv1, long long trail_n, axis xy= axis::Y) 
            : mxv{std::move(mxv1)}
            , trailing_n{trail_n}
            , ax{xy}
            {
            }
            ~trailing(){
                
            }
            
            trailing<T>& shift(long long offset){
                shift_by = offset;
                return *this;
            }

            trailing<T>& axis(axis xy){
                ax = xy;
                return *this;
            }

            trailing& std(double degree = 0);
        
            trailing& avg();
            
            trailing& min();

            trailing& max();

            trailing& sum(){
            }

            template<typename Umx, typename U = T>
            trailing<T>& apply()
            {   
                auto rows = mxv.get_rows();
                auto cols = mxv.get_cols();

                Matrix<T> copy = mxv.copy();

                long long start = 0;
                if(ax == axis::Y){
                    for(auto row{trailing_n} ; row < rows ; row++){
                        mxv(Slice{row}) = func(copy(Slice{start, row}), ax);
                        start++;
                    }
                }
                else{
                    for(auto col{trailing_n} ; col < cols ; col++){
                        mxv[Slice{col}] = func(copy(Slice{start, col}), ax);
                        start++;
                    }
                }
                return *this;
            }
    };
}
