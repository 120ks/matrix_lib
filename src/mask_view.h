#pragma once


#include "matrix_base.h"
#include "mask.h"

class Mask_view : public Matrix_base
{   
    private:
        Mask_view(const Matrix_base &mxb1, Mask m);
        Mask_view(Matrix_base &&mxb1, Mask m);
        Mask_view(Mask_view &&other);
        Mask_view(const Mask_view &other);
        Mask mask;
        double* mask_ptr{};
        std::vector<size_t>index_arr{};
    public:
        //overrides
        double& operator()(const size_t, const size_t) override;
        double operator()(const size_t, const size_t) const override;

        inline size_t get_rows()const override {index_arr.size();}
        inline size_t get_cols()const override {return 1;}

        inline size_t matrix_rows()const override {return mask.rows;}
        inline size_t matrix_cols()const override {return mask.cols;}

        //muutetaan ehkä jos huomataan että tarvitaan todellinen aloitus esim slice varten.
        inline size_t begin_row()const override{return 0;}
        inline size_t begin_col()const override {return 0;} 
};
