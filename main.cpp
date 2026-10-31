#include "test_functions.hpp"
#include <iomanip>


int main()
{   
    namespace mxn = machine;
    try
    {
        //mxn::test_masking();
        //mxn::test_view();
        mxn::test_view_safety();
        //mxn::test_operators();
        //mxn::speed_test();
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
    catch(mxn::data_view_exception &e)
    {
        std::cout<<"data_view_exception\n";
        std::cout<<e.what();
    }
    catch(std::exception &e)
    {   
        std::cout<<"exception detected\n";
        std::cout<<e.what();
        return 1;
    };

    return 0;

}
