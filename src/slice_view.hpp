#pragma once
#include "matrix_base.hpp"
#include "matrix_base.hpp"
#include "matrix_subtypes.hpp"

template<typename T>
class Slice_view : public Matrix_base<T>
{   
    template<typename>
    friend class Matrix;

    friend class mask_flag<T>;

    private:
        Matrix<T>* view_ptr{};
        Slice row_slice{};
        Slice col_slice{};
//
//CONSTRUCTORS
//
        Slice_view(Matrix<T> *matrix, Slice r_slice, Slice c_slice)
        : view_ptr{matrix}
        , row_slice{r_slice}
        , col_slice{c_slice}
        {
            row_slice.end = (row_slice.end == 0) ? matrix->get_rows() : row_slice.end; 
            col_slice.end = (col_slice.end == 0) ? matrix->get_cols() : col_slice.end; 
        }
        
        Slice_view(const Slice_view &other)
        : view_ptr{other.view_ptr}
        , row_slice{other.row_slice}
        , col_slice{other.col_slice}
        {}

        Slice_view(Slice_view &&other)noexcept
        : view_ptr{other.view_ptr}
        , row_slice{other.row_slice}
        , col_slice{other.col_slice}
        {other.view_ptr = nullptr;}

    public:
        ~Slice_view()override = default;
        Slice_view& operator=(const Slice_view &other) = delete;
        Slice_view& operator=(Slice_view&& other) = delete;
//  
// OVERRIDES
//        
        inline size_t get_rows()const override {return row_slice.end - row_slice.begin;}
        inline size_t get_cols()const override {return col_slice.end - col_slice.begin;}

        inline T& operator()(const size_t row, const size_t col)override
        {   
            if((row >= this->get_rows()) || (col >= this->get_cols())){
                throw indexing_mismatch(std::format("index ({}, {}), matrix({}x{})", row, col, this->get_rows(), this->get_cols()));
            }
            return (*view_ptr)(row+row_slice.begin, col+col_slice.begin);
        }

        inline const T& operator()(const size_t row, const size_t col)const override
        {   
            if((row >= this->get_rows()) || (col >= this->get_cols())){
                throw indexing_mismatch(std::format("index ({}, {}), matrix({}x{})", row, col, this->get_rows(), this->get_cols()));
            }
            return (*view_ptr)(row+row_slice.begin, col+col_slice.begin);
        }

        Matrix<T> transpose()const override
        {   
            auto rows{this->get_rows()};
            auto cols{this->get_cols()};
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
            return {*this};
        }
//
//SLICING AND MASKING
//      
        Slice_view& operator()(const Slice r_slice, const Slice c_slice)
        {   
            if((r_slice.end>this->get_rows()) || (c_slice.end>this->get_cols()))
            {
                throw slicing_mismatch(std::format("sl({}, {}) sl({}, {}) view({}x{})", r_slice.begin, r_slice.end, c_slice.begin, c_slice.end , this->get_rows(), this->get_cols()));
            }

            row_slice = row_slice+r_slice;
            col_slice = col_slice+c_slice;

            return{*this};
        }

        Slice_view<T>& operator()(const Slice r_slice)
        {   if(r_slice.end>this->get_rows())
            throw slicing_mismatch(std::format("row_sl({}, {}) view({}x{})", r_slice.begin, r_slice.end, this->get_rows()));
            row_slice = row_slice+r_slice;
            return {*this};
        }

        Matrix<mask_ref<T>> operator[](const Matrix<boolean> &mask){ //note the  []
            return{*this, mask};
        }
//
// ASSIGN OPERATORS
//  
        template<typename R>
        Slice_view& operator=(const Matrix_base<R> &mxb1)
        {
            method::broadcast(*this, mxb1, Operation::equal);
            return *this;
        }

        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Slice_view& operator=(const R val)
        {
            method::broadcast(*this, val, Operation::equal);
            return *this;
        }

        template<typename R>
        Slice_view& operator=(const Slice_view<R> &mxv1)
        {
            method::broadcast(*this, mxv1, Operation::equal);
            return *this;
        }
//
//FLAGS
//          
        mask_flag<T> M();
//
//MEMBER ARITHMETIC OPERATORS
//      
        template<typename R>
        Slice_view& operator+=(const Matrix_base<R>& other)
        {
            method::broadcast(*this, other, Operation::add);
            return *this;
        }
        
        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Slice_view& operator+=(const R val)
        {
            method::broadcast(*this, val, Operation::add);
            return *this;
        }

        template<typename R>
        Slice_view& operator-=(const Matrix_base<R>& other)
        {
            method::broadcast(*this, other, Operation::substract);
            return *this;
        }

        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Slice_view& operator-=(const R val)
        {
            method::broadcast(*this, val, Operation::substract);
            return *this;
        }
        template<typename R>
        Slice_view& operator*=(const Matrix_base<R>& other)
        {
            method::broadcast(*this, other, Operation::multiply);
            return *this;
        }

        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Slice_view& operator*=(const R val)
        {
            method::broadcast(*this, val, Operation::multiply);
            return *this;
        }

        template<typename R>
        Slice_view& operator/=(const Matrix_base<R>& other)
        {
            method::broadcast(*this, other, Operation::divide);
            return *this;
        }

        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Slice_view& operator/=(const R val)
        {
            method::broadcast(*this, val, Operation::divide);
            return *this;
        }

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
//FLAG FUNCTIONS FOR CIRCULAR DEPENCY
//

template<typename T>
mask_flag<T> Slice_view<T>::M(){
        return {*this};
    }
