#include "mx.hpp"
#include <iomanip>


void test()
{
//creating a matrix
    using machine::Matrix;

    Matrix<int> matrix1{4, 4, 1};
    Matrix<double> matrix2{3, 4, 2.5};
    std::cout<<"- matrix1 is 4x4 all 1's -"<<std::endl;
    std::cout<<matrix1<<'\n';

    std::cout<<"- matrix2 is 3x4 all 2.5's -"<<std::endl;
    std::cout<<matrix2<<'\n';

    //testing slicing
    std::cout<<"- testing basic slicing -"<<std::endl;

    std::cout<<"- matrix1 center to be 2 -"<<std::endl;
    matrix1(sl{1, 3}, sl{1, 3}) = matrix2(sl{0, 2}, sl{1, 3});
    std::cout<<matrix1<<'\n'; 

    std::cout<<"- matrix2 bottom row be *3 (=7.5) -"<<std::endl;
    matrix2(sl{2}, sl{}) *= 3;
    std::cout<<matrix2<<'\n'; 

    //testing basic masking
    std::cout<<"- testing basic masking -"<<std::endl;

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

    std::cout<<"- when multiplying in place, we usually want to keep the leftenmost type, we dont want that to change, so by multiplying matrix1 with matrix3-"<<std::endl;
    std::cout<<"- matrix1 should be integers -"<<std::endl;
    matrix1 *= matrix3;
    std::cout<<matrix1<<'\n';

    std::cout<<"- arithmetic numbers and common types- "<<std::endl;
    std::cout<<"- <int> * <double> should return a matrix of doubles- "<<std::endl;
    std::cout<< matrix1 * matrix2;
    
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
    machine::Matrix<boolean> sliced_mask = mask(sl{0, 3}); //mx::mask alias for Matrix<boolean>
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

    std::cout<<"- stress testing - "<<std::endl;

    std::cout<<"- very large combination masking, slicing assigning, multiplying etc - "<<std::endl;
    Matrix<double> matrix4{20, 20, 1.5};
    (((matrix4(sl{0, 15}, sl{3, 7})*=2)[((matrix4(sl{}, sl{10, 0})+= 5)({0, 15}, sl{2, 7}) /=3 > 0)(sl{}, sl{0, 4})] = 9))(sl{0}, sl{13, 0}) /= -5;
    std::cout<<matrix4;

    std::cout<<"- chaining - "<<std::endl;
    ((matrix1 -= matrix2) *=matrix3) += matrix4;
    std::cout<<matrix1;

    std::cout<<"- lets do this, well make a small 2x2 matrix and have it multiply our 4x4 matrix 4 times, each in different place- "<<std::endl;
    std::cout<<"- lets make our 4x4 matrix 3 all ones to visualize better and lets make the 2x2 matrix be 1, 2, 3 ,4- "<<std::endl;
    matrix3 = 1;
    Matrix<double> small_matrix{2, 2, 1};
    small_matrix(0, 1) = 2 ; small_matrix(1, 0) = 3 ; small_matrix(1, 1) = 4;
    auto matrix5 {(matrix3 *= small_matrix)};
    std::cout<<matrix5;


    std::cout<<"-   FLAGS   - "<<std::endl;
    std::cout<<"- ========= - "<<std::endl;

    std::cout<<"flag object is a special class that is used to make syntax more user friendly in some cases, flags are temp objects, so they exist only in statements"<<std::endl;
    std::cout<<"the .M() returns a proxy object that is basically a ptr to the underlying, this allows us to 'overload it so that we can bypass othwise akward syntax"<<std::endl;
    std::cout<<"for example slicing and masking the slice is very error prone as the same slice is needed, inside multiple loops this might be difficult to maintain"<<std::endl;
    std::cout<<"also: because direct mask creates 'true array' directly without a mask, if the mask changes a lot in iteration prefer .M()"<<std::endl;
    std::cout<<"- use .M() when you are finished slicing and want to make a direct mask - "<<std::endl;
    matrix5(sl{}, {2, 4}).M() == matrix5(sl{}, {2, 4}) = 0; //should have all values over 2 from previous matrix be 0, if they are on columns index 2, 3
    std::cout<<matrix5;

    std::cout<<"-   works with multislicing   - "<<std::endl;

    matrix1(sl{}, sl{0, 3})(sl{2, 4}, sl{}).M() >0 = 0;
    std::cout<<matrix1;

    std::cout<<"-   transpose() returns a Transposed matrix   - "<<std::endl;
    std::cout<<matrix1.transpose();

    std::cout<<"-   transpose works with slices aswell   - "<<std::endl;
    std::cout<<matrix1(sl{}, sl{1, 3}).transpose();

    std::cout<<"-   matrix_math   - "<<std::endl;
    std::cout<<"-   lets print matrix2 for memory   - "<<std::endl;
    std::cout<<matrix2;
    std::cout<<"-   statistics   - "<<std::endl;
    std::cout<<"-sum of matrix2 : "<<machine::sum(matrix2)<<'\n'<<std::endl;
    std::cout<<"-sums of matrix2 col_wise : "<<machine::sum(matrix2, axis::Y);
    std::cout<<"-sums of matrix2 slice row_wise :\n"<<machine::sum(matrix2(sl{0, 2}, {1, 3}), axis::X);

    std::cout<<"-avg of matrix2 row_wise :\n"<< machine::avarage(matrix2, axis::X);

    std::cout<<"-variance of matrix2 : "<<machine::variance(matrix2)<<std::endl;
    std::cout<<"-variance of matrix2 row_wise :\n"<<machine::variance(matrix2, axis::X);

    std::cout<<"-std of matrix2 : "<<machine::standard_dev(matrix2)<<std::endl;
    std::cout<<"-std of matrix2 row_wise :\n"<<machine::standard_dev(matrix2, axis::X);

    std::cout<<machine::euclidean_distance(matrix1(sl{1, 0}), matrix2)<<std::endl;
    std::cout<<matrix3(sl{1, 0})<<matrix2;
    std::cout<<machine::concat(matrix3(sl{1, 0}), matrix2, axis::Y);

    std::cout<<matrix1<<matrix2;
    machine::broadcast(matrix1, 3, Operation::power); //tarkista tämä siinä, missä se käyttää toista mxb2
    std::cout<<matrix1;
}



int main()
{   
    namespace mxn = machine;
    try
    {
        test();
    }
    catch(mxn::dimension_mismatch &e)
    {
        std::cout<<"dimension mismatch\n";
        std::cout<<e.what();
        return 1;
    }
    catch(mxn::indexing_mismatch &e)
    {
        std::cout<<"indexing mismatch\n";
        std::cout<<e.what();
        return 1;
    }
    catch(mxn::slicing_mismatch &e)
    {
        std::cout<<"slicing mismatch\n";
        std::cout<<e.what();
        return 1;
    }
    catch(std::exception &e)
    {   
        std::cout<<"exception detected\n";
        std::cout<<e.what();
        return 1;
    };

    return 0;

}
