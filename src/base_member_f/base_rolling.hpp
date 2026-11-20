#pragma once


#include "non_member_functions.hpp"


namespace machx
{   
    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::rolling(long long trail){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        auto copy_mx = this->copy();


        long long row_start = (trail<0) ? 0 : trail;
        long long row_end = (trail<0) ? rows-trail : rows;

        long long start{};
        long long end{trail+1};
        for(long long row{row_start} ; row < row_end ; row++){
            this->rows_at(row) = copy_mx.rows_at(start++, end++).col_mean();
        }
        this->rows_at(0, trail) = std::numeric_limits<T>::quiet_NaN();
        return static_cast<Tmx&>(*this);
    }
}

