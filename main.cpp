#include "mrx.h"

int main()
{
    //koodin testaus

    //matriisin luominen eri tavoin

    Matrix mx1{2, 3};
    std::cout<<mx1;

    Matrix mx2{3, 4, 1};
    std::cout<<mx2;

    Matrix mx3;
    mx3 = mx2*2; 
    std::cout<<mx3;

    Matrix mx4{mx3(0,3 , 0,2)};
    std::cout<<mx4;

    Matrix mx5 = mx3(0,1,0,1);
    std::cout<<mx5;

    
    return 0;
}
