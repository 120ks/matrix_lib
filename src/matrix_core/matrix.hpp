#pragma once

#include <utility>
#include <type_traits>
#include <mutex>

#include "lifetime_table.hpp"
#include "broadcast.hpp"
#include "matrix_base.hpp"



namespace machx
{   
    template <typename T>
    class Matrix_view;

    template <typename T>
    class Mask_view;

    template<typename, typename>
    class mask_flag;
template<typename T>
class Matrix : public Matrix_base<Matrix<T>, T>
{   
    template<typename, typename>
    friend class Matrix_base;

    private:
        machx::array<T> matrix{};
        long long rows{};
        long long cols{};
        const long long matrix_id{lifetime_table::get_id()};
        std::mutex mxt;
//
//flags
//
    public:
        friend class assign_flag<T>;
        assign_flag<T> A(){return *this;}

        friend class Matrix_view<T>;
        Matrix_view<T> V(){return {*this};}

        array<T>& get_matrix()
        {
            return matrix;
        }
        const array<T>& get_matrix() const
        {
            return matrix;
        }
//
//CONSTRUCTORS
//  
        Matrix(const long long row_count = 1, const long long col_count = 1, T val = T{})
        : matrix(row_count * col_count, val)
        , rows{row_count}
        , cols{col_count}
        {
        }

        Matrix(const Matrix<T> &other)
        : matrix{other.matrix}
        , rows{other.rows}
        , cols{other.cols}
        {
            quick_throw::throw_if_no_data(other);
        }  

        Matrix(Matrix<T> &&other)
        : matrix{std::move(other.matrix)}
        , rows{other.rows}
        , cols{other.cols}
        {   
            quick_throw::throw_if_no_data(other);
            lifetime_table::no_data(other.matrix_id);
            other.rows = 0;
            other.cols = 0;
        }

        template<typename Rmx, typename R>
        Matrix(const Matrix_base<Rmx, R> &mxb1)
        : rows{mxb1.get_rows()}
        , cols{mxb1.get_cols()}
        {   
            quick_throw::throw_if_no_data(mxb1);
            matrix.resize(rows*cols);
            for(long long row{} ; row<rows ; row++){
                for(long long col{} ; col<cols ; col++){
                    (*this)(row, col) = static_cast<T>(mxb1(row, col));
                }
            }
        }


        ~Matrix()override
        {
            lifetime_table::remove_id(this->matrix_id);
        }
        //SLICE
        Matrix_view<T> operator()(Slice r_slice, Slice c_slice){   
            r_slice.end = (r_slice.end <= 0) ? rows+r_slice.end : r_slice.end; //if 0, rows, if -1, rows-1
            c_slice.end = (c_slice.end <= 0) ? cols+c_slice.end : c_slice.end;
            
            r_slice.begin = (r_slice.begin < 0) ? rows+r_slice.begin : r_slice.begin;
            c_slice.begin = (c_slice.begin < 0) ? cols+c_slice.begin : c_slice.begin;
            return {*this, r_slice, c_slice};
        }   

        Matrix_view<T> operator()(Slice r_slice){   
            r_slice.end = (r_slice.end <= 0) ? rows+r_slice.end : r_slice.end;
            r_slice.begin = (r_slice.begin < 0) ? rows+r_slice.begin : r_slice.begin;
            return {*this, r_slice, Slice{0, cols}};
        }

        Matrix_view<T> operator[](Slice c_slice){
            c_slice.end = (c_slice.end <= 0) ? cols+c_slice.end : c_slice.end;
            c_slice.begin = (c_slice.begin < 0) ? cols+c_slice.begin : c_slice.begin;
            return {*this, Slice{0, rows}, c_slice};
        }

        //CONST SLICE
        const Matrix_view<T> operator()(Slice r_slice, Slice c_slice)const{   
            r_slice.end = (r_slice.end <= 0) ? rows+r_slice.end : r_slice.end; //if 0, rows, if -1, rows-1
            c_slice.end = (c_slice.end <= 0) ? cols+c_slice.end : c_slice.end;
            
            r_slice.begin = (r_slice.begin < 0) ? rows+r_slice.begin : r_slice.begin;
            c_slice.begin = (c_slice.begin < 0) ? cols+c_slice.begin : c_slice.begin;
            return {*this, r_slice, c_slice};
        }   

        const Matrix_view<T> operator()(Slice r_slice)const{   
            r_slice.end = (r_slice.end <= 0) ? rows+r_slice.end : r_slice.end;
            r_slice.begin = (r_slice.begin < 0) ? rows+r_slice.begin : r_slice.begin;
            return {*this, r_slice, Slice{0, cols}};
        }

        const Matrix_view<T> operator[](Slice c_slice)const{
            c_slice.end = (c_slice.end <= 0) ? cols+c_slice.end : c_slice.end;
            c_slice.begin = (c_slice.begin < 0) ? cols+c_slice.begin : c_slice.begin;
            return {*this, Slice{0, rows}, c_slice};
        }

        using Matrix_base<Matrix<T>, T>::operator[];
        
        //
        //INDEXING
        //        
        inline T& operator()(long long row, long long col){
            row = (row<0) ? rows+row : row;
            col = (col<0) ? cols+col : col;
            return matrix[row*cols + col];
        }
        
        inline const T& operator()(long long row, long long col)const{
            row = (row<0) ? rows+row : row;
            col = (col<0) ? cols+col : col;
            return matrix[row*cols + col];
        }
        inline T& at(long long index){
            index = (index<0) ? rows*cols + index : index;
            return matrix[index];
        }
        inline const T& at(long long index)const{
            index = (index<0) ? rows*cols + index : index;
            return matrix[index];
        }


        inline long long get_rows()const {return rows;}
        inline long long get_cols()const {return cols;}

        inline Matrix<T> copy()const{   
            return Matrix<T>{*this};
        }

        inline operator bool() const{
            return lifetime_table::check_id(matrix_id);
        }

        inline long long get_id()const{return matrix_id;}

        long long get_gen()const{return lifetime_table::get_gen(matrix_id);}
        std::mutex& get_mutex() {return mxt;}

//
// ASSIGN OPERATORS
//      
        using Matrix_base<Matrix<T>, T>::operator=;

        Matrix<T>& operator=(Matrix<T> mx)
        {   
            quick_throw::throw_if_no_data(*this);
            quick_throw::throw_if_dimension_mismatch(*this, mx);

            std::swap(this->matrix, mx.matrix);
            this->rows = mx.rows;
            this->cols = mx.cols;

            return *this;
        }

//
//NOT OPERATOR
//      
        template <typename U = T>
        requires (std::is_same_v<U, bool>)
        Matrix<bool> operator!()const{   
            Matrix<bool> mask{*this};
            for(auto& val : mask.matrix){
                val = !val;
            }
            return mask;
        }

        T* begin(){
            if(*this){
                return matrix.begin();
            }
            return nullptr;
        }
        T* end(){
            return matrix.end();
        }
        const T* begin()const{
            if(*this){
                return matrix.begin();
            }
            return nullptr;
        }
        const T* end()const{
            return matrix.end();
        }
};
}
namespace machx
{
    template <typename T>
    struct is_matrix : std::false_type {};

    template <typename U>
    struct is_matrix<Matrix<U>> : std::true_type {};

    template <typename T>
    inline constexpr bool is_matrix_v = is_matrix<T>::value;
}
