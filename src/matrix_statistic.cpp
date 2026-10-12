#include "matrix_statistic.h"

using method = Matrix_method;

double matrix_statistic::sum(const Matrix_base &mxb1)
{
    Matrix mx{1, 1};
    method::broadcast(mx, mxb1, Operation::add);
    return mx(0,0);
}

Matrix matrix_statistic::sum(const Matrix_base &mxb1, bool row_wise)
{   
    Matrix mx{(row_wise)? Matrix{mxb1.get_rows(), 1} : Matrix{1, mxb1.get_cols()}};
    method::broadcast(mx, mxb1, Operation::add);
    return mx;
}

double matrix_statistic::avg(const Matrix_base &mxb1)
{   
    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};

    double avarage{sum(mxb1)};
    return avarage / (r1*c1);
}

Matrix matrix_statistic::avg(const Matrix_base &mxb1, bool row_wise)
{
    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};

    Matrix avarage{sum(mxb1, (row_wise)? true : false)};
    method::broadcast(avarage, (row_wise)? c1 : r1, Operation::divide);
    return avarage;
}

double matrix_statistic::variance(const Matrix_base &mxb1, double degree)
{   
    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};
    double mean{avg(mxb1)};

    Matrix var{mxb1};
    var -= mean;
    method::broadcast(var, 2, Operation::power);
    double var_out{sum(var)};
    return  var_out / (r1*c1-degree);
}

Matrix matrix_statistic::variance(const Matrix_base &mxb1, const bool row_wise, double degree)
{   
    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};
    Matrix mean{avg(mxb1, row_wise)};

    Matrix var{mxb1};
    var -= mean;
    method::broadcast(var, 2, Operation::power);
    Matrix var_out {sum(var, row_wise)};
    method::broadcast(var_out, (row_wise)? c1-degree : r1-degree, Operation::divide);
    return var_out;
}


double matrix_statistic::std(const Matrix_base &mxb1, double degree)
{   
    return std::sqrt(variance(mxb1, degree));
}

Matrix matrix_statistic::std(const Matrix_base &mxb1, const bool row_wise, double degree)
{   
    Matrix standard_deviation{variance(mxb1, row_wise, degree)};
    method::broadcast(standard_deviation, 2.0, Operation::root);
    return standard_deviation;
}

Matrix matrix_statistic::normal_distribution(const Matrix_base &mxb1, const bool row_wise)
{   
    double small_num {0.001};
    Matrix n_dist{mxb1 - avg(mxb1, row_wise)};

    Matrix variance_squared{
        method::broadcast(variance(mxb1, row_wise) + small_num, 2.0, Operation::root)
    };

    return n_dist / variance_squared;
}




