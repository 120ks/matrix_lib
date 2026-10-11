#pragma once

#include "matrix.h"
#include "matrix_view.h"
#include "matrix_method.h"

using mrx = Matrix_method;

inline std::ostream& operator<<(std::ostream &os, const Matrix_base &mxb)
{
    mrx::print(os, mxb);
    return os;
}

