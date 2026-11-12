#pragma once


#include "matrix_view.hpp"


namespace machx
{   
    template<typename T>
    class Mask_view : public Matrix_base<Mask_view<T>, T>
    {
        private:

            array<T*> ptr_arr{};

        public:
            template<typename Tmx, typename BOOLmx>
            Mask_view(Matrix_base<Tmx, T> &mxb1, const Matrix_base<BOOLmx, bool> &mask)
            : ptr_arr{}
            {   
                quick_throw::throw_if_dimension_mismatch(mxb1, mask);
                auto rows = mask.get_rows();
                auto cols = mask.get_cols();
                ptr_arr.reserve(rows*cols);

                for(long long row{} ; row < rows ; row++){
                    for(long long col{} ; col<cols ; col++){
                        if(mask(row, col) == true){
                            ptr_arr.push_back(&mxb1(row, col));
                        }
                    }
                }
            }
            Mask_view(const machx::array<T*> &arr)
            : ptr_arr{arr}
            {}
            Mask_view(machx::array<T*>&& arr)
            : ptr_arr{std::move(arr)}
            {}

            template<typename Tmx, typename BOOLmx>
            Mask_view(Matrix_base<Tmx, T> &&mxb1, const Matrix_base<BOOLmx, bool> &mask)
            : Mask_view(mxb1, mask)
            {}

            Mask_view(const Mask_view<T> &other)
            : ptr_arr{other.ptr_arr}
            {}
            Mask_view(Mask_view<T> &&other)
            : ptr_arr{std::move(other.ptr_arr)}
            {}

            inline long long get_rows()const{return 1;} //always 1 row N cols
            inline long long get_cols()const{return ptr_arr.size();}

            inline T& operator()(long long row, long long col){
                col = (col<0)? ptr_arr.size()+col : col;
                return *(ptr_arr[col]); //row always = 0, cols = size
            }
            inline const T& operator()(long long row, long long col)const{   
                col = (col<0)? ptr_arr.size()+col : col;
                return *(ptr_arr[col]);
            }

            inline T& operator()(long long col){
                col = (col<0)? ptr_arr.size()+col : col;
                return *(ptr_arr[col]); //row always = 0, cols = size
            }
            inline const T& operator()(long long col)const{   
                col = (col<0)? ptr_arr.size()+col : col;
                return *(ptr_arr[col]);
            }

            template<typename BOOLmx>
            Mask_view<T> operator[](const Matrix_base<BOOLmx, bool> &mask){
                quick_throw::throw_if_dimension_mismatch(*this, mask, "mask does not match dims of mxb");
                
                return {*this, mask};
            }

            template<typename R>
            inline Matrix<R> copy_as(){
                return Matrix<R>{*this};
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
            //
            //FLAGS
            //
            mask_flag<Mask_view<T>, T> M(){
                return {*this};
            }

            operator bool()const{ //empty is valid with mask so always valid
                return true;
            }

    };
    template<typename T>
    template<typename BOOLmx>
    Mask_view<T> Matrix<T>::operator[](const Matrix_base<BOOLmx, bool> &mask){
        return {*this, mask};
    }
}
