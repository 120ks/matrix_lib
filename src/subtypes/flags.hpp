#pragma once
#include "mask_view.hpp"

//
//MASK FLAG COMPARISON OPERATORS
//
namespace machine
{   
    template<typename>
    class Mask_view;

    template<typename Tmx, typename T, typename R>
    Mask_view<T> operator>(mask_flag<Tmx, T> flag, const R &val) 
    {   
        Matrix<bool> mask = make_mask(*(flag.ptr), Condition::over, val);
        return {*(flag.ptr), mask};
    }
    template<typename Tmx, typename T, typename R>
    Mask_view<T> operator<(mask_flag<Tmx, T> flag, const R &val) 
    {   
        Matrix<bool> mask = make_mask(*(flag.ptr), Condition::under, val);
        return {*(flag.ptr), mask};
    }
    template<typename Tmx, typename T, typename R>
    Mask_view<T> operator>=(mask_flag<Tmx, T> flag, const R &val) 
    {   
        Matrix<bool> mask = make_mask(*(flag.ptr), Condition::over_or, val);
        return {*(flag.ptr), mask};
    }
    template<typename Tmx, typename T, typename R>
    Mask_view<T> operator<=(mask_flag<Tmx, T> flag, const R &val)  
    {   
        Matrix<bool> mask = make_mask(*(flag.ptr), Condition::under_or, val);
        return {*(flag.ptr), mask};
    }
    template<typename Tmx, typename T, typename R>
    Mask_view<T> operator==(mask_flag<Tmx, T> flag, const R &val) 
    {   
        Matrix<bool> mask = make_mask(*(flag.ptr), Condition::is, val);
        return {*(flag.ptr), mask};
    }
    template<typename Tmx, typename T, typename R>
    Mask_view<T> operator!=(mask_flag<Tmx, T> flag, const R &val) 
    {   
        Matrix<bool> mask = make_mask(*(flag.ptr), Condition::is_not, val);
        return {*(flag.ptr), mask};
    }
}
//
//ASSIGN FLAG
//
namespace machine
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
        
        lifetime_table::new_generation(ptr->id);
        return *ptr;
    }
}


//
//assign view
//
namespace machine
{
    template<typename T>
    Matrix_view<T>& view_assign_flag<T>::operator=(Matrix_view<T> mxv1)
    {   
        ptr->start_data = mxv1.start_data;
        ptr->row_slice = mxv1.row_slice;
        ptr->col_slice = mxv1.col_slice;
        ptr->base_rows = mxv1.base_rows;
        ptr->base_cols = mxv1.base_cols;

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


