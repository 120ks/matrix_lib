#pragma once
#include "matrix_subtypes.hpp"
#include <type_traits>
#include <cassert>
#include "matrix_helper.hpp"
#include "matrix_base.hpp"
#include <utility>



template <typename T>
class Slice_view;

template<typename>
class mask_flag;


template<typename T>
class Matrix : public Matrix_base<T>
{   
    friend class Slice_view<T>;
    private:
        std::vector<T> matrix{};
        size_t rows{};
        size_t cols{};

        std::vector<T>& get_matrix(){return matrix;}
        
    public:
//
//flag cababilities
//  
    friend class assign_flag<T>;
    friend class mask_flag<T>;
    mask_flag<T> M();
    assign_flag<T> A();
//
//CONSTRUCTORS
//  
        explicit Matrix(const size_t row_count = 1, const size_t col_count = 1, T val = T{})
        : matrix(row_count * col_count, val)
        , rows{row_count}
        , cols{col_count}
        {}

        Matrix(const Matrix<T> &other)
        : matrix{other.matrix}
        , rows{other.rows}
        , cols{other.cols}
        {}  

        Matrix(Matrix<T> &&other) noexcept
        : matrix{std::move(other.matrix)}
        , rows{other.rows}
        , cols{other.cols}
        {}
        
        Matrix(const Matrix_base<T> &mxb1)
        : matrix(mxb1.get_rows() * mxb1.get_cols())
        , rows{mxb1.get_rows()}
        , cols{mxb1.get_cols()}
        {   
            auto r1{mxb1.get_rows()};
            auto c1{mxb1.get_cols()};
            for(size_t r{} ; r<r1 ; r++){
                for(size_t c{} ; c<c1 ; c++){
                    (*this)(r, c) = mxb1(r, c);
                }
            }
        }

        template<typename R>
        Matrix(const Matrix_base<R> &mxb1)
        : matrix(mxb1.get_rows() * mxb1.get_cols())
        , rows{mxb1.get_rows()}
        , cols{mxb1.get_cols()}
        {
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
        ~Matrix()override = default;

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
            
            if(!matrix_math::same_dims(other, mask))
            {
                throw dimension_mismatch(std::format("fitting mask({}x{} into ({}x{})", r1, c1, other.get_rows(), other.get_cols()));
            }
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
        Slice_view<T> operator()(Slice r_slice, Slice c_slice)
        {   
            
            if((r_slice.end>rows) || (c_slice.end>cols)){
                throw slicing_mismatch(std::format("sl({}, {}) sl({}, {}) matrix({}x{})", r_slice.begin, r_slice.end, c_slice.begin, c_slice.end , rows, cols));
            }

            r_slice.end = (r_slice.end == 0) ? rows : r_slice.end;
            c_slice.end = (c_slice.end == 0) ? cols : c_slice.end;
            return {this, r_slice, c_slice};
        }   
        Slice_view<T> operator()(Slice r_slice)
        {   
            
            if(r_slice.end>rows)
            {
                throw slicing_mismatch(std::format("r_slice({}, {}) matrix({}x{})", r_slice.begin, r_slice.end, rows, cols));
            }

            r_slice.end = (r_slice.end == 0) ? rows : r_slice.end;
            Slice c_slice{0, cols};
            return {this, r_slice, c_slice};
        }

        Matrix<mask_ref<T>> operator[](const Matrix_base<boolean> &mask){
            return {*this, mask};
        }
//
// OVERRIDES
//        
        inline T& operator()(const size_t row, const size_t col)override{
            if((row>=rows) || (col>=cols)){
                throw indexing_mismatch(std::format("index ({}, {}), matrix({}x{})", row, col, rows, cols));
            }
            return matrix[row*cols + col];
        }
        
        inline const T& operator()(const size_t row, const size_t col)const override{
            if((row>=rows) || (col>=cols)){
                throw indexing_mismatch(std::format("index ({}, {}), matrix({}x{})", row, col, rows, cols));
            }
            return matrix[row*cols + col];
        }

        inline size_t get_rows()const override {return rows;}
        inline size_t get_cols()const override {return cols;}

        Matrix<T> transpose()const override
        {
            Matrix<T> transposed{cols, rows};
            for(size_t row{} ; row<rows ; row++){
                for(size_t col{} ; col<cols ; col++){
                    transposed(col, row) = (*this)(row, col);
                }
            }
            return transposed;
        }

        Matrix<T> copy()const override
        {
            return Matrix<T>{*this};
        }
//
// ASSIGN OPERATORS
//      
        
        Matrix<T>& operator=(Matrix<T> mx)
        {   
            std::swap(this->matrix, mx.matrix);
            this->rows = mx.rows;
            this->cols = mx.cols;
            return *this;
        }

        template<typename R, typename U = T>
        requires (!is_mask_ref_v<U>)
        Matrix<T>& operator=(const Matrix_base<R> &mxb1)
        {   
            Matrix<T> mx{mxb1};
            this->matrix = std::move(mx.matrix);
            this->rows = mx.rows;
            this->cols = mx.cols;
            return *this;
        }
        template<typename R>
        Matrix<T>& operator=(const Matrix_base<mask_ref<R>> &mxb1)
        {   
            method::broadcast(*this, mxb1, Operation::equal);
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
            method::broadcast(*this, val, Operation::add);
            return *this;
        }
        template<typename R>
        Matrix<T>& operator-=(const R &val)
        {
            method::broadcast(*this, val, Operation::substract);
            return *this;
        }

        template<typename R>
        Matrix<T>& operator*=(const R &val)
        {
            method::broadcast(*this, val, Operation::multiply);
            return *this;
        }


        template<typename R>
        Matrix<T>& operator/=(const R &val)
        {
            method::broadcast(*this, val, Operation::divide);
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
};
//
//CREATING BOOLEANS
//
namespace boolean_logic
{   
    template<typename T, typename R>
    requires (std::integral<R> || std::floating_point<R>)
    Matrix<boolean> make_mask(const Matrix_base<T> &mxb1, const Condition condition, const R &threshold)
    {
        size_t r1{mxb1.get_rows()};
        size_t c1{mxb1.get_cols()};
        Matrix<boolean> mask{r1, c1, boolean{false}};

        using common_type = std::common_type_t<T, R>;
        auto N_threshold {static_cast<common_type> (threshold)};

        for(size_t r{} ; r<r1 ; r++){
            for(size_t c{} ; c<c1 ; c++)
            {
                auto current_element = static_cast<common_type>(mxb1(r, c));
                switch(condition)
                {   
                case(Condition::is_true):   mask(r, c) = true; break;
                case(Condition::is_false):  mask(r, c) = false; break;
                case(Condition::over):      mask(r, c) = (current_element >  N_threshold); break;
                case(Condition::over_or):   mask(r, c) = (current_element >= N_threshold); break;
                case(Condition::under):     mask(r, c) = (current_element <  N_threshold); break;
                case(Condition::under_or):  mask(r, c) = (current_element <= N_threshold); break;
                case(Condition::is) :       mask(r, c) = (current_element == N_threshold); break;
                case(Condition::is_not):    mask(r, c) = (current_element != N_threshold); break;
                }
            }
        }
        return mask;
    }
    template<typename T, typename R>
    Matrix<boolean> make_mask(const Matrix_base<T> &mxb1, const Condition condition, const Matrix_base<R> &mxb2)
    {   
        size_t r1{mxb1.get_rows()};
        size_t c1{mxb1.get_cols()};
        if(!matrix_math::same_dims(mxb1, mxb2)){
            throw dimension_mismatch(std::format("masking mxb1({}x{}), with mxb2({}x{})", r1, c1, mxb2.get_rows(), mxb2.get_cols()));
        }
        
        Matrix<boolean> mask{r1, c1, boolean{false}};

        for(size_t r{} ; r<r1 ; r++){
            for(size_t c{} ; c<c1 ; c++){

                auto mxb1_current = mxb1(r, c);
                auto mxb2_current = mxb2(r, c);
                switch(condition)
                {   
                case(Condition::is_true):   mask(r, c) = true; break;
                case(Condition::is_false):  mask(r, c) = false; break;
                case(Condition::over):      mask(r, c) = (mxb1_current > mxb2_current); break;
                case(Condition::over_or):   mask(r, c) = (mxb1_current >= mxb2_current); break;
                case(Condition::under):     mask(r, c) = (mxb1_current < mxb2_current); break;
                case(Condition::under_or):  mask(r, c) = (mxb1_current <= mxb2_current); break;
                case(Condition::is) :       mask(r, c) = (mxb1_current == mxb2_current); break;
                case(Condition::is_not):    mask(r, c) = (mxb1_current != mxb2_current); break;
                }
            }
        }
        return mask;
    }
}
//
//MASK_FLAG (NOTE route in some cleaner way)
//
template<typename T, typename R>
requires (std::integral<R> || std::floating_point<R>)
std::vector<mask_ref<T>> get_true_elements(Matrix_base<T> &mxb1, const Condition condition, const R &val)
{   

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
    size_t rows{mxb1.get_rows()};
    size_t cols{mxb1.get_cols()};

    if(!matrix_math::same_dims(mxb1, mxb2)){
        throw dimension_mismatch(std::format("comparing mxb1({}x{}), with mxb2({}x{})", rows, cols, mxb2.get_rows(), mxb2.get_cols()));
    }
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

template<typename T>
mask_flag<T> Matrix<T>::M()
{
    return {*this};
}
//
//Assign
//
template<typename T>
assign_flag<T>::assign_flag(Matrix<T> &matrix) : ptr{&matrix}{}

template<typename T>
assign_flag<T> Matrix<T>::A()
{
    return {*this};
}
template<typename T>
template<typename R>
Matrix<T>&  assign_flag<T>::operator=(const Matrix_base<R> &mxb1){
    Matrix<T> copy{mxb1};
    ptr->get_matrix() = std::move(copy.matrix);
    ptr->rows = copy.rows;
    ptr->cols = copy.cols;
    return *ptr;
}
template<typename T>
Matrix<T>&  assign_flag<T>::operator=(Matrix<T> &&mx){
    ptr->get_matrix() = std::move(mx.matrix);
    ptr->rows = mx.rows;
    ptr->cols = mx.cols;
    return *ptr;
}
