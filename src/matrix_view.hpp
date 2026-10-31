#pragma once
#include "matrix_base.hpp"
#include "matrix_subtypes.hpp"


namespace machine
{



template<typename T>
class Matrix_view : public Matrix_base<T>
{   
    template<typename>
    friend class Matrix;

    friend class mask_flag<T>;
    friend class view_assign_flag<T>;
    friend class expression_lock_view<T>;

    private:
        Matrix<T> *view_ptr{};
        Slice row_slice{};
        Slice col_slice{};
        size_t matrix_id{};
        long long generation{};
//
//CONSTRUCTORS
//
    public:
//
//FLAGS
//          
        mask_flag<T> M();
        view_assign_flag<T> A();
        expression_lock_view<T> E();

        Matrix_view(Matrix<T> &matrix)
        : view_ptr{&matrix}
        , row_slice{0, matrix.get_rows()}
        , col_slice{0, matrix.get_cols()}
        , matrix_id{matrix.id}
        , generation{lifetime_table::get_gen(matrix.id)}
        {
            quick_throw::throw_if_no_data(matrix, " binding view to empty data");
        }
        Matrix_view(Matrix<T> &matrix, Slice r_slice, Slice c_slice)
        : view_ptr{&matrix}
        , row_slice{r_slice}
        , col_slice{c_slice}
        , matrix_id{matrix.id}
        , generation{lifetime_table::get_gen(matrix.id)}
        {
            quick_throw::throw_if_no_data(matrix, " binding view to empty data");
            row_slice.end = (row_slice.end == 0) ? matrix.get_rows() : row_slice.end; 
            col_slice.end = (col_slice.end == 0) ? matrix.get_cols() : col_slice.end; 
            
        }
        
        Matrix_view(const Matrix_view &other)
        : view_ptr{other.view_ptr}
        , row_slice{other.row_slice}
        , col_slice{other.col_slice}
        , matrix_id{other.matrix_id}
        , generation{other.generation}
        {}

        Matrix_view(Matrix_view &&other)noexcept
        : view_ptr{other.view_ptr}
        , row_slice{other.row_slice}
        , col_slice{other.col_slice}
        , matrix_id{other.matrix_id}
        , generation{other.generation}
        {other.view_ptr = nullptr;}

    
        ~Matrix_view()override = default;
//  
// OVERRIDES
//        
        inline size_t get_rows()const override {return row_slice.end - row_slice.begin;}
        inline size_t get_cols()const override {return col_slice.end - col_slice.begin;}

        inline T& operator()(const size_t row, const size_t col)override
        {   
            return (*view_ptr)(row+row_slice.begin, col+col_slice.begin);
        }
        inline const T& operator()(const size_t row, const size_t col)const override
        {   
            return (*view_ptr)(row+row_slice.begin, col+col_slice.begin);
        }
        Matrix<T> transpose()const override
        {   
            quick_throw::throw_if_no_data(*this);
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
        operator bool() const
        {   
            auto current_gen {lifetime_table::get_gen(matrix_id)};
            return (current_gen == generation) && (current_gen>0);
        }
        inline size_t get_id()const{return matrix_id;}

        long long get_gen()const{return generation;}

        std::mutex& get_mutex() override {return view_ptr->mxt;}

//
//SLICING AND MASKING
//      
        Matrix_view<T> operator()(const Slice r_slice, const Slice c_slice)
        {   
            quick_throw::throw_if_no_data(*this);
            quick_throw::throw_if_slice_mismatch(*this, r_slice, c_slice);
            return{*view_ptr, row_slice+r_slice, col_slice+c_slice};
        }

        Matrix_view<T> operator()(const Slice r_slice)
        {   
            quick_throw::throw_if_no_data(*this);
            quick_throw::throw_if_slice_mismatch(*this, r_slice, Slice{}, "row slicing");
            return {*view_ptr, row_slice+r_slice, col_slice};
        }

        Matrix<mask_ref<T>> operator[](const Matrix<boolean> &mask){ //note the  []
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

        template<typename R>
        Matrix_view& operator=(const Matrix_base<R> &mxb1)
        {   
            broadcast(*this, mxb1, Operation::equal);
            return *this;
        }

        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Matrix_view& operator=(const R val)
        {
            broadcast(*this, val, Operation::equal);
            return *this;
        }

//
//MEMBER ARITHMETIC OPERATORS
//      
        template<typename R>
        Matrix_view& operator+=(const Matrix_base<R>& other)
        {
            broadcast(*this, other, Operation::add);
            return *this;
        }
        
        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Matrix_view& operator+=(const R val)
        {
            broadcast(*this, val, Operation::add);
            return *this;
        }

        template<typename R>
        Matrix_view& operator-=(const Matrix_base<R>& other)
        {
            broadcast(*this, other, Operation::substract);
            return *this;
        }

        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Matrix_view& operator-=(const R val)
        {
            broadcast(*this, val, Operation::substract);
            return *this;
        }
        template<typename R>
        Matrix_view& operator*=(const Matrix_base<R>& other)
        {
            broadcast(*this, other, Operation::multiply);
            return *this;
        }

        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Matrix_view& operator*=(const R val)
        {
            broadcast(*this, val, Operation::multiply);
            return *this;
        }

        template<typename R>
        Matrix_view& operator/=(const Matrix_base<R>& other)
        {
            broadcast(*this, other, Operation::divide);
            return *this;
        }

        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Matrix_view& operator/=(const R val)
        {
            broadcast(*this, val, Operation::divide);
            return *this;
        }
//
//not operator
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

//
//VIEW SPECIFIC
//
        void reset_view()
        {
            row_slice.begin = 0;
            row_slice.end = view_ptr->get_rows();
            col_slice.begin = 0;
            col_slice.end = view_ptr->get_cols();

            if(*view_ptr){
                generation = lifetime_table::get_gen(matrix_id);
                assert(generation>0); //DEBUG
            }
        }

        void force_update()
        {
            if(*view_ptr){
                generation = lifetime_table::get_gen(matrix_id);
                assert(generation>0); //DEBUG
            }
        }

        Matrix_view& change_pointer(Matrix<T> &matrix)
        {   
            quick_throw::throw_if_no_data(matrix, "cant view a matrix with no data");
            view_ptr = &matrix;
            matrix_id = matrix.id;
            reset_view();
            return *this;
        }
};//class end
}
//
//FLAG FUNCTIONS
//
namespace machine
{
    template<typename T>
    mask_flag<T> Matrix_view<T>::M(){
        return {*this};
    }

    template<typename T>
    view_assign_flag<T> Matrix_view<T>::A()
    {
        return {*this};
    }
    template<typename T>
    expression_lock_view<T> Matrix_view<T>::E()
    {
        return {*this};
    }

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

    template<typename T>
    expression_lock_view<T>::expression_lock_view(Matrix_view<T> &mxv)
    : ptr{&mxv}
    {
        ptr->view_ptr->get_mutex().lock();
    }

    template<typename T>
    expression_lock_view<T>::~expression_lock_view()
    {
        ptr->view_ptr->get_mutex().unlock();
    }
}
