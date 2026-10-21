#pragma once
#include "matrix_subtypes.hpp"
#include "matrix_method.hpp"
#include <type_traits>
#include <cassert>

template <typename T>
class Slice_view;

template<typename T>
class Mask_view;

template<typename T>
class Matrix : public Matrix_base<T>
{   
    private:
        std::vector<T> matrix{};
        size_t rows{};
        size_t cols{};
//
//CONSTRUCTORS
//  
    friend class Slice_view<T>;

    public:
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
            for(size_t r{} ; r<r1 ; r++){
                for(size_t c{} ; c<c1 ; c++){
                    (*this)(r, c) = static_cast<T>(mxb1(r, c));
                }
            }
        }
        ~Matrix()override = default;
//
//SLICE AND MASK
//
        Slice_view<T> operator()(Slice r_slice, Slice c_slice)
        {   
            r_slice.end = (r_slice.end == 0) ? rows : r_slice.end;
            c_slice.end = (c_slice.end == 0) ? cols : c_slice.end;
            return {this, r_slice, c_slice};
        }   
        Slice_view<T> operator()(Slice r_slice)
        {   
            r_slice.end = (r_slice.end == 0) ? rows : r_slice.end;
            Slice c_slice{0, cols};
            return {this, r_slice, c_slice};
        }

        Mask_view<T> operator[](Matrix<boolean> mask){
            return {*this, mask};
        }
//
// OVERRIDES
//        
        T& operator()(const size_t row, const size_t col)override{
            return matrix[row*cols + col];
        }
        
        const T& operator()(const size_t row, const size_t col)const override{
            return matrix[row*cols + col];
        }

        inline size_t get_rows()const override {return rows;}
        inline size_t get_cols()const override {return cols;}
        
        inline size_t matrix_rows()const override {return rows;}
        inline size_t matrix_cols()const override {return cols;}

//
// ASSIGN OPERATORS
// 
        Matrix& operator=(Matrix<T> other)
        {   
            rows = other.rows;
            cols = other.cols;
            std::swap(matrix, other.matrix);
            return *this;
        }

        template<typename R>
        Matrix& operator=(const Matrix_base<R> &mxb1)
        {
            Matrix<T> other{mxb1};
            std::swap(*this, other);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Matrix& operator=(R val)
        {      
            for(auto &value :  matrix){
                value = val;
            }                                                                                                                       
            return *this;
        }

        Matrix& operator=(const Slice_view<T> &slice_view){
            Matrix<T> other{slice_view};
            std::swap(*this, other);
            return *this;
        }

//
//MEMBER ARITHMETIC OPERATORS
//
        template<typename R>
        Matrix& operator+=(const Matrix_base<R>& other)
        {   
            static_assert(!std::is_same_v<T, boolean>);
            matrix_method::broadcast(*this, other, Operation::add);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Matrix& operator+=(const R val)
        {   
            matrix_method::broadcast(*this, val, Operation::add);
            return *this;
        }
        
        template<typename R>
        Matrix& operator-=(const Matrix_base<R>& other)
        {
            matrix_method::broadcast(*this, other, Operation::substract);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Matrix& operator-=(const R val)
        {
            matrix_method::broadcast(*this, val, Operation::substract);
            return *this;
        }

        template<typename R>
        Matrix& operator*=(const Matrix_base<R>& other)
        {
            matrix_method::broadcast(*this, other, Operation::multiply);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Matrix& operator*=(const R val)
        {
            matrix_method::broadcast(*this, val, Operation::multiply);
            return *this;
        }

        template<typename R>
        Matrix& operator/=(const Matrix_base<R>& other)
        {
            matrix_method::broadcast(*this, other, Operation::divide);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Matrix& operator/=(const R val)
        {
            matrix_method::broadcast(*this, val, Operation::divide);
            return *this;
        }

//
//NOT OPERATOR
//      
        template <typename U = T>
        requires std::is_same_v<U, boolean>
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
    requires std::integral<R> || std::floating_point<R>
    Matrix<boolean> make_mask(const Matrix_base<T> &mxb1, const Condition condition, const R threshold)
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
                    case(Condition::is_true): mask(r, c) = true; break;
                    case(Condition::is_false): mask(r, c) = false; break;
                    case(Condition::over): mask(r, c) = (current_element > N_threshold); break;
                    case(Condition::over_or): mask(r, c) = (current_element >= N_threshold); break;
                    case(Condition::under): mask(r, c) = (current_element < N_threshold); break;
                    case(Condition::under_or): mask(r, c) = (current_element <= N_threshold); break;
                    case(Condition::is) : mask(r, c) = (current_element == N_threshold); break;
                    case(Condition::is_not): mask(r, c) = (current_element != N_threshold); break;
                }
            }
        }
        return mask;
    }

    template<typename T, typename R>
    Matrix<boolean> make_mask(const Matrix_base<T> &mxb1, const Condition condition, const Matrix_base<R> &mxb2)
    {   
        if(mxb1.get_rows() != mxb2.get_rows()){
            std::cerr<<"make_mask row mismatch\n"; //change to throw
        }
        if(mxb1.get_cols() != mxb2.get_cols()){
            std::cerr<<"make_mask col mismatch\n"; //change to throw
        }

        size_t r1{mxb1.get_rows()};
        size_t c1{mxb1.get_cols()};
        Matrix<boolean> mask{r1, c1, boolean{false}};

        using common_type = std::common_type_t<T, R>;

        for(size_t r{} ; r<r1 ; r++){
            for(size_t c{} ; c<c1 ; c++){

                auto mxb1_current = static_cast<common_type>(mxb1(r, c));
                auto mxb2_current = static_cast<common_type>(mxb2(r, c));
                switch(condition)
                {   
                    case(Condition::is_true): mask(r, c) = true; break;
                    case(Condition::is_false): mask(r, c) = false; break;
                    case(Condition::over): mask(r, c) = (mxb1_current > mxb2_current); break;
                    case(Condition::over_or): mask(r, c) = (mxb1_current >= mxb2_current); break;
                    case(Condition::under): mask(r, c) = (mxb1_current < mxb2_current); break;
                    case(Condition::under_or): mask(r, c) = (mxb1_current <= mxb2_current); break;
                    case(Condition::is) : mask(r, c) = (mxb1_current == mxb2_current); break;
                    case(Condition::is_not): mask(r, c) = (mxb1_current != mxb2_current); break;
                }
            }
        }
        return mask;
    }
}
