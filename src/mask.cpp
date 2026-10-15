#include "mask.h"


Mask::Mask(const Matrix_base& mxb1, const Condition condition, const double threshold)
: rows{mxb1.get_rows()}
, cols{mxb1.get_cols()}
, mask(rows*cols, false)
{   
    for(size_t r{} ; r<rows ; r++)
    {
        for(size_t c{}; c<cols ; c++){
            mask[r*cols + c] = check_condition(mxb1(r, c), condition, threshold);
            if(mask[r*cols + c]) {
                true_elements++;
            }
            
        }
    }
}

Mask::Mask(const Matrix_base& mxb1, const Condition condition, const Matrix_base& mxb2)
: rows{mxb1.get_rows()}
, cols{mxb1.get_cols()}
, mask(rows*cols, false)
{
    for(size_t r{} ; r<rows ; r++)
    {
        for(size_t c{}; c<cols ; c++){
            mask[r*cols + c] = check_condition(mxb1(r, c), condition, mxb2(r, c));
            if(mask[r*cols + c]) {
                true_elements++;
            }
        }
    }
}

Mask::Mask(size_t rows, size_t cols, bool set_to)
: rows{rows}
, cols{cols}
, mask(rows*cols, set_to)
{
}

Mask::Mask(const Mask &other)
: rows{other.rows}
, cols{other.cols}
, mask{other.mask}
{}

Mask::Mask(Mask &&other)
: rows{other.rows}
, cols{other.cols}
, mask{std::move(other.mask)}
{}

char Mask::check_condition(const double val, const Condition condition, double threshold)
{   
    switch(condition)
    {   
        case(Condition::is_true): return 1;
        case(Condition::is_false): return 0;
        case(Condition::over): return (val == threshold) ? 1 : 0;
        case(Condition::over_or): return (val == threshold) ? 1 : 0;
        case(Condition::under): return (val == threshold) ? 1 : 0;
        case(Condition::under_or): return (val == threshold) ? 1 : 0;
        case(Condition::is) : return (val == threshold) ? 1 : 0;
        case(Condition::is_not): return (val == threshold) ? 1 : 0;
    }
    return 0;

}


Mask& Mask::operator!()
{   
    true_elements = total_elements-true_elements;
    is_inverted = !is_inverted;
    return *this;
}

Mask operator&&(const Mask &mask1, const Mask mask2)
{
    assert((mask1.cols == mask2.cols) && (mask1.rows == mask2.rows)); //make throw
    Mask mask3{mask1};
    for(size_t r{} ; r<mask1.rows ; r++){
        for(size_t c{} ; c<mask1.cols ; c++){
            mask3(r, c) = ((mask1(r, c) == 1) && (mask2(r,c) == 1)) ? 1 : 0;
            if(mask3(r,c)){

                mask3.true_elements++;
            }
        }
    }
    return mask3;
}

Mask operator||(const Mask &mask1, const Mask mask2)
{
    assert((mask1.cols == mask2.cols) && (mask1.rows == mask2.rows)); //make throw
    Mask mask3{mask1};
    for(size_t r{} ; r<mask1.rows ; r++){
        for(size_t c{} ; c<mask1.cols ; c++){
            mask3(r, c) = ((mask1(r, c) == 1) || (mask2(r,c) == 1)) ? 1 : 0;
            if(mask3(r,c)){
                mask3.true_elements++;
            }
        }
    }
    return mask3;
}

//
//COMPARISON FRIEND FUNCTIONS
//

Mask operator>(const Matrix_base &mxb1, const double val)
{   
    return {mxb1, Condition::over, val};
}

Mask operator<(const Matrix_base &mxb1, const double val)
{   
    return {mxb1, Condition::under, val};
}

Mask operator>=(const Matrix_base &mxb1, const double val)
{   
    return {mxb1, Condition::over_or, val};
}

Mask operator<=(const Matrix_base &mxb1, const double val)
{   
    return {mxb1, Condition::under_or, val};
}

Mask operator==(const Matrix_base &mxb1, const double val)
{   
    return {mxb1, Condition::is, val};
}

Mask operator!=(const Matrix_base &mxb1, const double val)
{   
    return {mxb1, Condition::is_not, val};
}

//for comparing two matrices

Mask operator>(const Matrix_base &mxb1, const Matrix_base &mxb2)
{   
    return {mxb1, Condition::over, mxb2};
}

Mask operator<(const Matrix_base &mxb1, const Matrix_base &mxb2)
{   
    return {mxb1, Condition::under, mxb2};
}

Mask operator>=(const Matrix_base &mxb1, const Matrix_base &mxb2)
{   
    return {mxb1, Condition::over_or, mxb2};
}

Mask operator<=(const Matrix_base &mxb1, const Matrix_base &mxb2)
{   
    return {mxb1, Condition::under_or, mxb2};
}

Mask operator==(const Matrix_base &mxb1, const Matrix_base &mxb2)
{   
    return {mxb1, Condition::is, mxb2};
}

Mask operator!=(const Matrix_base &mxb1, const Matrix_base &mxb2)
{   
    return {mxb1, Condition::is_not, mxb2};
}
