#include "mx.hpp"
#include <iomanip>

int main()
{
    Matrix<int> matrix{30, 30, 1};

    
    std::cout<<(matrix(sl{2}, sl{2, 10})((matrix(sl{1, 10}, sl{2, 24})(sl{3, 4}, sl{2,10})>0)) = 2);

    

    return 0;
}
