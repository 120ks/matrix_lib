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
            Matrix_view<T>* mptr{};
            size_t trail{};
            size_t rows{};
            size_t cols{};
            axis ax{axis::Y};
            bool reverse{};

            trailing(Matrix_view<T> mxv1, long long trail_n, axis xy= axis::Y) 
            : mptr{&mxv1}
            , trail{}
            , rows{mxv1->get_rows()}
            , cols{mxv1->get_cols()}
            , ax{xy}
            {
                if(trail_n<0){
                    reverse = true;
                }
                trail = static_cast<size_t> (std::abs(trail_n));
            }

            trailing& std(double degree = 0);
        
            trailing& avg();
            
            trailing& min();

            trailing& max();

            trailing sum(){
            }

            template<typename U = T>
            trailing<T>& apply(std::function<void(Matrix_base<U>&, axis)> func)
            {   
                Slice row_slice{0, trail};
                Slice col_slice{0, cols};
                if(ax == axis::X){
                    Slice row_slice{0, rows};
                    Slice col_slice{0, trail};
                    
                    for(size_t col{} ; col<cols ; col++){
                        col_slice.begin += col;
                        col_slice.end += col;

                        (*mptr)[trail+col-1] = func((*mptr)(row_slice, col_slice), ax);
                    }
                    return *this;
                
                for(size_t row{} ; row<rows ; row++){
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
