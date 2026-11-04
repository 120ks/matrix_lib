#pragma once

#include <utility>
#include <type_traits>
#include <mutex>

#include "lifetime_table.hpp"
#include "broadcast.hpp"


template <typename T>
class Matrix_view;

template<typename>
class mask_flag;
namespace machine
{   
    

template<typename T>
class Matrix : public Matrix_base<T>
{   
    private:
        std::vector<T> matrix{};
        size_t rows{};
        size_t cols{};
        const size_t id{lifetime_table::get_id()};
        std::mutex mxt;
//
//flags
//
    public:
        friend class mask_flag<T>;
        mask_flag<T> M(){return *this;}

        friend class assign_flag<T>;
        assign_flag<T> A(){return *this;}

        friend class Matrix_view<T>;
        Matrix_view<T> V(){return *this;}

        friend class matrix_lock;
        matrix_lock L(){return *this;}

        std::vector<T>& get_matrix()
        {
            return matrix;
        }
        const std::vector<T>& get_matrix() const
        {
            return matrix;
        }
//
//CONSTRUCTORS
//  
        explicit Matrix(const size_t row_count = 1, const size_t col_count = 1, T val = T{})
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

        Matrix(Matrix<T> &&other) noexcept
        : matrix{std::move(other.matrix)}
        , rows{other.rows}
        , cols{other.cols}
        {   
            quick_throw::throw_if_no_data(other);
            lifetime_table::no_data(other.id);
        }
        
        template<typename R>
        Matrix(const Matrix_base<R> &mxb1)
        : matrix(mxb1.get_rows() * mxb1.get_cols())
        , rows{mxb1.get_rows()}
        , cols{mxb1.get_cols()}
        {   
            quick_throw::throw_if_no_data(mxb1);    

            auto r1{mxb1.get_rows()};
            auto c1{mxb1.get_cols()};
            for(size_t r{} ; r<r1 ; r++)
            {
                for(size_t c{} ; c<c1 ; c++)
                {
                    (*this)(r, c) = static_cast<T>(mxb1(r, c));
                }
            }
        }
        ~Matrix()override
        {
            lifetime_table::remove_id(this->id);
        }
        Matrix(std::vector<T> vec, const size_t row_count = 0, const size_t col_count = 0)
        : matrix{std::move(vec)}
        , rows{row_count}
        , cols{col_count}
        {   
            if(rows*cols != matrix.size()){rows = 1 ; cols = matrix.size();}
        }

        template<typename R>
        Matrix(Matrix_base<R> &other, const Matrix_base<boolean> &mask)
        : matrix{}
        , rows{1}
        , cols{}
        {   
            auto r1{mask.get_rows()};
            auto c1{mask.get_cols()};
            
            quick_throw::throw_if_dimension_mismatch(other, mask, "mxb2 = mask");
            matrix.reserve(r1*c1);

            for(size_t r{} ; r<r1 ; r++)
            {
                for(size_t c{} ; c<c1 ; c++)
                {
                    if(mask(r, c) == true){
                        matrix.push_back(mask_ref{other(r, c)});
                    }
                }
            }
            cols = matrix.size();
        }
//
//SLICE AND MASK
//
        Matrix_view<T> operator()(Slice r_slice, Slice c_slice)
        {   
            
            quick_throw::throw_if_slice_mismatch(*this, r_slice, c_slice);

            r_slice.end = (r_slice.end == 0) ? rows : r_slice.end;
            c_slice.end = (c_slice.end == 0) ? cols : c_slice.end;
            return {*this, r_slice, c_slice};
        }   
        Matrix_view<T> operator()(Slice r_slice) //row slice
        {   
            quick_throw::throw_if_slice_mismatch(*this, r_slice, Slice{}, "row slice");

            r_slice.end = (r_slice.end == 0) ? rows : r_slice.end;
            Slice c_slice{0, cols};

            return {*this, r_slice, c_slice};
        }

        Matrix_view<T> operator[](Slice c_slice) //cols slice
        {
            quick_throw::throw_if_slice_mismatch(*this, Slice{}, c_slice, "col slice");

            c_slice.end = (c_slice.end == 0) ? cols : c_slice.end;

            Slice r_slice{0, rows};
            return {*this, r_slice, c_slice};

        }

        Matrix<mask_ref<T>> operator[](const Matrix_base<boolean> &mask){
            return {*this, mask};
        }
//
// OVERRIDES
//        
        inline T& operator()(const size_t row, const size_t col)override{
            
            return matrix[row*cols + col];
        }
        
        inline const T& operator()(const size_t row, const size_t col)const override{

            return matrix[row*cols + col];
        }

        inline size_t get_rows()const override {return rows;}
        inline size_t get_cols()const override {return cols;}

        inline Matrix<T> transpose()const override{
            Matrix<T> transposed{cols, rows};
            for(size_t row{} ; row<rows ; row++){
                for(size_t col{} ; col<cols ; col++){
                    transposed(col, row) = (*this)(row, col);
                }
            }
            return transposed;
        }

        inline Matrix<T> copy()const override{   
            return Matrix<T>{*this};
        }

        inline operator bool() const override{
            return lifetime_table::check_id(id);
        }

        inline size_t get_id()const{return id;}

        long long get_gen()const{return lifetime_table::get_gen(id);}
        std::mutex& get_mutex() override{return mxt;}

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

        template<typename R, typename U = T>
        requires (!is_mask_ref_v<U>)
        Matrix<T>& operator=(const Matrix_base<R> &mxb1)
        {   
            
            quick_throw::throw_if_dimension_mismatch(*this, mxb1);
            Matrix<T> mx{mxb1};
            this->matrix = std::move(mx.matrix);

            return *this;
        }
        
        template<typename R>
        Matrix<T>& operator=(const Matrix_base<mask_ref<R>> &mxb1)
        {   
            machine::broadcast(*this, mxb1, Operation::equal);
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
        requires (std::is_same_v<U, boolean>)
        Matrix<boolean> operator!()const
        {   
            Matrix<boolean> mask{*this};
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
