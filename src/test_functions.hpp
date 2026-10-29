#pragma once

#include "mx.hpp"
#include <chrono>
#include <iostream>

namespace machine
{
    void test_masking()
    {   
        std::cout<<"testing masking\n"<<std::endl;

        Matrix<int> matrix1{5, 5};
        Matrix<int> matrix2{5, 5};
        Matrix<double> matrix3{5, 10};

        randomize(matrix1, 1, 10);
        randomize(matrix2, 1, 10);
        randomize(matrix3, 1.0, 10.0);
        auto matrix1_copy = matrix1.copy();

        std::cout<<"matrix1"<<std::endl;
        std::cout<<matrix1;

        std::cout<<"matrix2"<<std::endl;
        std::cout<<matrix2;

        std::cout<<"matrix3"<<std::endl;
        std::cout<<matrix3;

        std::cout<<"matrix1 values under 2 will be 3"<<std::endl;
        matrix1[matrix1<2] = 3;
        std::cout<<matrix1<<'\n'; 

        std::cout<<"matrix2 values that are under matrix 1 values will be *2"<<std::endl;
        matrix2[matrix2<matrix1] *=2;
        std::cout<<matrix2<<'\n';

        std::cout<<"matrix 3 values of left side under the values of right side will be 0 right side"<<std::endl;
        std::cout<<"using mask flag .M() for less syntax"<<std::endl;

        matrix3(sl{}, sl{0, 5}).M() < matrix3(sl{}, sl{5, 0}) = 0;
        std::cout<<matrix3<<'\n';

        std::cout<<"new matrix from the values of matrix1 vals over matrix 2 "<<std::endl;
        Matrix<int> matrix_trues = matrix1[matrix1 > matrix2];
        std::cout<<matrix_trues<<'\n';

        std::cout<<"logical operations"<<std::endl;
        std::cout<<"matrix1 values under matrix2 and over 3 will be 0 (&&)"<<std::endl;
        matrix1[(matrix1<matrix2) && (matrix1>3)] = 0;
        std::cout<<matrix1<<'\n';

        std::cout<<"same but reversed should make all in matrix1 be 0 (!)"<<std::endl;
        matrix1[!((matrix1<matrix2) && (matrix1>3))] = 0;

        std::cout<<"gladly .copy() was used to create a copy of matrix1"<<std::endl;
        std::cout<<matrix1<<'\n';

        std::cout<<"gladly .copy() was used to create a copy of matrix1"<<std::endl;
        matrix1 = matrix1_copy;
        std::cout<<matrix1<<'\n';

        std::cout<<"last logical :  and (||) operator to make all values 3, 5 and 9 = 0"<<std::endl;
        matrix1[(matrix1==3) || (matrix1==5) || (matrix1==9)] = 0;
        std::cout<<matrix1<<'\n';
    }   




    void test_view()
    {       
        std::cout<<"testing view\n"<<std::endl;

        Matrix<int> matrix1{5, 5};
        Matrix<int> matrix2{5, 5};
        Matrix<double> matrix3{2, 10};

        randomize(matrix1, 1, 10);
        randomize(matrix2, 1, 10);
        randomize(matrix3, 1.0, 10.0);

        std::cout<<"matrix1"<<std::endl;
        std::cout<<matrix1;

        std::cout<<"matrix2"<<std::endl;
        std::cout<<matrix2;

        std::cout<<"matrix3"<<std::endl;
        std::cout<<matrix3;

        std::cout<<"matrix1 first 2 rows and last 2 cols"<<std::endl;
        std::cout<<matrix1(sl{0, 2}, sl{3, 5});

        std::cout<<"matrix2 first row only"<<std::endl;
        std::cout<<matrix2(sl{0});

        std::cout<<"make middle row of matrix1 be middle of matrix2 "<<std::endl;
        matrix1(sl{2}) = matrix2(sl{2});
        std::cout<<matrix1;

        std::cout<<" matrix2 row 2 the same as matrix1 column 1 //note that for this to work, the right side needs to be transposed "<<std::endl;
        matrix2(sl{2}) = matrix1(sl{}, sl{1}).transpose();
        std::cout<<matrix2;

        std::cout<<" multi slicing matrix3"<<std::endl;
        std::cout<<matrix3(sl{}, sl{1, 4})((sl{1, 3}, sl{1, 0}))(sl{0}, sl{1, 0}); //not recommended for obvious reasons

        std::cout<<"multiplying matrix3 with a smaller 2x2 matrix from matrix1"<<std::endl;
        matrix3 *= matrix2(sl{0, 2}, sl(0, 2));
        std::cout<<matrix3;

        std::cout<<"for a better understanding, same but assign, because of matrix gen system interference we must use a slice in tile assign, easy to do with .V() flag"<<std::endl;
        matrix3.V() = matrix2(sl{0, 2}, sl(0, 2));
        std::cout<<matrix3;
    }  

    void test_view_safety()
    {   

        std::cout<<"view safety\n"<<std::endl;

        Matrix<int> matrix1{5, 5};

        randomize(matrix1, 1, 10);

        std::cout<<"matrix1"<<std::endl;
        std::cout<<matrix1;
        std::cout<<"matrix1 gen is now: "<<matrix1.get_gen()<<std::endl;

        std::cout<<"when matrix1 gets moved and has a view pointing at it\n"<<std::endl;
        auto mxv1 = matrix1.V();
        auto matrix_temp = std::move(matrix1);
        std::cout<<"matrix1 gen is now: "<<matrix1.get_gen()<<std::endl;

        if(!mxv1){
            std::cout<<"if this prints, the move was safely detected"<<std::endl;
            std::cout<<"trying to reset\n";
            mxv1.reset_view();
        }
        if(!mxv1){
            std::cout<<"if this prints, the reset did not activate like it should not have\n"<<std::endl;
        }
        std::cout<<"when matrix1 gets data again , note that we must use assign flag .A() when editing dims of left val matrix\n";
        matrix1.A() = matrix_temp;
        std::cout<<"matrix1 gen is now: "<<matrix1.get_gen()<<std::endl;

        std::cout<<"trying to reset\n";
        mxv1.reset_view();
        if(mxv1){
            std::cout<<"if this prints, the mxv1 reset worked like it should have\n";
        }
        std::cout<<"checking by editing the first row to be 0, using slice on view  will actually allow us to keep the mxv1 as full view\n";
        mxv1(sl{0}) = 0;
        std::cout<<matrix1<<std::endl;

        std::cout<<"assigning matrix to be temp, but with assign flag\n";
        matrix1.A() = std::move(matrix_temp);
        std::cout<<"matrix1 gen is now: "<<matrix1.get_gen()<<std::endl;
        
        if(!mxv1){
            std::cout<<"if this reads, it means the version of matrix1 has changed and is now flagged by matrix_view as 'error prone'\n"<<std::endl;
        }
        std::cout<<"trying to reset\n";
        mxv1.reset_view();
        if(mxv1){
            std::cout<<"if this prints, the mxv1 reset worked like it should have\n"<<std::endl;
        }
        std::cout<<"to check, this generation of mxv1 should match the matrix version: this version is -> "<<mxv1.get_gen()<<std::endl;
        std::cout<<"matrix1 gen is now: "<<matrix1.get_gen()<<std::endl<<std::endl;

        std::cout<<"lets make matrix1 one more time not valid by moving its data\n";
        matrix_temp.A() = std::move(matrix1);
        if(!mxv1){
            std::cout<<"this should print as matrix1 is no longer valid\n"<<std::endl;
        }
        
        std::cout<<"changing the place where we point with .change_pointer()\n";
        if(matrix_temp){
            std::cout<<"changing to temp\n";
            mxv1.change_pointer(matrix_temp);
        }
        
        if(mxv1){
            std::cout<<"this printing means we point at matrix_temp now\n"<<std::endl;
        }
        std::cout<<"to check, this generation of mxv1 should be same as temp, this mxv1 is at gen -> "<<mxv1.get_gen()<<std::endl;
        std::cout<<"matrix_temp gen -> "<<matrix_temp.get_gen()<<std::endl<<std::endl;

        std::cout<<"also if we make the whole view = 0, then temp should be 0 as well \n";
        mxv1 = 0;
        std::cout<<matrix_temp;

        

        std::cout<<"there is also .force_update() that keeps slicing data on a view, however it is exception prone as it does not guarantee that they are valid\n";
        std::cout<<"use it in scenarios where you are certain the rows / cols have not changed in matrix during assign in assign flag\n";
        std::cout<<"it does guarantee that the matrix has data so it will trigger at max exception\n"<<std::endl;


        std::cout<<"move data to matrix and change view to it \n";
        matrix1.A() = std::move(matrix_temp);
        mxv1.change_pointer(matrix1);
        
        if(mxv1){
            std::cout<<"this printing means we point at matrix1\n"<<std::endl;
            std::cout<<"mxv1 gen -> "<<mxv1.get_gen()<<std::endl;
            std::cout<<"matrix1 gen -> "<<matrix1.get_gen()<<std::endl<<std::endl;
        }
        std::cout<<"lets make mxv1 a slice \n";
        std::cout<<"note that when we want an matrix_view to literally become the other view, we must use assign flag, otherwise it will be tiling / direct assign\n";
        mxv1.A() = mxv1(sl{0, 3}, sl{2});
        mxv1 = 1;
        std::cout<<"changing matrix1 to 1 in the slice area\n";
        std::cout<<matrix1;

        std::cout<<"lets use assign flag on matrix and assign it its own copy, which will have same dims\n";
        matrix1.A() = matrix1.copy();

        if(!mxv1){
            std::cout<<"this printing means version has changed and the view is flagged\n";
            std::cout<<"mxv1 gen -> "<<mxv1.get_gen()<<std::endl;
            std::cout<<"matrix1 gen -> "<<matrix1.get_gen()<<std::endl<<std::endl;
        }
        std::cout<<"we know the dims are same so we can force update\n";
        mxv1.force_update();
        if(mxv1){
            std::cout<<"mxv1 gen -> "<<mxv1.get_gen()<<std::endl;
            std::cout<<"matrix1 gen -> "<<matrix1.get_gen()<<std::endl;
        }
        
        std::cout<<"changes matrix1 to 9 in the same slice area\n";
        mxv1 = 9;
        std::cout<<matrix1;
    }

    void test_operators()
    {
        std::cout<<"operator testing\n"<<std::endl;

        Matrix<int> matrix1{4, 4};
        Matrix<int> matrix2{4, 4};
        Matrix<double> matrix3{8, 8};

        randomize(matrix1, 1, 10);
        randomize(matrix2, 1, 10);
        randomize(matrix3, 1.0, 10.0);

        std::cout<<"matrix1"<<std::endl;
        std::cout<<matrix1;

        std::cout<<"matrix2"<<std::endl;
        std::cout<<matrix2;

        std::cout<<"matrix3"<<std::endl;
        std::cout<<matrix3<<std::endl;

        std::cout<<"member +=, -= *= /= with single value on other side"<<std::endl<<std::endl;

        std::cout<<"matrix1 += 2"<<std::endl;
        matrix1 += 2;
        std::cout<<matrix1;

        std::cout<<"matrix1 -= 7"<<std::endl;
        matrix1 -= 7;
        std::cout<<matrix1;

        std::cout<<"matrix1 *= 2.5"<<std::endl;
        matrix1 *= 2.5;
        std::cout<<matrix1;

        std::cout<<"matrix1 /= 5"<<std::endl;
        matrix1 /= 5.0;
        std::cout<<matrix1;

        std::cout<<"binary operations between matrices with +, - * / and val"<<std::endl;
        std::cout<<"nota that with binary operations the larger type (int, double) -> double remains"<<std::endl<<std::endl;
        std::cout<<matrix1+1.2;
        std::cout<<matrix2 - 3;
        std::cout<<matrix3 * 2;
        std::cout<<matrix1 / 1.5;

        std::cout<<"member +=, -= *= /= with other mx on other side"<<std::endl;
        std::cout<<"note that on some operations the other side is larger / smaller by dims, but always divisible"<<std::endl<<std::endl;
        matrix1 += matrix2;
        std::cout<<matrix1;

        matrix2 *= matrix2.copy();
        std::cout<<matrix2;

        matrix2 /= matrix1;
        std::cout<<matrix2;

        matrix3 -= matrix2;
        std::cout<<matrix3;

        std::cout<<"binary operations between matrices with +, - * / and matrix"<<std::endl;
        std::cout<<"nota that with binary operations the larger type (int, double) -> double remains, also left side of dims"<<std::endl<<std::endl;
        std::cout<<matrix1 + matrix2;
        std::cout<<matrix2 - matrix3;
        std::cout<<matrix3 * matrix1;
        std::cout<<matrix1 / matrix2;
    }

    void speed_test(size_t rows = 256, size_t cols = 256, size_t iterations = 1000)
    {
        Matrix<double> matrix1{rows, cols, 1};
        std::cout<<std::format("testing speed, matrix is ({}x{}), with {} iterations", rows, cols, iterations)<<std::endl<<std::endl;
        
        std::cout<<"one iter is dot product with its copy()"<<std::endl;
        
        auto start = std::chrono::steady_clock::now();

        for(size_t i{} ; i<iterations ; i++)
        {
            matrix1 = dot(matrix1, matrix1.copy());

            if(i % 10 == 0){
                auto end = std::chrono::steady_clock::now();
                std::cout<<end-start<<'\n';
                start = end;
            }
        }
    }
}

