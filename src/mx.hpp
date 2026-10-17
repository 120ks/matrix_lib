#pragma once

#include "friend_operators.hpp"
#include "matrix_statistics.hpp"
#include "matrix_learn.hpp"


using sl = Slice;

namespace mx
{   
    namespace method = matrix_method;
    namespace learn = matrix_learn;
    namespace statistics = matrix_statistics;
    using mask = Matrix<boolean>;
}
