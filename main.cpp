#include "mx.hpp"

int main()
{
    //koodin testaus
    //matriisin luominen eri tavoin

    Matrix<double> mx{10, 12 ,0};
    mx(sl{}, sl{1}) = 20;
    std::cerr<<mx;
    mx::mask mask = ((mx(sl{}, {3, 7}))>3);

    std::cerr<<((!(mx>0)) && (mx>0))(sl{}, {3, 6});


    return 0;
}
