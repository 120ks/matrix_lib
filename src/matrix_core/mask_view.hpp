#pragma once


#include "matrix_view.hpp"


namespace machine
{   
    template<typename T>
    class Mask_view : public Matrix_base<Mask_view<T>, T>
    {
        private:

            array<T*> ptr_arr{};
            long long matrix_id{};
            long long generation{};

        public:
            template<typename Tmx, typename BOOLmx>
            Mask_view(Matrix_base<Tmx, T> &mxb1, const Matrix_base<BOOLmx, bool> &mask)
            : ptr_arr{}
            , matrix_id{mxb1.get_id()}
            , generation{mxb1.get_gen()}
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
            template<typename Tmx, typename BOOLmx>
            Mask_view(Matrix_base<Tmx, T> &&mxb1, const Matrix_base<BOOLmx, bool> &mask)
            : Mask_view(mxb1, mask)
            {}

            inline long long get_rows()const{return 1;} //always 1 row N cols
            inline long long get_cols()const{return ptr_arr.size();}

            inline T& operator()(long long row, long long col){
                return *(ptr_arr[col]); //row always = 0, cols = size
            }
            inline const T& operator()(long long row, long long col)const{   
                return *(ptr_arr[col]);
            }

            Matrix<T> copy()const{   
                return {*this};
            }

            template<typename R>
            inline Matrix<R> copy_as(){
                return Matrix<R>{*this};
            }

            operator bool() const{   
                auto current_gen {lifetime_table::get_gen(matrix_id)};
                return (current_gen == generation) && (current_gen>0);
            }

            inline long long get_id()const{return matrix_id;}
            inline long long get_gen()const{return generation;}

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

            Mask_view<T>& operator=(Mask_view<T> &&mxv){
                broadcast(*this, mxv, Operation::equal);
                return *this;
            }
            using Matrix_base<Mask_view<T>, T>::operator=;
    };
    template<typename T>
    template<typename BOOLmx>
    Mask_view<T> Matrix<T>::operator[](const Matrix_base<BOOLmx, bool> &mask){
        return {*this, mask};
    }
}
