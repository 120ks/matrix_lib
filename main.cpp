#include "mx.hpp"
#include <iomanip>

int main()
{
    //creating a matrix

    Matrix<int> matrix1{4, 4, 1};
    Matrix<double> matrix2{3, 4, 2.5};

    std::cout<<"- matrix1 is 4x4 all 1's -"<<std::endl;
    std::cout<<matrix1<<'\n';

    std::cout<<"- matrix2 is 3x4 all 2.5's -"<<std::endl;
    std::cout<<matrix2<<'\n';

    //testing slicing 
    std::cout<<"- testing basic slicing -"<<std::endl;

    std::cout<<"- matrix1 center to be 2 -"<<std::endl;
    matrix1(sl{1, 3}, sl{1, 3}) = 2;
    std::cout<<matrix1<<'\n'; 

    std::cout<<"- matrix2 bottom row be *3 (=7.5) -"<<std::endl;
    matrix2(sl{2}, sl{}) *= 3;
    std::cout<<matrix2<<'\n'; 

    //testing basic masking
    std::cout<<"- testing basic slicing -"<<std::endl;

    std::cout<<"- matrix1 values under 2 will be 3 -"<<std::endl;
    matrix1[matrix1<2] = 3;
    std::cout<<matrix1<<'\n'; 

    std::cout<<"- matrix2 values under 5 will be /2, but only on cols 0, 1, should have 4 (1.25 values) -"<<std::endl;
    matrix2(sl{}, sl{0, 2})[matrix2(sl{}, sl{0, 2}) <5] /= 2;
    std::cout<<matrix2<<'\n'; 

    //testing operators
    std::cout<<"- testing operators -"<<std::endl;
    std::cout<<"- lets create a new matrix by multiplying matrix1, using only the values under 2 in matrix2 (4 values) -"<<std::endl;
    std::cout<<"- this matrix should be of type double, and 4x4, and for simplicity, lets actually make all those under 2 values 1.5-"<<std::endl;
    std::cout<<"- so all matrix 1 values multiplied by 1.5 should be four 3 in the middle and the surrounding 4.5 -"<<std::endl;
    auto matrix3 = matrix1*(matrix2[matrix2<2]=1.5);
    std::cout<<matrix3<<'\n';

    std::cout<<"- lets actually try something little bit more common -"<<std::endl;
    std::cout<<"- when multiplying in place, we usually want to keep the leftenmost type, we dont want that to change, so by multiplying matrix1 with matrix3-"<<std::endl;
    std::cout<<"- matrix1 should be integers -"<<std::endl;
    matrix1 *= matrix3;
    std::cout<<matrix1<<'\n';
    
    std::cout<<"- lets use every operator on matrix3 with matrix1 -"<<std::endl;

    std::cout<<"- + -"<<std::endl;
    std::cout<<(matrix3 += matrix1)<<'\n';
    std::cout<<"- - -"<<std::endl;
    std::cout<<(matrix3 -= matrix1)<<'\n';
    std::cout<<"- * -"<<std::endl;
    std::cout<<(matrix3 *= matrix1)<<'\n';
    std::cout<<"- / -"<<std::endl;
    std::cout<<(matrix3 /= matrix1)<<'\n';
    std::cout<<"- and were back where we left-"<<std::endl;

    std::cout<<"- lets look at the mask when we want values under 4 on matrix 3 - "<<std::endl;
    auto mask = matrix3<4;
    std::cout<<mask<<'\n';

    std::cout<<"lets shave the last row off"<<std::endl;
    mx::mask sliced_mask = mask(sl{0, 3});
    std::cout<<sliced_mask<<'\n';

    std::cout<<"- now that it is correct shape, lets use it on matrix2, however lets reverse it with ! - "<<std::endl;
    matrix2[!sliced_mask] = 9;
    std::cout<<matrix2<<'\n';

    std::cout<<"- double mask && mask, should have all of the values between 2 and 9 be 0.1 - "<<std::endl;
    matrix2[(matrix2<9) && (matrix2>2)] = 0.1;
    std::cout<<matrix2<<'\n';

    std::cout<<"- mask using || mask1 and mask2, values over 5 and values under 1 be *3 all previous 9's and 0.1's should be *3 - "<<std::endl;
    matrix2[(matrix2>5) || (matrix2<1)] *= 3;
    std::cout<<matrix2<<'\n';


    return 0;
}
