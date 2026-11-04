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
            Matrix_base<T>* mptr{};
            size_t trail{};
            size_t rows{};
            size_t cols{};
            axis ax{axis::Y};
            bool reverse{};

            trailing(Matrix_base<T> &mxb1, size_t trailing_n, axis xy= axis::Y) 
            : mptr{&mxb1}
            , trail{trailing_n}
            , rows{mxb1->get_rows()}
            , cols{mxb1->get_cols()}
            , ax{xy}
            {}

            trailing& std(double degree = 0);
        
            trailing& avg();
            
            trailing& min();

            trailing& max();

            trailing sum(){
            }

            
            trailing apply()
            {   
                

                Slice row_slice{0, trail};
                Slice col_slice{0, cols};
                if(ax == axis::X){
                    Slice row_slice{0, rows};
                    Slice col_slice{0, trail};
                }
                


            }
            
            
            //annetaan aina esim avg'ssa slice per kohta, eli esim jos on 1, 2, 3, 4, 5 kahden avulla niin kutsutaan sum[sl{0, 2}] etc, tähän voidaan soveltaa jokin funktio





    };




}
