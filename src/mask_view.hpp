#pragma once
#include "matrix_base.hpp"
template<typename T>
class Mask_view : public Matrix_base<T>
{   
    friend class Matrix<T>;
    friend class Matrix_view<T>;

    private:
        T* mask_ptr{};
        std::vector<size_t>index_array{};
        size_t base_rows{};
        size_t base_cols{};

        Mask_view(const Matrix_base<T>& mxb1, const Matrix<boolean> &mask)
        : mask_ptr{const_cast<T*>(mxb1.start_ptr())}
        , index_array{}
        , base_rows{mxb1.matrix_rows()}
        , base_cols{mxb1.matrix_cols()}
        {   
            auto rows{mask.get_rows()};
            auto cols{mask.get_cols()};
            index_array.reserve(rows*cols);

            assert(rows == mxb1.get_rows());
            assert(cols == mxb1.get_cols());

            for(size_t r{} ; r<rows ; r++){
                for(size_t c{} ; c<cols ; c++)
                {
                    if(mask(r, c) == true){
                        index_array.push_back(r*base_cols + c);
                    }
                }
            }
        }

        Mask_view(Mask_view<T> &&other)
        : mask_ptr{other.mask_ptr}
        , index_array{std::move(other.index_array)}
        , base_rows{other.base_rows}
        , base_cols{other.base_cols}
        {
            other.mask_ptr = nullptr;
        }

        Mask_view(const Mask_view<T> &other)
        : mask_ptr{other.mask_ptr}
        , index_array{other.mask_ptr}
        , base_rows{other.base_rows}
        , base_cols{other.base_cols}
        {}
//
//OVERRIDES
//
    public:
        T& operator()(const size_t row, const size_t col)override{
            return *(mask_ptr + index_array[row+col]);
        }
        T operator()(const size_t row , const size_t col) const override{
            return  *(mask_ptr + index_array[row+col]);
        }

        inline size_t get_rows()const override {return index_array.size();}
        inline size_t get_cols()const override {return 1;};
        inline size_t matrix_rows()const override {return base_rows;}
        inline size_t matrix_cols()const override {return base_cols;}
        Type derived_type()const override {return Type::mask;}

        inline T* start_ptr()const override {return mask_ptr;}
//
// ASSIGN OPERATORS
//      
        template<typename R>
        Mask_view& operator=(const Matrix_base<R> &mxb1)
        {
            matrix_method::broadcast(*this, mxb1, Operation::equal);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Mask_view& operator=(const R val)
        {
            matrix_method::broadcast(*this, val, Operation::equal);
            return *this;
        }
//
//MEMBER ARITHMETIC OPERATORS
//      
        template<typename R>
        Mask_view& operator+=(const Matrix_base<R>& other)
        {
            matrix_method::broadcast(*this, other, Operation::add);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Mask_view& operator+=(const R val)
        {
            matrix_method::broadcast(*this, val, Operation::add);
            return *this;
        }

        template<typename R>
        Mask_view& operator-=(const Matrix_base<R>& other)
        {
            matrix_method::broadcast(*this, other, Operation::substract);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Mask_view& operator-=(const R val)
        {
            matrix_method::broadcast(*this, val, Operation::substract);
            return *this;
        }

        template<typename R>
        Mask_view& operator*=(const Matrix_base<R>& other)
        {
            matrix_method::broadcast(*this, other, Operation::multiply);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Mask_view& operator*=(const R val)
        {
            matrix_method::broadcast(*this, val, Operation::multiply);
            return *this;
        }

        template<typename R>
        Mask_view& operator/=(const Matrix_base<R>& other)
        {
            matrix_method::broadcast(*this, other, Operation::divide);
            return *this;
        }

        template<typename R>
        requires std::integral<R> || std::floating_point<R>
        Mask_view&operator/=(const T val)
        {
            matrix_method::broadcast(*this, val, Operation::divide);
            return *this;
        }
};

