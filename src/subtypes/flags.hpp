#pragma once
#include "matrix_view.hpp"



//
//MASK_FLAG
//
namespace machine
{

    template<typename T, typename R>
    requires (std::integral<R> || std::floating_point<R>)
    std::vector<mask_ref<T>> get_true_elements(Matrix_base<T> &mxb1, const Condition condition, const R &val)
    {   
        quick_throw::throw_if_no_data(mxb1);

        size_t rows{mxb1.get_rows()};
        size_t cols{mxb1.get_cols()};
        std::vector<mask_ref<T>> vec;
        vec.reserve(rows*cols);

        for(size_t r{} ; r<rows ; r++){
            for(size_t c{} ; c<cols ; c++)
            {
                auto& mx_val = mxb1(r, c);
                switch(condition)
                {
                case(Condition::over):      if(mx_val >  val){vec.push_back(mx_val);} break;
                case(Condition::over_or):   if(mx_val >= val){vec.push_back(mx_val);} break;
                case(Condition::under):     if(mx_val <  val){vec.push_back(mx_val);} break;
                case(Condition::under_or):  if(mx_val <= val){vec.push_back(mx_val);} break;
                case(Condition::is) :       if(mx_val == val){vec.push_back(mx_val);} break;
                case(Condition::is_not):    if(mx_val != val){vec.push_back(mx_val);} break;
                }
            }
        }
        return vec;
    }

    template<typename T, typename R>
    std::vector<mask_ref<T>> get_true_elements(Matrix_base<T> &mxb1, const Condition condition, const Matrix_base<R> &mxb2)
    {   
        quick_throw::throw_if_dimension_mismatch(mxb1, mxb2, "in get_true_element()");
        quick_throw::throw_if_no_data(mxb1);
        quick_throw::throw_if_no_data(mxb2);


        size_t rows{mxb1.get_rows()};
        size_t cols{mxb1.get_cols()};

        std::vector<mask_ref<T>> vec;
        vec.reserve(rows*cols);
        
        for(size_t r{} ; r<rows ; r++){
            for(size_t c{} ; c<cols ; c++)
            {
                auto& mx_val = mxb1(r, c);
                const auto& mx2_val = mxb2(r, c);
                switch(condition)
                {
                case(Condition::over):      if(mx_val >  mx2_val){vec.push_back(mx_val);} break;
                case(Condition::over_or):   if(mx_val >= mx2_val){vec.push_back(mx_val);} break;
                case(Condition::under):     if(mx_val <  mx2_val){vec.push_back(mx_val);} break;
                case(Condition::under_or):  if(mx_val <= mx2_val){vec.push_back(mx_val);} break;
                case(Condition::is) :       if(mx_val == mx2_val){vec.push_back(mx_val);} break;
                case(Condition::is_not):    if(mx_val != mx2_val){vec.push_back(mx_val);} break;
                }
            }
        }
        return vec;
    }
}
//
//MASK FLAG COMPARISON OPERATORS
//
namespace machine
{
    template<typename T, typename R>
    Matrix<mask_ref<T>> operator>(mask_flag<T> flag, const R &val) 
    {   
        return {get_true_elements((*flag.ptr), Condition::over, val)};
    }
    template<typename T, typename R>
    Matrix<mask_ref<T>> operator<(mask_flag<T> flag, const R &val) 
    {   
        return {get_true_elements((*flag.ptr), Condition::under, val)};
    }
    template<typename T, typename R>
    Matrix<mask_ref<T>> operator>=(mask_flag<T> flag, const R &val) 
    {   
        return {get_true_elements((*flag.ptr), Condition::over_or, val)};
    }
    template<typename T, typename R>
    Matrix<mask_ref<T>> operator<=(mask_flag<T> flag, const R &val) 
    {   
        return {get_true_elements((*flag.ptr), Condition::under_or, val)};
    }
    template<typename T, typename R>
    Matrix<mask_ref<T>> operator==(mask_flag<T> flag, const R &val) 
    {   
        return {get_true_elements((*flag.ptr), Condition::is, val)};
    }
    template<typename T, typename R>
    Matrix<mask_ref<T>> operator!=(mask_flag<T> flag, const R &val) 
    {   
        return {get_true_elements((*flag.ptr), Condition::is_not, val)};
    }
}
//
//ASSIGN FLAG
//
namespace machine
{
    template<typename T>
    template<typename R>
    Matrix<T>& assign_flag<T>::operator=(const Matrix_base<R>& mxb1)
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
        ptr->view_ptr = mxv1.view_ptr;
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




namespace machine
{   
    template<typename T>
    matrix_lock::matrix_lock(Matrix_base<T> &matrix)
    : ptr{matrix.get_mutex()}
    {   
        has_locked = true;
        ptr->lock();
    }
    matrix_lock::~matrix_lock()
    {
        ptr->unlock();
        has_locked = false;
    }
    void matrix_lock::lock()
    {   
        if(!has_locked){
            ptr->lock();
        }
    }
    void matrix_lock::unlock(){
        ptr->unlock();
    }
}
