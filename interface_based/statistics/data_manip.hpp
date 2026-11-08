#pragma once
#include "matrix_statistics.hpp"
#include "Matrix_view.hpp"
#include <functional>


namespace machine
{
    template<typename T>
    class trailing
    {
        private:
            Matrix_view<T> mptr{};
            long long trail{};
            long long rows{};
            long long cols{};
            axis ax{axis::Y};
            bool reverse{};

            trailing(Matrix_view<T> mxv1, long long trail_n, axis xy= axis::Y) 
            : mptr{std::move(mxv1)}
            , trail{}
            , rows{mxv1->get_rows()}
            , cols{mxv1->get_cols()}
            , ax{xy}
            {
                if(trail_n<0){
                    reverse = true;
                }
                trail = static_cast<long long> (std::abs(trail_n));
            }

            trailing& std(double degree = 0);
        
            trailing& avg();
            
            trailing& min();

            trailing& max();

            trailing sum(){
            }

            template<typename Umx, typename U = T>
            trailing<T>& apply(std::function<void(Matrix_base<Umx, U>&, axis)> func)
            {   
                Slice row_slice{0, trail};
                Slice col_slice{0, cols};
                if(ax == axis::X){
                    Slice row_slice{0, rows};
                    Slice col_slice{0, trail};
                    
                    for(long long col{} ; col<cols ; col++){
                        col_slice.begin += col;
                        col_slice.end += col;

                        (*mptr)[trail+col-1] = func((*mptr)(row_slice, col_slice), ax);
                    }
                    return *this;
                
                for(long long row{} ; row<rows ; row++){
                        row_slice.begin += row;
                        row_slice.end += row;

                        (*mptr)(trail + row-1) = func((*mptr)(row_slice, col_slice), ax);
                    }
                return *this;
                }
            }
            //annetaan aina esim avg'ssa slice per kohta, eli esim jos on 1, 2, 3, 4, 5 kahden avulla niin kutsutaan sum[sl{0, 2}] etc, tähän voidaan soveltaa jokin funktio





    };




}
