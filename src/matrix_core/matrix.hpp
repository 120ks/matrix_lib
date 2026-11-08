#pragma once

#include <utility>
#include <type_traits>
#include <mutex>

#include "lifetime_table.hpp"
#include "broadcast.hpp"




template<typename>
class mask_flag;
namespace machine
{   
    template <typename T>
    class Matrix_view;

    template <typename T>
    class Mask_view;
template<typename T>
class Matrix : public Matrix_base<Matrix<T>, T>
{   
    private:
        machine::array<T> matrix{};
        long long rows{};
        long long cols{};
        const long long id{lifetime_table::get_id()};
        std::mutex mxt;
//
//flags
//
    public:
        friend class mask_flag<Matrix<T>, T>;
        mask_flag<Matrix<T>, T> M(){return *this;}

        friend class assign_flag<T>;
        assign_flag<T> A(){return *this;}

        friend class Matrix_view<T>;
        Matrix_view<T> V(){return *this;}

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
        explicit Matrix(const long long row_count = 1, const long long col_count = 1, T val = T{})
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
            lifetime_table::no_data(other.id);
            other.rows = 0;
            other.cols = 0;
        }
        
        template<typename R>
        Matrix(const Matrix_view<R> &view)
        : matrix{}
        , rows{view.get_rows()}
        , cols{view.get_cols()}
        {
            auto r1{view.get_rows()};
            auto c1{view.get_cols()};
            for(long long r{} ; r<r1 ; r++)
            {
                for(long long c{} ; c<c1 ; c++)
                {
                    (*this)(r, c) = static_cast<T>(view(r, c));
                }
            }
        }
        template<typename R>
        Matrix(const Matrix<R> &other)
        : matrix{other.get_rows() * other.get_cols()}
        , rows{other.get_rows()}
        , cols{other.get_cols()}
        {
            quick_throw::throw_if_no_data(other);
            auto r1{other.get_rows()};
            auto c1{other.get_cols()};
            for(long long r{} ; r<r1 ; r++)
            {
                for(long long c{} ; c<c1 ; c++)
                {
                    (*this)(r, c) = static_cast<T>(other(r, c));
                }
            }
        }  
        template<typename R>
        Matrix(const Mask_view<R>& other)
        : matrix{other.get_cols()}
        , rows{1}
        , cols{other.get_cols()}
        {
            quick_throw::throw_if_no_data(other);
            for(long long col{} ; col<cols ; col++){
                matrix[col] = other(0, col);
            }
        }

        template<typename Rmx, typename R>
        Matrix(const Matrix_base<Rmx, R> &mxb1)
        : Matrix(static_cast<const Rmx&>(mxb1))
        {}

        ~Matrix()override
        {
            lifetime_table::remove_id(this->id);
        }
//
//SLICE AND MASK
//
        Matrix_view<T> operator()(Slice r_slice, Slice c_slice){   

            r_slice.end = (r_slice.end <= 0) ? rows+r_slice.end : r_slice.end; //if 0, rows, if -1, rows-1
            c_slice.end = (c_slice.end <= 0) ? cols+c_slice.end : c_slice.end;
            
            r_slice.begin = (r_slice.begin < 0) ? rows+r_slice.begin : r_slice.begin;
            c_slice.begin = (c_slice.begin < 0) ? cols+c_slice.begin : c_slice.begin;

            return {*this, r_slice, c_slice};
        }   

        Matrix_view<T> operator()(Slice r_slice) //row slice
        {   
            r_slice.end = (r_slice.end <= 0) ? rows+r_slice.end : r_slice.end;
            r_slice.begin = (r_slice.begin < 0) ? rows+r_slice.begin : r_slice.begin;

            Slice c_slice{0, cols};

            return {*this, r_slice, c_slice};
        }

        Matrix_view<T> operator[](Slice c_slice) //cols slice
        {
            c_slice.end = (c_slice.end <= 0) ? cols+c_slice.end : c_slice.end;
            c_slice.begin = (c_slice.begin < 0) ? cols+c_slice.begin : c_slice.begin;

            Slice r_slice{0, rows};
            return {*this, r_slice, c_slice};
        }

        template<typename BOOLmx>
        Mask_view<T> operator[](const Matrix_base<BOOLmx, bool> &mask);
//
// OVERRIDES
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

        inline long long get_rows()const {return rows;}
        inline long long get_cols()const {return cols;}

        inline Matrix<T> transpose()const{
            Matrix<T> transposed{cols, rows};
            for(long long row{} ; row<rows ; row++){
                for(long long col{} ; col<cols ; col++){
                    transposed(col, row) = (*this)(row, col);
                }
            }
            return transposed;
        }

        inline Matrix<T> copy()const{   
            return Matrix<T>{*this};
        }

        template<typename R, typename R2 = R>
        inline Matrix<R> copy_as()const{
            return Matrix<R>{*this};
        }

        inline operator bool() const{
            return lifetime_table::check_id(id);
        }

        inline long long get_id()const{return id;}

        long long get_gen()const{return lifetime_table::get_gen(id);}
        std::mutex& get_mutex() {return mxt;}

//
// ASSIGN OPERATORS
//      
        Matrix<T>& operator=(Matrix<T> mx)
        {   
            quick_throw::throw_if_no_data(*this);
            quick_throw::throw_if_dimension_mismatch(*this, mx);

            std::swap(this->matrix, mx.matrix);
            this->rows = mx.rows;
            this->cols = mx.cols;

            return *this;
        }

        template<typename R, typename Rmx, typename U = T>
        Matrix<T>& operator=(const Matrix_base<Rmx, R> &mxb1)
        {   
            
            quick_throw::throw_if_dimension_mismatch(*this, mxb1);
            Matrix<T> mx{mxb1};
            this->matrix = std::move(mx.matrix);

            return *this;
        }

        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Matrix<T>& operator=(R val)
        {   
            for(auto& value : this->matrix){
                value = val;
            }
            return *this;
        }
//
//MEMBER ARITHMETIC OPERATORS
//
        template<typename R>
        Matrix<T>& operator+=(const R &val)//handles both nums and base<T>
        {   
            machine::broadcast(*this, val, Operation::add);
            return *this;
        }
        template<typename R>
        Matrix<T>& operator-=(const R &val)
        {
            machine::broadcast(*this, val, Operation::substract);
            return *this;
        }

        template<typename R>
        Matrix<T>& operator*=(const R &val)
        {
            machine::broadcast(*this, val, Operation::multiply);
            return *this;
        }

        template<typename R>
        Matrix<T>& operator/=(const R &val)
        {
            machine::broadcast(*this, val, Operation::divide);
            return *this;
        }
//
//NOT OPERATOR
//      
        template <typename U = T>
        requires (std::is_same_v<U, bool>)
        Matrix<bool> operator!()const
        {   
            Matrix<bool> mask{*this};
            for(auto& val : mask.matrix){
                val = !val;
            }
            return mask;
        }

        T* begin(){
            if(*this){
                return &matrix[0];
            }
            return nullptr;
        }
        T* end(){
            return &matrix[0]+rows*cols;
        }
        const T* begin()const{
            if(*this){
                return &matrix[0];
            }
            return nullptr;
        }
        const T* end()const{
            return &matrix[0]+rows*cols;
        }
};
}
namespace machine
{
    template <typename T>
    struct is_matrix : std::false_type {};

    template <typename U>
    struct is_matrix<Matrix<U>> : std::true_type {};

    template <typename T>
    inline constexpr bool is_matrix_v = is_matrix<T>::value;
}
