#pragma once

#include <random>
#include <mutex>
#include <cassert>
#include "matrix_subtypes.hpp"
#include <type_traits>
#include <functional>

namespace machx
{   

    template<typename>
    class Matrix;

    template<typename>
    class Matrix_view;

    template<typename>
    class Mask_view;

    template<typename, typename>
    class Matrix_base;

    template<typename, typename>
    class mask_flag;

    template<typename Tmx, typename T>
    Matrix<T> transpose(const Matrix_base<Tmx, T>&);

    template<typename Tmx, typename T>
    class Matrix_base
    {   
    protected:
        Matrix_base() = default;
        virtual ~Matrix_base() = default;
    public:
        //
        //INDEXING
        //
        inline T& operator()(long long row, long long col){
            auto& mx = static_cast<Tmx&>(*this);
            return mx(row, col);
        }
        inline const T& operator()(long long row, long long col)const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx(row, col);
        }
        //
        //ROWS AND COLS
        //
        inline long long get_rows()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx.get_rows();
        }
        inline long long get_cols()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx.get_cols();
        }
        //
        //DATA MANAGEMENT
        //
        inline operator bool()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx;
        }
        inline long long get_id()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx.get_id();
        };
        inline long long get_gen()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx.get_gen();
        }

        //SLICING
        inline auto operator()(Slice row_slice, Slice col_slice){
            auto& mx = static_cast<Tmx&>(*this);
            return mx(row_slice, col_slice);
        }
        inline auto operator()(Slice row_slice){
            auto& mx = static_cast<Tmx&>(*this);
            return mx(row_slice);
        }
        inline auto operator[](Slice col_slice){
            auto& mx = static_cast<Tmx&>(*this);
            return mx[col_slice];
        }
        //CONST SLICING
        inline const auto operator()(Slice row_slice, Slice col_slice)const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx(row_slice, col_slice);
        }
        inline const auto operator()(Slice row_slice)const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx(row_slice);
        }
        inline const auto operator[](Slice col_slice)const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx[col_slice];
        }
        
        //AT FUNCTIONS
        inline auto rows_at(long long start, long long end){
            auto& mx = static_cast<Tmx&>(*this);
            return mx(Slice{start, end});
        }
        inline auto cols_at(long long start, long long end){
            auto& mx = static_cast<Tmx&>(*this);
            return mx[Slice{start, end}];
        }
        inline auto rows_at(long long start){
            auto& mx = static_cast<Tmx&>(*this);
            return mx(Slice{start, start+1});
        }
        inline auto cols_at(long long start){
            auto& mx = static_cast<Tmx&>(*this);
            return mx[Slice{start, start+1}];
        }

        //AT FUNCTIONS CONST
        inline const auto rows_at(long long start, long long end)const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx(Slice{start, end});
        }
        inline const auto cols_at(long long start, long long end)const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx[Slice{start, end}];
        }
        inline const auto rows_at(long long start)const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx(Slice{start, start+1});
        }
        inline const auto cols_at(long long start)const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx[Slice{start, start+1}];
        }

        //MASK
        template<typename BOOLmx>
        Mask_view<T> operator[](const Matrix_base<BOOLmx, bool> &mask){
            return {*this, mask};
        }
        //CONST MASK
        template<typename BOOLmx>
        const Mask_view<T> operator[](const Matrix_base<BOOLmx, bool> &mask)const{
            return {*this, mask};
        }

        //BEGIN END
        inline T* begin(){
            auto& mx = static_cast<Tmx&>(*this);
            return mx.begin();
        }
        inline T* end(){
            auto& mx = static_cast<Tmx&>(*this);
            return mx.end();
        }
        inline const T * begin()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx.begin();
        }
        inline const T* end()const{
            const auto& mx = static_cast<const Tmx&>(*this);
            return mx.end();
        }
        //
        //COPIES
        //
        Matrix<T> copy()const{
            return {*this};
        }

        template<typename R>
        inline Matrix<R> copy_as()const{
            return Matrix<R>{*this};
        }
        //
        //ASSIGN
        //
        template<typename R>
        Tmx& operator=(const R &val){
            broadcast(*this, val, Operation::equal);
            return static_cast<Tmx&>(*this);
        }
        //
        //ARITHMETIC
        //
        template<typename R>
        Tmx& operator+=(const R &val){
            broadcast(*this, val, Operation::add);
            return static_cast<Tmx&>(*this);
        }
        template<typename R>
        Tmx& operator-=(const R &val){
            broadcast(*this, val, Operation::substract);
            return static_cast<Tmx&>(*this);
        }
        template<typename R>
        Tmx& operator*=(const R &val){
            broadcast(*this, val, Operation::multiply);
            return static_cast<Tmx&>(*this);
        }
        template<typename R>
        Tmx& operator/=(const R &val){
            broadcast(*this, val, Operation::divide);
            return static_cast<Tmx&>(*this);
        }
        //
        //DATA MANIP
        //
        Matrix<T> transpose()const;
        Tmx& sort();

        Tmx& reverse_rows()const;
        Tmx& reverse_cols()const;

        Matrix<T> flatten()const;

        mask_flag<Tmx, T> M();
        const mask_flag<Tmx, T> M()const;

        mask_flag<Tmx, T> RMA();
        mask_flag<Tmx, T> CMA();
        const mask_flag<Tmx, T> RMA()const;
        const mask_flag<Tmx, T> CMA()const;

        mask_flag<Tmx, T> RMO();
        mask_flag<Tmx, T> CMO();
        const mask_flag<Tmx, T> RMO()const;
        const mask_flag<Tmx, T> CMO()const;

        //DATA OPERATIONS DELEGATIONS
        template<typename Rmx, typename R>
        Matrix<T> stack(const Matrix_base<Rmx, R> &mxb2)const;

        template<typename Rmx, typename R>
        Matrix<T> stack_side(const Matrix_base<Rmx, R> &mxb2)const;

        template<typename R>
        Matrix<T> concat_rows(std::initializer_list<R> indexes)const;
        template<typename R>
        Matrix<T> concat_cols(std::initializer_list<R> indexes)const;
        template<typename R>
        Mask_view<T> ref_rows(std::initializer_list<R> indexes);
        template<typename R>
        Mask_view<T> ref_cols(std::initializer_list<R> indexes);

        Matrix<T> shift_rows(long long)const;
        Matrix<T> shift_cols(long long)const;
        
        template<typename R = T>
        requires std::is_same_v<R, bool>
        Matrix<bool> row_mask(bool = true)const;

        template<typename R = T>
        requires std::is_same_v<R, bool>
        Matrix<bool> col_mask(bool = true)const;
        
        Matrix<bool> mask_nan()const;
        Matrix<T> drop_nan_rows()const;
        Matrix<T> drop_nan_cols()const;

        //SEARCH
        template<typename R>
        T& closest(const R&);

        template<typename R>
        const T& closest(const R&)const;

        //MEAN SUM VAR SD
        T sum()const;
        Matrix<T> row_sum()const;
        Matrix<T> col_sum()const;

        double mean()const;
        Matrix<double> row_mean()const;
        Matrix<double> col_mean()const;

        T median()const;
        Matrix<T> row_median()const;
        Matrix<T> col_median()const;

        auto var(float = 0)const;
        Matrix<double> row_var(float = 0)const;
        Matrix<double> col_var(float = 0)const;

        auto std(float = 0)const;
        Matrix<double> row_std(float = 0)const;
        Matrix<double> col_std(float = 0)const;

        //MIN AND MAX
        const T& min()const;
        const T& max()const;
        const Mask_view<T> row_min()const;
        const Mask_view<T> row_max()const;
        const Mask_view<T> col_min()const;
        const Mask_view<T> col_max()const;

        T& min();
        T& max();
        Mask_view<T> row_min();
        Mask_view<T> row_max();
        Mask_view<T> col_min();
        Mask_view<T> col_max();

        T& min_abs();
        T& max_abs();
        Mask_view<T> row_min_abs();
        Mask_view<T> row_max_abs();
        Mask_view<T> col_min_abs();
        Mask_view<T> col_max_abs();

        const T& min_abs()const;
        const T& max_abs()const;
        const Mask_view<T> row_min_abs()const;
        const Mask_view<T> row_max_abs()const;
        const Mask_view<T> col_min_abs()const;
        const Mask_view<T> col_max_abs()const;

        //CORRELATION, COVARIANCE
        double auto_corr(long long = 1, float = 1);

        template<typename Rmx, typename R>
        double corr(const Matrix_base<Rmx, R>&, float = 1);

        template<typename Rmx, typename R>
        Matrix<double> auto_row_corr(const Matrix_base<Rmx, R>&, float = 1); //NIMPLY

        template<typename Rmx, typename R>
        Matrix<double> auto_col_corr(const Matrix_base<Rmx, R>&, float = 1); //NIMPLY

        template<typename Rmx, typename R>
        Matrix<double> row_corr(const Matrix_base<Rmx, R>&, float = 1);

        template<typename Rmx, typename R>
        Matrix<double> col_corr(const Matrix_base<Rmx, R>&, float = 1);

        template<typename Rmx, typename R>
        double cov(const Matrix_base<Rmx, R>&, float = 1);

        Matrix<double> cov_matrix(float = 1);

        template<typename Rmx, typename R>
        Matrix<double> row_cov(const Matrix_base<Rmx, R>&, float = 1);

        template<typename Rmx, typename R>
        Matrix<double> col_cov(const Matrix_base<Rmx, R>&, float = 1);

        //LINEAR
        template<typename R = T>
        requires (std::is_same_v<R, float>)
        Matrix<float> matmul(const Matrix<float>&)const;

        template<typename R = T>
        requires (std::is_same_v<R, double>)
        Matrix<double> matmul(const Matrix<double>&)const;

        template<typename R = T, typename Umx, typename U>
        requires (!std::is_floating_point_v<R>)
        Matrix<T> matmul(const Matrix_base<Umx, U>&)const;

        Matrix<T> matmul_t(const Matrix<T>&)const;
        
        template<typename Rmx, typename R>
        double dot(const Matrix_base<Rmx, R>&)const;

        double manhattan()const;

        template<typename Rmx, typename R>
        double manhattan(const Matrix_base<Rmx, R>&)const;

        double euclidean()const;

        template<typename Rmx, typename R>
        double euclidean(const Matrix_base<Rmx, R>&)const;

        template<typename Rmx, typename R>
        double cosine_sim(const Matrix_base<Rmx, R>&)const;

        template<typename Rmx, typename R>
        double cosine_dist(const Matrix_base<Rmx, R>&)const;

        //NORMS & SCALES
        template<typename R, typename U>
        Tmx& min_max(R = 0, U = 1);
        Tmx& z_scores(float = 1);
        Tmx& robust_scale(float = 0.2, float = 0.8);
        Tmx& robust_z(float = 0.2, float = 0.8, float = 1);
        Tmx& batch_norm(float = 1);
        Tmx& layer_norm(float = 1);
        Tmx& group_norm(long long, float = 1);


        //CLIPS AND EXHAUST
        template<typename R, typename U>
        Tmx& clip(R, U);
        Tmx& clip_Q(float = 0.1, float = 0.9);
        template<typename R, typename U>
        Tmx& clip_to_zero(R = -0.1, U = 0.1);

        template<typename R>
        Tmx& exhaust(R = 1, float = 1);
        Tmx& exhaust_Q(float = 0.1, float = 0.9, float = 1);

        //SPECIAL
        Tmx& abs();
        Tmx& log();
        Tmx& exp();

        //TRIGONOMETRY
        Tmx& sin();
        Tmx& cos();
        Tmx& tan();
        Tmx& tanh();


        //NON LINEAR TRANSFORM
        Tmx& rank(bool = true, bool = false);
        Tmx& quantile();
        Tmx& bucket(long long);
        Tmx& fit_gaussian();

        //Rolling
        Tmx& rolling(long long = 1);
        

        //INIT AND RANDOM
        template<typename R, typename U>
        Tmx& randomize_uniform(R = 0, U = 1);

        template<typename R, typename U>
        Tmx& randomize_normal(R = 0, U = 1);

        Tmx& init_he_normal(long long = -1);
        Tmx& init_he_uniform(long long = -1);
        Tmx& init_xav_normal(long long = -1, long long = -1);
        Tmx& init_xav_uniform(long long = -1, long long = -1);




        //POWER AND ROOT
        template<typename R>
        requires (std::is_arithmetic_v<R>)
        Tmx& power(const R&);
        template<typename Rmx, typename R>
        Tmx& power(const Matrix_base<Rmx, R>&);

        template<typename R>
        requires (std::is_arithmetic_v<R>)
        Tmx& root(const R&);
        template<typename Rmx, typename R>
        Tmx& root(const Matrix_base<Rmx, R>&);

        //CHARTING FEATURES
        void print()const;
        Tmx& map()const;
        Tmx& chart_xy()const;
        Tmx& chart_yx()const;




    };
}
//
//FLAG SKELETONS
//
namespace machx
{
    template<typename Tmx, typename T>
    class mask_flag
    {   
    private:
        friend class Matrix<T>;
        friend class Matrix_view<T>;
        friend class Mask_view<T>;

    public:
        Matrix_base<Tmx, T>* ptr{};
        bool row_mask{false};
        bool col_mask{false};
        bool logical_and{false};


        mask_flag(const Matrix_base<Tmx, T> &mxb1, bool row_m = false, bool col_m = false, bool use_and_logic = false) 
        : ptr{& const_cast<Matrix_base<Tmx, T> &> (mxb1)}
        , row_mask{row_m}
        , col_mask{col_m}
        , logical_and{use_and_logic}
        {}


        mask_flag(const mask_flag<Tmx, T>&) = delete;
        mask_flag(mask_flag<Tmx, T> &&other)
        : ptr{other.ptr} 
        , row_mask{other.row_mask}
        , col_mask{other.col_mask}
        , logical_and{other.logical_and}
        {other.ptr = nullptr;}

        mask_flag<Tmx, T>& operator=(mask_flag<Tmx, T>&&) = delete;
        mask_flag<Tmx, T>& operator=(const mask_flag<Tmx, T>&) = delete;
        ~mask_flag() = default;
    };

    template<typename Tmx, typename T>
    mask_flag<Tmx, T> Matrix_base<Tmx, T>::M(){
        return {*this};
    }
    template<typename Tmx, typename T>
    const mask_flag<Tmx, T> Matrix_base<Tmx, T>::M()const{
        return {*this};
    }
    template<typename Tmx, typename T>
    mask_flag<Tmx, T> Matrix_base<Tmx, T>::RMA(){
        return {*this, true, false, true}; //rowmask, colmask, LOGICAL AND bools
    }
    template<typename Tmx, typename T>
    mask_flag<Tmx, T> Matrix_base<Tmx, T>::CMA(){
        return {*this, false, true, true};
    }

    template<typename Tmx, typename T>
    const mask_flag<Tmx, T> Matrix_base<Tmx, T>::RMA()const{
        return {*this, true, false, true};
    }
    template<typename Tmx, typename T>
    const mask_flag<Tmx, T> Matrix_base<Tmx, T>::CMA()const{
        return {*this, false, true, true};
    }

    template<typename Tmx, typename T>
    mask_flag<Tmx, T> Matrix_base<Tmx, T>::RMO(){
        return {*this, true, false, false};
    }
    template<typename Tmx, typename T>
    mask_flag<Tmx, T> Matrix_base<Tmx, T>::CMO(){
        return {*this, false, true, false};
    }

    template<typename Tmx, typename T>
    const mask_flag<Tmx, T> Matrix_base<Tmx, T>::RMO()const{
        return {*this, true, false, false};
    }
    template<typename Tmx, typename T>
    const mask_flag<Tmx, T> Matrix_base<Tmx, T>::CMO()const{
        return {*this, false, true, false};
    }


    template<typename T>
    class assign_flag
    {
        friend class Matrix<T>;
        Matrix<T>* ptr{};
    public:
        
        assign_flag(Matrix<T> &mx) : ptr{&mx}{}
        assign_flag(const Matrix<T> &mx) = delete;
        assign_flag(Matrix<T> &&mx) = delete;
        assign_flag& operator= (const assign_flag&) = delete;
        assign_flag& operator= (assign_flag&&) = delete;
        
        template<typename Rmx, typename R>
        Matrix<T>& operator=(const Matrix_base<Rmx, R>&);
        Matrix<T>& operator=(Matrix<T>);
    };  

    template<typename T>
    class view_assign_flag
    {
        friend class Matrix_view<T>;
        Matrix_view<T>* ptr{};
    public:
        
        view_assign_flag(Matrix_view<T> &mxv1) : ptr{&mxv1}{}
        view_assign_flag(const view_assign_flag<T>&) = delete;
        view_assign_flag(view_assign_flag<T>&&) = delete;
        view_assign_flag<T>& operator= (const view_assign_flag<T>&) = delete;
        view_assign_flag<T>& operator= (view_assign_flag<T>&&) = delete;
        
        Matrix_view<T>& operator=(Matrix_view<T>);
        Matrix_view<T>& operator=(Matrix<T> &matrix);
    };  
}
