#include "matrix_mask.h"

//switch is outside to reduce unnecessary switch calls, will be looking into how this could be changed for cleaner code.

Matrix_mask::Matrix_mask(const Matrix_base& mxb1, Condition cond, const double threshold)
: rows{mxb1.get_rows() + mxb1.begin_row()}
, cols{mxb1.get_rows() + mxb1.begin_col()}
, mask(rows*cols, 0)
//mask array on alkuperäisen matriisin kokoinen jotta indeksöinti voidaan toteuttaa
//sekä matrix ja view objektilla
{   
    size_t r{mxb1.begin_row()};
    size_t c{mxb1.begin_col()};
    switch(cond)
    {
        case(Condition::over):
        {
            for(r ; r<rows ; r++){
                for(c; c<cols ; c++){
                    (*this)(r,c) = (mxb1(r,c)>threshold) ? 1 : 0;
                }
            }
            break;
        }
        case(Condition::under):
        {
            for(r ; r<rows ; r++){
                for(c; c<cols ; c++){
                    (*this)(r,c) = (mxb1(r,c)<threshold) ? 1 : 0;
                }
            }
            break;
        }
        case(Condition::over_or):
        {
            for(r ; r<rows ; r++){
                for(c; c<cols ; c++){
                    (*this)(r,c) = (mxb1(r,c)>=threshold) ? 1 : 0;
                }
            }
            break;
        }
        case(Condition::under_or):
        {
            for(r ; r<rows ; r++){
                for(c; c<cols ; c++){
                    (*this)(r,c) = (mxb1(r,c)<=threshold) ? 1 : 0;
                }
            }
            break;
        }
        case(Condition::is):
        {
            for(r ; r<rows ; r++){
                for(c; c<cols ; c++){
                    (*this)(r,c) = (mxb1(r,c)==threshold) ? 1 : 0;
                }
            }
            break;
        }
        case(Condition::is_not):
        {
            for(r ; r<rows ; r++){
                for(c; c<cols ; c++){
                    (*this)(r,c) = (mxb1(r,c)!=threshold) ? 1 : 0;
                }
            }
            break;
        }
    }
}

Matrix_mask::Matrix_mask(const Matrix_base& mxb1, Condition cond, const Matrix_base& mxb2)
: rows{mxb1.get_rows() + mxb1.begin_row()}
, cols{mxb1.get_rows() + mxb1.begin_col()}
, mask(rows*cols, 0)
{   
    assert(mxb1.get_rows() == mxb2.get_rows());
    assert(mxb1.get_rows() == mxb2.get_rows());

    size_t r{mxb1.begin_row()};
    size_t c{mxb1.begin_col()};
    switch(cond)
    {
        case(Condition::over):
        {
            for(r ; r<rows ; r++){
                for(c; c<cols ; c++){
                    (*this)(r,c) = (mxb1(r,c)>mxb2(r,c)) ? 1 : 0;
                }
            }
            break;
        }
        case(Condition::under):
        {
            for(r ; r<rows ; r++){
                for(c; c<cols ; c++){
                    (*this)(r,c) = (mxb1(r,c)<mxb2(r,c)) ? 1 : 0;
                }
            }
            break;
        }
        case(Condition::over_or):
        {
            for(r ; r<rows ; r++){
                for(c; c<cols ; c++){
                    (*this)(r,c) = (mxb1(r,c)>=mxb2(r,c)) ? 1 : 0;
                }
            }
            break;
        }
        case(Condition::under_or):
        {
            for(r ; r<rows ; r++){
                for(c; c<cols ; c++){
                    (*this)(r,c) = (mxb1(r,c)<=mxb2(r,c)) ? 1 : 0;
                }
            }
            break;
        }
        case(Condition::is):
        {
            for(r ; r<rows ; r++){
                for(c; c<cols ; c++){
                    (*this)(r,c) = (mxb1(r,c)==mxb2(r,c))? 1 : 0;
                }
            }
            break;
        }
        case(Condition::is_not):
        {
            for(r ; r<rows ; r++){
                for(c; c<cols ; c++){
                    (*this)(r,c) = (mxb1(r,c)!=mxb2(r,c)) ? 1 : 0;
                }
            }
            break;
        }
    }
}

Matrix_mask::Matrix_mask(const Matrix_mask &other)
: rows{other.rows}
, cols{other.cols}
, mask{other.mask}
{}

Matrix_mask::Matrix_mask(Matrix_mask &&other)
: rows{other.rows}
, cols{other.cols}
, mask{std::move(other.mask)}
{}

Matrix_mask& Matrix_mask::operator!()
{
    for(auto& val : mask)
    {
        val = (val==1) ? 0 : 1;
    }
    return *this;
}

Matrix_mask operator&&(const Matrix_mask &mask1, const Matrix_mask mask2)
{
    assert((mask1.cols == mask2.cols) && (mask1.rows == mask2.rows)); //make throw
    Matrix_mask mask3{mask1};
    for(size_t r{} ; r<mask1.rows ; r++){
        for(size_t c{} ; c<mask1.cols ; c++){
            mask3(r, c) = ((mask1(r, c) == 1) && (mask2(r,c) == 1)) ? 1 : 0;
        }
    }
    return mask3;
}

Matrix_mask operator||(const Matrix_mask &mask1, const Matrix_mask mask2)
{
    assert((mask1.cols == mask2.cols) && (mask1.rows == mask2.rows)); //make throw
    Matrix_mask mask3{mask1};
    for(size_t r{} ; r<mask1.rows ; r++){
        for(size_t c{} ; c<mask1.cols ; c++){
            mask3(r, c) = ((mask1(r, c) == 1) || (mask2(r,c) == 1)) ? 1 : 0;
        }
    }
    return mask3;
}



//
//COMPARISON FRIEND FUNCTIONS
//

Matrix_mask operator>(const Matrix_base &mxb1, const double val)
{   
    return {mxb1, Condition::over, val};
}

Matrix_mask operator<(const Matrix_base &mxb1, const double val)
{   
    return {mxb1, Condition::under, val};
}

Matrix_mask operator>=(const Matrix_base &mxb1, const double val)
{   
    return {mxb1, Condition::over_or, val};
}

Matrix_mask operator<=(const Matrix_base &mxb1, const double val)
{   
    return {mxb1, Condition::under_or, val};
}

Matrix_mask operator==(const Matrix_base &mxb1, const double val)
{   
    return {mxb1, Condition::is, val};
}

Matrix_mask operator!=(const Matrix_base &mxb1, const double val)
{   
    return {mxb1, Condition::is_not, val};
}

//for comparing two matrices

Matrix_mask operator>(const Matrix_base &mxb1, const Matrix_base &mxb2)
{   
    return {mxb1, Condition::over, mxb2};
}

Matrix_mask operator<(const Matrix_base &mxb1, const Matrix_base &mxb2)
{   
    return {mxb1, Condition::under, mxb2};
}

Matrix_mask operator>=(const Matrix_base &mxb1, const Matrix_base &mxb2)
{   
    return {mxb1, Condition::over_or, mxb2};
}

Matrix_mask operator<=(const Matrix_base &mxb1, const Matrix_base &mxb2)
{   
    return {mxb1, Condition::under_or, mxb2};
}

Matrix_mask operator==(const Matrix_base &mxb1, const Matrix_base &mxb2)
{   
    return {mxb1, Condition::is, mxb2};
}

Matrix_mask operator!=(const Matrix_base &mxb1, const Matrix_base &mxb2)
{   
    return {mxb1, Condition::is_not, mxb2};
}
