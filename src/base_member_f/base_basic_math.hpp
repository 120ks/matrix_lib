#pragma once 

#include "non_member_functions.hpp"

namespace machx
{   
    //BASIC MATH OPERATIONS (NON ARITHMETIC)
    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::abs(){
        this->M() < 0 *= -1;
        return static_cast<Tmx&>(*this);
    }

    //LOG EXP
    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::log(){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        auto& mx = static_cast<Tmx&>(*this);
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){
                mx(row, col) = std::log(mx(row, col));
            }
        }
        return static_cast<Tmx&>(*this);
    }
    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::exp(){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix<float> exp_arr{rows, cols, machx::E};
        exp_arr.power(*this);
        (*this) = exp_arr;

        return static_cast<Tmx&>(*this);
    }

    //TRIGONOMETRY
    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::sin(){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        auto& mx = static_cast<Tmx&>(*this);
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){
                mx(row, col) = std::sin(mx(row, col));
            }
        }
        return static_cast<Tmx&>(*this);    
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::cos(){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        auto& mx = static_cast<Tmx&>(*this);
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){
                mx(row, col) = std::cos(mx(row, col));
            }
        }
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::tan(){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        auto& mx = static_cast<Tmx&>(*this);
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){
                mx(row, col) = std::tan(mx(row, col));
            }
        }
        return static_cast<Tmx&>(*this);
    }
    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::tanh(){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        auto& mx = static_cast<Tmx&>(*this);
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){
                mx(row, col) = std::tanh(mx(row, col));
            }
        }
        return static_cast<Tmx&>(*this);
    }

     //POWER AND ROOT
    template<typename Tmx, typename T>
    template<typename R>
    requires (std::is_arithmetic_v<R>)
    Tmx& Matrix_base<Tmx, T>::power(const R& val){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix_base<Tmx, T>& mx = *this;

        for(long long row{0} ; row<rows ; row++){  
            for(long long col{0} ; col<cols ; col++){
                mx(row, col) = std::pow(static_cast<double>(mx(row, col)), val);
            }
        }
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    Tmx& Matrix_base<Tmx, T>::power(const Matrix_base<Rmx, R>& mx2){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        Matrix_base<Tmx, T>& mx = *this;

        auto mx2_rows = mx2.get_rows();
        auto mx2_cols = mx2.get_cols();

        auto max_rows = std::max(rows, mx2_rows);
        auto max_cols = std::max(cols, mx2_cols);

        long long mx1_row{}, mx2_row{};
        long long mx1_col{}, mx2_col{};

        for(long long row{0} ; row<max_rows ; row++){  
            mx1_row = row % rows;
            mx2_row = row % mx2_rows; 
            for(long long col{0} ; col<max_cols ; col++){
                mx1_col = col % cols;
                mx2_col = col % mx2_cols;
                mx(mx1_row, mx1_col) = std::pow(static_cast<double>(mx(mx1_row, mx1_col)), mx2(mx2_row, mx2_col));
            }
        }
        return static_cast<Tmx&>(*this);
    }   

    template<typename Tmx, typename T>
    template<typename R>
    requires (std::is_arithmetic_v<R>)
    Tmx& Matrix_base<Tmx, T>::root(const R& val){
        return this->power(1.0 / val);
    }

    template<typename Tmx, typename T>
    template<typename Rmx, typename R>
    Tmx& Matrix_base<Tmx, T>::root(const Matrix_base<Rmx, R>& mxb){
        auto mxb_rows = mxb.get_rows();
        auto mxb_cols = mxb.get_cols();
        Matrix<double> ones_mx{mxb_rows, mxb_cols, 1};
        ones_mx /= mxb;
        return this->power(ones_mx);
    }
}
