#pragma once

#include "matrix.h"
#include "matrix_view.h"
#include "matrix_method.h"

inline std::ostream& operator<<(std::ostream &os, const Matrix_base &mxb)
{
    Matrix_method::print(os, mxb);
    return os;
}

