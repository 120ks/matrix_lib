#pragma once
#include "matrix_method.h"
#include "matrix.h"
#include "matrix_view.h"


inline std::ostream& operator<<(std::ostream &os, const Matrix_base &mxb)
{   
    
    Matrix_method::print(os, mxb);
    return os;
}

