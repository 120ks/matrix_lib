#pragma once

#include "matrix.h"
#include "matrix_view.h"
#include "matrix_method.h"


inline std::ostream& operator<<(std::ostream &os, const Matrix &mx)
{
    Matrix_method::print(os, mx);
    return os;
}
