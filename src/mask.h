
#pragma once
#include "matrix.h"


class Mask
{   
    friend class Matrix_view;
    friend class Mask_view;
    private:
        size_t rows{};
        size_t cols{};
        std::vector<BOOL> mask{};

        size_t total_elements{rows*cols};
        size_t true_elements{};

        bool is_inverted{false};

        char check_condition(const double val, const Condition, const double threshold);

    public:
        Mask(const Matrix_base& mxb1, Condition cond, const double threshold);
        Mask(const Matrix_base& mxb1, Condition cond, const Matrix_base& mxb2);
        Mask(size_t rows, size_t cols, bool set_to);

        Mask(const Mask &other);                                                                       
        Mask(Mask &&other);

        inline BOOL& operator()(const size_t row, const size_t col){
            return mask[row * rows + col];
        }
        inline bool operator()(const size_t row, const size_t col)const{
            return mask[row * rows + col];
        }

        Mask operator()(Slice sl1, Slice sl2)
        {   
            auto total_rows{sl1.end-sl1.begin};
            auto total_cols{sl2.end-sl2.begin};
            Mask mask2{total_rows, total_cols, false};
            

        }

        Mask& operator!();
        friend Mask operator&&(const Mask &mask1, const Mask mask2);
        friend Mask operator||(const Mask &mask1, const Mask mask2);


        friend Mask operator>(const Matrix_base &mxb1, const double val);
        friend Mask operator<(const Matrix_base &mxb1, const double val);
        friend Mask operator>=(const Matrix_base &mxb1, const double val);
        friend Mask operator<=(const Matrix_base &mxb1, const double val);
        friend Mask operator==(const Matrix_base &mxb1, const double val);
        friend Mask operator!=(const Matrix_base &mxb1, const double val);

        friend Mask operator>(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Mask operator<(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Mask operator>=(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Mask operator<=(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Mask operator==(const Matrix_base &mxb1, const Matrix_base &mxb2);
        friend Mask operator!=(const Matrix_base &mxb1, const Matrix_base &mxb2);
};
