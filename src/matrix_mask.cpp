#include "matrix_mask.h"

Matrix_mask::Matrix_mask(Matrix_base &mxb1, Condition con, const double filter_val)
: mask_ref{mxb1}
, condition{con}
, filter{filter_val}
{
    
}

Matrix_mask::Matrix_mask(const Matrix_mask &other)
: mask_ref{other.mask_ref}
, condition{other.condition}
, filter{other.filter}
{
    
}

Matrix_mask::Matrix_mask(Matrix_mask &&other)noexcept
: mask_ref{other.mask_ref}
, condition{other.condition}
, filter{other.filter}
{
}

double& Matrix_mask::operator()(const size_t row, const size_t col)
{   
    double& temp {mask_ref(row, col)};
    return (this->check_condition(temp)) ? temp : temp;
}



bool Matrix_mask::check_condition(const double &val)
{
    switch(condition)
    {
        case(Condition::over): return val>filter;
        case(Condition::under): return val<filter;
        case(Condition::over_or): return val>=filter;
        case(Condition::under_or): return val<=filter;
        case(Condition::is): return val==filter;
        case(Condition::is_not): return val!=filter;
    }
}
