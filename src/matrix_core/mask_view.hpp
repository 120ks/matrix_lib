#pragma once


#include "matrix_view.hpp"


namespace machine
{   
    template<typename T>
    class Mask_view : public Matrix_base<Mask_view<T>, T>
    {
        private:
            T* start_data;
            array<long long> index_arr;
            
            long long mask_rows{};
            long long mask_cols{};

            long long base_cols{};
            long long base_rows{};
            long long matrix_id{};
            long long generation{};

        public:
            template<typename BOOLmx>
            Mask_view(const Matrix<T> &matrix, const Matrix_base<BOOLmx, bool> &mask)
            : start_data{const_cast<T*>(matrix.begin())}
            , mask_rows{mask.get_rows()}
            , mask_cols{mask.get_cols()}
            , base_rows{matrix.get_rows()}
            , base_cols{matrix.get_cols()}
            , matrix_id{matrix.get_id()}
            , generation{lifetime_table::get_gen(matrix.get_id())}
            {   
                quick_throw::throw_if_dimension_mismatch(matrix, mask);
                const auto& mask_mx = static_cast<const BOOLmx&>(mask);
                index_arr.reserve(mask_rows*mask_cols);
                for(long long row{} ; row<mask_rows ; row++){
                    for(long long col{} ; col<mask_rows ; col++){
                        if(mask_mx(row, col) == true){
                            index_arr.push_back(row * base_cols + col);
                        }
                    }
                }
            }
            template<typename BOOLmx>
            Mask_view(const Matrix_view<T> &view, const Matrix_base<BOOLmx, bool> &mask)
            : start_data{view.start_data}
            , mask_rows{mask.get_rows()}
            , mask_cols{mask.get_cols()}
            , base_rows{view.base_rows}
            , base_cols{view.base_cols}
            , matrix_id{view.matrix_id}
            , generation{view.generation}
            {   
                quick_throw::throw_if_dimension_mismatch(view, mask);
                auto v_start_row = view.row_slice.begin;
                auto v_start_col = view.col_slice.begin;
                index_arr.reserve(mask_rows*mask_cols);
                for(long long row{} ; row<mask_rows; row++){
                    for(long long col{} ; col<mask_cols; col++){
                        if(mask(row, col) == true){
                            
                            index_arr.push_back( (v_start_row+row)*base_cols + v_start_col+col );
                        }
                    }
                }
            }

            template<typename Tmx, typename BOOLmx>
            Mask_view(Matrix_base<Tmx, T> &mxb1, const Matrix_base<BOOLmx, bool> &mask)
            : Mask_view(static_cast<Tmx&>(mxb1), mask)
            {}

            inline long long get_rows()const{return 1;} //always 1 row N cols
            inline long long get_cols()const{return index_arr.size();}

            inline T& operator()(long long row, long long col)
            {   

                return *(start_data + index_arr[col]);
            }
            inline const T& operator()(long long row, long long col)const
            {   
                return *(start_data + index_arr[col]);
            }

            Matrix<T> transpose()const
            {   
                quick_throw::throw_if_no_data(*this);
                auto rows{this->get_rows()};
                auto cols{this->get_cols()};
                Matrix<T> transposed{cols, rows};

                for(long long row{} ; row<rows ; row++){
                    for(long long col{} ; col<cols ; col++){
                        transposed(col, row) = (*this)(row, col);
                    }
                }
                return transposed;
            }

            Matrix<T> copy()const
            {   
                return {*this};
            }

            template<typename R, typename R2 = R>
            inline Matrix<R> copy_as(){
                return Matrix<R>{*this};
            }

            operator bool() const
            {   
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
            
            template<typename R>
            Mask_view<T>& operator=(const Mask_view<T> &mxv){
                broadcast(*this, mxv, Operation::equal);
                return *this;
            }

            template<typename Rmx, typename R>
            Mask_view<T>& operator=(const Matrix_base<Rmx, R> &mxb1){   
                if(rows_and_cols_are(mxb1, mask_rows, mask_cols)){
                    Matrix_view<T> mxv{mxb1};
                    auto* start = mxv.start_data;

                    for(long long &index : index_arr){
                        *(start_data + index) = *(start + index);
                    }
                    return *this;
                }

                broadcast(*this, mxb1, Operation::equal);
                return *this;
            }

            template<typename R>
            requires (std::integral<R> || std::floating_point<R>)
            Mask_view<T>& operator=(const R val){
                broadcast(*this, val, Operation::equal);
                return *this;
            }

        //
        //MEMBER ARITHMETIC OPERATORS
        //      
        template<typename Rmx, typename R>
        Mask_view<T>& operator+=(const Matrix_base<Rmx, R>& other){
            broadcast(*this, other, Operation::add);
            return *this;
        }
        
        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Mask_view<T>& operator+=(const R val){
            broadcast(*this, val, Operation::add);
            return *this;
        }

        template<typename Rmx, typename R>
        Mask_view<T>& operator-=(const Matrix_base<Rmx, R>& other){
            broadcast(*this, other, Operation::substract);
            return *this;
        }

        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Mask_view<T>& operator-=(const R val){
            broadcast(*this, val, Operation::substract);
            return *this;
        }
        template<typename Rmx, typename R>
        Mask_view<T>& operator*=(const Matrix_base<Rmx, R>& other){
            broadcast(*this, other, Operation::multiply);
            return *this;
        }

        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Mask_view<T>& operator*=(const R val){
            broadcast(*this, val, Operation::multiply); 
            return *this;
        }

        template<typename Rmx, typename R>
        Mask_view<T>& operator/=(const Matrix_base<Rmx, R>& other){
            broadcast(*this, other, Operation::divide); 
            return *this;
        }

        template<typename R>
        requires (std::integral<R> || std::floating_point<R>)
        Mask_view<T>& operator/=(const R val){
            broadcast(*this, val, Operation::divide);
            return *this;
        }
    };
    template<typename T>
    template<typename BOOLmx>
    Mask_view<T> Matrix<T>::operator[](const Matrix_base<BOOLmx, bool> &mask){
        return {*this, mask};
    }
}
