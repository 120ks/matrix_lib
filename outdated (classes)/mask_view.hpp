#pragma once
#include "matrix_base.hpp"

template<typename T>
class Mask_view : public Matrix_base<T>
{   
    friend class Matrix<T>;
    friend class Slice_view<T>;
//
//constructors
//
    private:
        size_t true_elements{};
        size_t shift_begin{};
        std::vector<T*>true_array{};

        Mask_view(Matrix_base<T>& mxb1, const Matrix_base<boolean> &mask)
        : shift_begin{0}
        , true_array{}
        {   
            auto rows{mask.get_rows()};
            auto cols{mask.get_cols()};
            true_array.reserve(rows*cols);

            auto base_rows{mxb1.matrix_rows()};
            auto base_cols{mxb1.matrix_cols()};

            assert(rows == mxb1.get_rows());
            assert(cols == mxb1.get_cols());


            for(size_t r{} ; r<rows ; r++){
                for(size_t c{} ; c<cols ; c++)
                {
                    if(mask(r, c) == true){
                        true_array.push_back(&mxb1(r, c));
                    }
                }
            }
            true_elements = true_array.size();
        }

        Mask_view(Mask_view<T> &&other)
        : true_elements{other.true_elements}
        , shift_begin{other.shift_begin}
        , true_array{std::move(other.true_array)}
        {
        }

        Mask_view(const Mask_view<T> &other)
        : true_elements{other.true_elements}
        , shift_begin{0}
        , true_array{}
        {
            true_array.reserve(true_elements);
            for(size_t t_element{} ; t_element < true_elements ; t_element++){
                true_array.push_back(other.true_array[t_element + other.shift_begin]);
            }
            assert(true_elements == true_array.size());
        }
//
//OVERRIDES
//
    public:
        T& operator()(const size_t row, const size_t col)override{
            assert(col < true_elements);
            return *true_array[shift_begin+col];
        }
        const T& operator()(const size_t row , const size_t col) const override{
            assert(col < true_elements);
            return  *true_array[shift_begin+col];
        }

        inline size_t get_rows()const override {return 1;}
        inline size_t get_cols()const override {return true_elements;};
        inline size_t matrix_rows()const override {return 1;} //if further indexed, will use the index array
        inline size_t matrix_cols()const override {return true_elements;} //if further sliced, masked etc, will use the index array

//
//SLICING AND MASKING
//      
        Mask_view& operator()(const Slice &sl1) //slicing modifies the mask_view
        {
            assert(sl1.end <= true_elements); //MAKE THROW

            size_t end = (sl1.end == 0) ? true_elements : sl1.end;
            true_elements = end-sl1.begin;
            shift_begin = sl1.begin;

            return this;
        }

        Mask_view<T> operator[](Matrix<boolean> mask){ //note the  []
            return{*this, mask};
        }
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
        Mask_view&operator/=(const R val)
        {
            matrix_method::broadcast(*this, val, Operation::divide);
            return *this;
        }

//
//FRIEND FUNCTIONS
//




};

