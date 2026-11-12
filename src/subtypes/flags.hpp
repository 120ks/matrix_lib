#pragma once
#include "mask_view.hpp"

//
//MASK FLAG COMPARISON OPERATORS
//
namespace machx
{


    template<typename Tmx, typename T, typename Rmx, typename R>
    machx::array<T*> make_ptr_arr(Matrix_base<Tmx, T> &mxb1, const Condition condition, const Matrix_base<Rmx, R> &other){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();

        machx::array<T*> p_arr{};
        p_arr.reserve(rows*cols);

        auto over = [&]() -> void {
            for(long long row{} ; row < rows ; row++){
                for(long long col{} ; col < cols ; col++){
                    if(mxb1(row, col) > other(row, col)){
                        p_arr.push_back(&mxb1(row, col));
                    }
                }
            }
        };
        auto under = [&]() -> void {
            for(long long row{} ; row < rows ; row++){
                for(long long col{} ; col < cols ; col++){
                    if(mxb1(row, col) < other(row, col)){
                        p_arr.push_back(&mxb1(row, col));
                    }
                }
            }
        };
        auto over_or = [&]() -> void {
            for(long long row{} ; row < rows ; row++){
                for(long long col{} ; col < cols ; col++){
                    if(mxb1(row, col) >= other(row, col)){
                        p_arr.push_back(&mxb1(row, col));
                    }
                }
            }
        };
        auto under_or = [&]() -> void {
            for(long long row{} ; row < rows ; row++){
                for(long long col{} ; col < cols ; col++){
                    if(mxb1(row, col) <= other(row, col)){
                        p_arr.push_back(&mxb1(row, col));
                    }
                }
            }
        };
        auto is = [&]() -> void {
            for(long long row{} ; row < rows ; row++){
                for(long long col{} ; col < cols ; col++){
                    if(mxb1(row, col) == other(row, col)){
                        p_arr.push_back(&mxb1(row, col));
                    }
                }
            }
        };
        auto is_not = [&]() -> void {
            for(long long row{} ; row < rows ; row++){
                for(long long col{} ; col < cols ; col++){
                    if(mxb1(row, col) != other(row, col)){
                        p_arr.push_back(&mxb1(row, col));
                    }
                }
            }
        };
        
        switch(condition)
        {   
        case(Condition::over):      over(); break;
        case(Condition::over_or):   over_or(); break;
        case(Condition::under):     under(); break;
        case(Condition::under_or):  under_or(); break;
        case(Condition::is) :       is(); break;
        case(Condition::is_not):    is_not(); break;
        }
        return p_arr;
    }

    template<typename Tmx, typename T, typename R>
    requires (std::is_arithmetic_v<R>)
    machx::array<T*> make_ptr_arr(Matrix_base<Tmx, T> &mxb1, const Condition condition, const R &value){
        auto rows = mxb1.get_rows();
        auto cols = mxb1.get_cols();
        machx::array<T*> p_arr{};
        p_arr.reserve(rows*cols);

        auto over = [&]() -> void {
            for(long long row{} ; row < rows ; row++){
                for(long long col{} ; col < cols ; col++){
                    if(mxb1(row, col) > value){
                        p_arr.push_back(&mxb1(row, col));
                    }
                }
            }
        };
        auto under = [&]() -> void {
            for(long long row{} ; row < rows ; row++){
                for(long long col{} ; col < cols ; col++){
                    if(mxb1(row, col) < value){
                        p_arr.push_back(&mxb1(row, col));
                    }
                }
            }
        };
        auto over_or = [&]() -> void {
            for(long long row{} ; row < rows ; row++){
                for(long long col{} ; col < cols ; col++){
                    if(mxb1(row, col) >= value){
                        p_arr.push_back(&mxb1(row, col));
                    }
                }
            }
        };
        auto under_or = [&]() -> void {
            for(long long row{} ; row < rows ; row++){
                for(long long col{} ; col < cols ; col++){
                    if(mxb1(row, col) <= value){
                        p_arr.push_back(&mxb1(row, col));
                    }
                }
            }
        };
        auto is = [&]() -> void {
            for(long long row{} ; row < rows ; row++){
                for(long long col{} ; col < cols ; col++){
                    if(mxb1(row, col) == value){
                        p_arr.push_back(&mxb1(row, col));
                    }
                }
            }
        };
        auto is_not = [&]() -> void {
            for(long long row{} ; row < rows ; row++){
                for(long long col{} ; col < cols ; col++){
                    if(mxb1(row, col) != value){
                        p_arr.push_back(&mxb1(row, col));
                    }
                }
            }
        };
        auto is_true = [&]() -> void {
            for(long long row{} ; row < rows ; row++){
                for(long long col{} ; col < cols ; col++){
                    p_arr.push_back(&mxb1(row, col));
                }
            }
        };
        switch(condition)
        {   
        case(Condition::over):      over(); break;
        case(Condition::over_or):   over_or(); break;
        case(Condition::under):     under(); break;
        case(Condition::under_or):  under_or(); break;
        case(Condition::is) :       is(); break;
        case(Condition::is_not):    is_not(); break;
        case(Condition::is_true):   is_true(); break;
        }
        return p_arr;
    }


    template<typename>
    class Mask_view;
    
    template<typename Tmx, typename T, typename R>
    Mask_view<T> operator>(mask_flag<Tmx, T> flag, const R &val) 
    {   
        return {make_ptr_arr(*flag.ptr, Condition::over, val)};
    }
    template<typename Tmx, typename T, typename R>
    Mask_view<T> operator<(mask_flag<Tmx, T> flag, const R &val) 
    {   
        return {make_ptr_arr(*flag.ptr, Condition::under, val)};
    }
    template<typename Tmx, typename T, typename R>
    Mask_view<T> operator>=(mask_flag<Tmx, T> flag, const R &val) 
    {   
        return {make_ptr_arr(*flag.ptr, Condition::over_or, val)};
    }
    template<typename Tmx, typename T, typename R>
    Mask_view<T> operator<=(mask_flag<Tmx, T> flag, const R &val)  
    {   
        return {make_ptr_arr(*flag.ptr, Condition::under_or, val)};
    }
    template<typename Tmx, typename T, typename R>
    Mask_view<T> operator==(mask_flag<Tmx, T> flag, const R &val) 
    {   
        return {make_ptr_arr(*flag.ptr, Condition::is, val)};
    }
    template<typename Tmx, typename T, typename R>
    Mask_view<T> operator!=(mask_flag<Tmx, T> flag, const R &val) 
    {   
        return {make_ptr_arr(*flag.ptr, Condition::is_not, val)};
    }
}
//
//ASSIGN FLAG
//
namespace machx
{
    template<typename T>
    template<typename Rmx, typename R>
    Matrix<T>& assign_flag<T>::operator=(const Matrix_base<Rmx, R>& mxb1)
    {   
        Matrix<T> temp{mxb1};
        std::swap(ptr->matrix, temp.matrix);
        ptr->rows = temp.rows;
        ptr->cols = temp.cols;

        lifetime_table::new_generation(ptr->id);
        return *ptr;
    }

    template<typename T>
    Matrix<T>& assign_flag<T>::operator=(Matrix<T> other)
    {
        std::swap(ptr->matrix, other.matrix);
        ptr->rows = other.rows;
        ptr->cols = other.cols;
        
        lifetime_table::new_generation(ptr->matrix_id);
        return *ptr;
    }
}


//
//assign view
//
namespace machx
{
    template<typename T>
    Matrix_view<T>& view_assign_flag<T>::operator=(Matrix_view<T> mxv1)
    {   
        ptr->m_ptr = mxv1.m_ptr;
        ptr->row_slice = mxv1.row_slice;
        ptr->col_slice = mxv1.col_slice;
        ptr->matrix_id = mxv1.matrix_id;
        ptr->generation = mxv1.generation;

        return *ptr;
    }   

    template<typename T>
    Matrix_view<T>& view_assign_flag<T>::operator=(Matrix<T> &matrix)
    {   
        ptr->change_pointer(matrix);
        return *ptr;
    }   
}


