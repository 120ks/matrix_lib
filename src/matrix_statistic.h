#pragma once

#include "matrix.h"
#include "matrix_view.h"
#include "matrix_method.h"

namespace matrix_statistic
{   
    double minimum(const Matrix_base& mxb1); //not yet
    double maximum(const Matrix_base& mxb1); //not yet

    double sum(const Matrix_base &mxb1);
    Matrix sum(const Matrix_base &mxb1, const bool row_wise);

    double avg(const Matrix_base &mxb1);
    Matrix avg(const Matrix_base &mxb1, const bool row_wise);
    
    double variance(const Matrix_base &mxb1, double degree = 0);
    Matrix variance(const Matrix_base &mxb1, const bool row_wise, double degree = 0);

    double std(const Matrix_base &mxb1, double degree = 0);
    Matrix std(const Matrix_base &mxb1, const bool row_wise, double degree = 0);

    Matrix normal_distribution(const Matrix_base &mxb1); //not yet
    Matrix normal_distribution(const Matrix_base &mxb1, const bool row_wise);

    Matrix min_max_norm(const Matrix_base &mxb1); //not yet
    Matrix min_max_norm(const Matrix_base &mxb1, const bool row_wise); //not yet

    Matrix robust_scale();
    Matrix z_score();

};

