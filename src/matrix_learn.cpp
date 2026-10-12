#include "matrix_learn.h"
#include <cassert>

namespace statistic = matrix_statistic;
namespace learn = matrix_learn;
using method = Matrix_method;

Matrix matrix_learn::softmax(const Matrix_base &mxb1)
{   
    double e{2.718281828459};
    Matrix probabilities{method::broadcast(mxb1, e, Operation::equal)};
    probabilities /= statistic::sum(probabilities);
    
    return probabilities;
}


Matrix matrix_learn::softmax(const Matrix_base &mxb1, const bool row_wise)
{   
    double e{2.718281828459};
    Matrix probabilities{method::broadcast(mxb1, e, Operation::equal)};
    probabilities /= statistic::sum(probabilities, row_wise);

    return probabilities;
}







