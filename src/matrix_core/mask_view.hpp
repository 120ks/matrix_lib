#pragma once


#include "matrix_view.hpp"


namespace machx
{   
    template<typename T>
    class Mask_view : public Matrix_base<Mask_view<T>, T>
    {
        private:
            template<typename, typename>
            friend class Matrix_base;


            array<T*> ptr_arr{};
            long long rows{};
            long long cols{};
        public:
            template<typename Tmx, typename BOOLmx>
            Mask_view(Matrix_base<Tmx, T> &mxb1, const Matrix_base<BOOLmx, bool> &mask)
            : ptr_arr{}
            {   
                auto mask_rows = mask.get_rows();
                auto mask_cols = mask.get_cols();
                auto mxb1_rows = mxb1.get_rows();
                auto mxb1_cols = mxb1.get_cols();
                ptr_arr.reserve(rows*cols);

                //ROW/COL MASK
                if(mask_cols == 1 && mask_rows == mxb1_rows){
                    this->cols = mxb1_cols;
                    for(long long row{} ; row < mask_rows ; row++){
                        if(mask(row, 0) == true){
                            this->rows++;
                            for(long long col{} ; col<mxb1_cols ; col++){
                                ptr_arr.push_back(&mxb1(row, col));
                            }
                        }
                    }
                    return;
                }
                else if(mask_rows == 1 && mask_cols == mxb1_cols){
                    this->rows = mxb1_rows;
                    for(long long row{} ; row < mxb1_rows ; row++){
                        for(long long col{} ; col<mask_cols ; col++){
                            if(mask(0, col) == true){
                                ptr_arr.push_back(&mxb1(row, col));
                                if(row==0){
                                    this->cols++;
                                }
                            }
                        }
                    }
                    return;
                }
                this->rows = 1;
                for(long long row{} ; row < mask_rows ; row++){
                    for(long long col{} ; col<mask_cols ; col++){
                        if(mask(row, col) == true){
                            ptr_arr.push_back(&mxb1(row, col));
                        }
                    }
                }
                this->cols = ptr_arr.size();
            }
            template<typename Tmx, typename BOOLmx>
            Mask_view(const Matrix_base<Tmx, T> &mxb1, const Matrix_base<BOOLmx, bool> &mask)
            : Mask_view(const_cast<Matrix_base<Tmx, T>&>(mxb1), mask)
            {}

            Mask_view(const machx::array<T*> &arr)
            : ptr_arr{arr}
            , rows{1}
            , cols{}
            {
                this->cols = ptr_arr.size();
            }
            Mask_view(machx::array<T*>&& arr)
            : ptr_arr{std::move(arr)}
            , rows{1}
            , cols{}
            {
                this->cols = ptr_arr.size();
            }

            template<typename Tmx, typename BOOLmx>
            Mask_view(Matrix_base<Tmx, T> &&mxb1, const Matrix_base<BOOLmx, bool> &mask)
            : Mask_view(mxb1, mask)
            {}
            Mask_view(const Mask_view<T> &other)
            : ptr_arr{other.ptr_arr}
            , rows{other.rows}
            , cols{other.cols}
            {}
            Mask_view(Mask_view<T> &&other)
            : ptr_arr{std::move(other.ptr_arr)}
            , rows{other.rows}
            , cols{other.cols}
            {}

            inline long long get_rows()const{return rows;}
            inline long long get_cols()const{return cols;}

            inline T& operator()(long long row, long long col){
                row = (row<0)? rows+row : row;
                col = (col<0)? cols+col : col;
                return *(ptr_arr[row * cols + col]);
            }
            inline const T& operator()(long long row, long long col)const{   
                row = (row<0)? rows+row : row;
                col = (col<0)? cols+col : col;
                return *(ptr_arr[row * cols + col]);
            }
            //MASK
            using Matrix_base<Mask_view<T>, T>::operator[];

            //SLICING
            Mask_view<T> operator()(Slice row_slice, Slice col_slice){
                Matrix<bool> mask{this->get_rows(), this->get_cols(), false};
                mask(row_slice, col_slice) = true;
                return {*this, mask};
            }
            Mask_view<T> operator()(Slice row_slice){
                Matrix<bool> mask{this->get_rows(), this->get_cols(), false};
                mask(row_slice) = true;
                return {*this, mask}; 
            }
            Mask_view<T> operator[](Slice col_slice){
                Matrix<bool> mask{this->get_rows(), this->get_cols(), false};
                mask[col_slice] = true;
                return {*this, mask};
            }
            //CONST SLICING
            const Mask_view<T> operator()(Slice row_slice, Slice col_slice)const{
                Matrix<bool> mask{this->get_rows(), this->get_cols(), false};
                mask(row_slice) = true;
                return {*this, mask};
            }
            const Mask_view<T> operator()(Slice row_slice)const{
                Matrix<bool> mask{this->get_rows(), this->get_cols(), false};
                mask(row_slice)= true;
                return {*this, mask};
            }
            const Mask_view<T> operator[](Slice col_slice)const{
                Matrix<bool> mask{this->get_rows(), this->get_cols(), false};
                mask[col_slice] = true;
                return {*this, mask};
            }

            T* begin(){return nullptr;}
            T* end(){return nullptr;}
            const T* begin()const{return nullptr;}
            const T* end()const{return nullptr;}
            //
            //ASSIGNS
            //
            Mask_view<T>& operator=(const Mask_view<T> &mxv){
                broadcast(*this, mxv, Operation::equal);
                return *this;
            }

            Mask_view<T>& operator=(Mask_view<T> &&mxv)noexcept{
                broadcast(*this, mxv, Operation::equal);
                return *this;
            }
            using Matrix_base<Mask_view<T>, T>::operator=;

            operator bool()const{ //empty is valid with mask so always valid
                return true;
            }

            bool empty()const{
                if(ptr_arr.size() == 0){
                    return true;
                }
                return false;
            }
        
        private:
            //BASE EDITING FUNCTIONS
            T*& get_ptr_at(long long row, long long col){
                return ptr_arr[row * cols + col];
            }
    };
    
}
