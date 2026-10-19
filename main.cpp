#include "mx.hpp"
#include <iomanip>

int main()
{
    Matrix<int> matrix{30, 30, 1};
    Matrix<double> matrix2{30, 30, 1.1};

    matrix[(matrix(sl{}, sl{}) > matrix2)] = 10;

    std::cout<<matrix<<matrix2;
    return 0;
}
