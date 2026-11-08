#pragma once

#include "matrix.hpp"

namespace machine
{

template<typename T>
class Matrix_view : public Matrix_base<Matrix_view<T>, T>
{   
    template<typename>
    friend class Matrix;

    template<typename>
    friend class Mask_view;

    friend class mask_flag<Matrix_view<T>, T>;
    friend class view_assign_flag<T>;
    friend class matrix_lock;

    private:
        T* start_data{};
        Slice row_slice{};
        Slice col_slice{};
        long long base_cols{};
        long long base_rows{};
        long long matrix_id{};
        long long generation{};
        std::mutex* mxt{};
//  
//CONSTRUCTORS
//
    public:
//
//FLAGS
//          
        mask_flag<Matrix_view<T>, T> M(){
            return *this;
        }
        view_assign_flag<T> A(){
            return *this;
        }

        Matrix_view(const Matrix<T> &matrix)
        : Matrix_view(matrix, Slice{0, matrix.rows}, Slice{0, matrix.cols})
        {}

        Matrix_view(const Matrix<T> &matrix, Slice r_slice, Slice c_slice)
        : start_data{const_cast<T*>(matrix.begin())}
        , row_slice{r_slice}
        , col_slice{c_slice}
        , base_cols{matrix.get_cols()}
        , base_rows{matrix.get_rows()}
        , matrix_id{matrix.matrix_id}
        , generation{matrix.get_gen()}
        {
            quick_throw::throw_if_no_data(matrix, " binding view to empty data");
            Matrix<T> &mx = const_cast<Matrix<T>&>(matrix);
            mxt = &mx.get_mutex();
            row_slice.end = (row_slice.end <= 0) ? base_rows+row_slice.end : row_slice.end; 
            col_slice.end = (col_slice.end <= 0) ? base_cols+col_slice.end : col_slice.end; 

            row_slice.begin = (row_slice.end < 0) ? base_rows+row_slice.begin : row_slice.begin; 
            col_slice.begin = (col_slice.end < 0) ? base_cols+col_slice.begin : col_slice.begin;
        }
        Matrix_view(const Matrix_view<T> &other)
        : Matrix_view(other, other.row_slice, other.col_slice)
        {}
        Matrix_view(const Matrix_view<T> &other, Slice r_slice, Slice c_slice)
        : start_data{const_cast<T*>(other.start_data)}
        , row_slice{r_slice}
        , col_slice{c_slice}
        , base_cols{other.base_cols}
        , base_rows{other.base_rows}
        , matrix_id{other.matrix_id}
        , generation{other.generation}
        , mxt{other.mxt}
        {
            row_slice.end = (row_slice.end <= 0) ? base_rows+row_slice.end : row_slice.end; 
            col_slice.end = (col_slice.end <= 0) ? base_cols+col_slice.end : col_slice.end; 

            row_slice.begin = (row_slice.end < 0) ? base_rows+row_slice.begin : row_slice.begin; 
            col_slice.begin = (col_slice.end < 0) ? base_cols+col_slice.begin : col_slice.begin;
        }
        Matrix_view(Matrix_view<T> &&other)noexcept
        : Matrix_view(other)
        {}
        Matrix_view(Matrix_view<T> &&other, Slice r_slice, Slice c_slice)
        : Matrix_view(other, r_slice, c_slice)
        {}
        template<typename Rmx, typename R=T>
        requires (std::is_same_v<T, R> && !std::is_same_v<Rmx, Mask_view<R>>)
        Matrix_view(const Matrix_base<Rmx, R> &mxb1)
        : Matrix_view(static_cast<const Rmx&>(mxb1))
        {}
        ~Matrix_view()override = default;
//  
// OVERRIDES
//        
        inline long long get_rows()const {return row_slice.end - row_slice.begin;}
        inline long long get_cols()const {return col_slice.end - col_slice.begin;}

        inline T& operator()(long long row, long long col)
        {   
            row = (row<0) ? this->get_rows()+row : row;
            col = (col<0) ? this->get_cols()+col : col;
            return *(start_data + (row+row_slice.begin)*base_cols + col+col_slice.begin);
        }
        inline const T& operator()(long long row, long long col)const
        {   
            row = (row<0) ? this->get_rows()+row : row;
            col = (col<0) ? this->get_cols()+col : col;
            return *(start_data + (row+row_slice.begin)*base_cols + col+col_slice.begin);
        }
        Matrix<T> copy()const{   
            return {*this};
        }

        template<typename R>
        inline Matrix<R> copy_as(){
            return Matrix<R>{*this};
        }

        operator bool() const{   
            auto current_gen {lifetime_table::get_gen(matrix_id)};
            return (current_gen == generation) && (current_gen>0);
        }

        inline long long get_id()const{return matrix_id;}

        inline long long get_gen()const{return generation;}

        T* begin(){
            if(this->get_cols() == base_cols){
                return &((*this)(0, 0));
            }
            return nullptr;
        }

        T* end(){
            return &((*this)(-1, -1))+1;
        }
        const T* begin()const{
            if(this->get_cols() == base_cols){
                return &((*this)(0, 0));
            }
            return nullptr;
        }

        const T* end()const
        {
            return &((*this)(-1, -1))+1;
        }

        std::mutex& get_mutex(){
            return *mxt;
        }

//
//SLICING AND MASKING
//      
        Matrix_view<T> operator()(const Slice r_slice, const Slice c_slice)
        {   
            quick_throw::throw_if_no_data(*this);
            quick_throw::throw_if_slice_mismatch(*this, r_slice, c_slice);

            return{*this, row_slice+r_slice, col_slice+c_slice};
        }

        Matrix_view<T> operator()(const Slice r_slice) //row slice
        {   
            quick_throw::throw_if_no_data(*this);
            quick_throw::throw_if_slice_mismatch(*this, r_slice, Slice{}, "row slicing");
            return {*this, row_slice+r_slice, col_slice};
        }

        Matrix_view<T> operator[](const Slice c_slice) //col slice
        {   
            quick_throw::throw_if_no_data(*this);
            quick_throw::throw_if_slice_mismatch(*this, Slice{}, c_slice, "row slicing");
            return {*this, row_slice, col_slice+c_slice};
        }

        Mask_view<T> operator[](const Matrix<bool> &mask){ //note the  []
            return{*this, mask};
        }
//
// ASSIGN OPERATORS
//      
        Matrix_view& operator=(const Matrix_view<T> &mxv)
        {
            broadcast(*this, mxv, Operation::equal);
            return *this;
        }
        Matrix_view& operator=(Matrix_view<T> &&mxv){
            broadcast(*this, mxv, Operation::equal);
            return *this;
        }
        using Matrix_base<Matrix_view<T>, T>::operator=;
//
//other functions
//
        void reset_view()
        {
            row_slice.begin = 0;
            row_slice.end = base_rows;
            col_slice.begin = 0;
            col_slice.end = base_cols;

            if(lifetime_table::check_id(matrix_id)){
                generation = lifetime_table::get_gen(matrix_id);
                assert(generation>0); //DEBUG
            }
        }

        void force_update()
        {
            if(lifetime_table::check_id(matrix_id)){
                generation = lifetime_table::get_gen(matrix_id);
                assert(generation>0); //DEBUG
            }
        }

        Matrix_view& change_pointer(const Matrix<T> &matrix)
        {   
            quick_throw::throw_if_no_data(matrix, "cant view a matrix with no data");
            start_data = const_cast<T*>(matrix.begin());
            matrix_id = matrix.matrix_id;
            reset_view();
            return *this;
        }
    };//class end
}
