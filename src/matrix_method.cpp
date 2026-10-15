#include "matrix_method.h"
#include "matrix.h"


class Matrix;


Matrix Matrix_method::dot(const Matrix_base &mxb1, const Matrix_base &mxb2)
{
    assert(mxb1.get_cols() == mxb2.get_rows());

    auto r1{mxb1.get_rows()};
    auto c2{mxb2.get_cols()};
    auto r2c1{mxb2.get_rows()};

    Matrix mx{r1, c2};
    for(size_t r{} ; r<r1 ; r++){
        for(size_t c{} ; c<c2 ; c++){
            double dot_sum{};
            for(size_t k{} ; k<r2c1 ; k++){
                dot_sum += mxb1(r, k) * mxb2(k, c);
            }
            mx(r, c) = dot_sum;
        }
    }
    return mx;
}

bool Matrix_method::has_zero(const Matrix_base &mxb1)
{   
    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};

    for(size_t r{} ; r<r1 ; r++){
        for(size_t c{} ; c<c1 ; c++){
            if(mxb1(r, c) == 0){return true;}
        }
    }
    return false;
}

void Matrix_method::broadcast(Matrix_base &mxb1, const Matrix_base &mxb2, const Operation op)
{
    auto r1{mxb1.get_rows()}, r2{mxb2.get_rows()};
    auto c1{mxb1.get_cols()}, c2{mxb2.get_cols()};

    if(op == Operation::divide || op == Operation::root){
        if(has_zero(mxb2)){return;}
    }
    assert(r1!=0 && c1!=0 && r2!=0 && c2!=0);

    //lisätään nopeudelle suora täsmäys eli jos mxb1 ja mxb2 ovat ulottuvuudelta samat

    size_t max_row{std::max(r1, r2)};
    size_t max_col{std::max(c1, c2)};
    switch(op)
    {
        case(Operation::multiply):
        {
            for(size_t r{} ; r<max_row ; r++)
            {
                for(size_t c{} ; c<max_col ; c++)
                {
                    mxb1(r%r1, c%c1) = mxb1(r%r1, c%c1) * mxb2(r%r2, c%c2); 
                }
                
            }
            return;
        }
        case(Operation::divide):
        {
            for(size_t r{} ; r<max_row ; r++)
            {
                for(size_t c{} ; c<max_col; c++)
                {
                    mxb1(r%r1, c%c1) = mxb1(r%r1, c%c1) / mxb2(r%r2, c%c2);
                }
                
            }
            return;
        }
        case(Operation::add):
        {
            for(size_t r{} ; r<max_row ; r++)
            {
                for(size_t c{} ; c<max_col ; c++)
                {
                    mxb1(r%r1, c%c1) = mxb1(r%r1, c%c1) + mxb2(r%r2, c%c2);
                }
                
            }
            return;
        }               
        case(Operation::substract):
        {
            for(size_t r{} ; r<max_row ; r++)
            {
                for(size_t c{} ; c<max_col ; c++)
                {
                    mxb1(r%r1, c%c1) = mxb1(r%r1, c%c1) - mxb2(r%r2, c%c2);
                }
                
            }
            return;
        }
        case(Operation::equal):
        {
            for(size_t r{} ; r<max_row ; r++)
            {
                for(size_t c{} ; c<max_col ; c++)
                {
                    mxb1(r%r1, c%c1) = mxb2(r%r2, c%c2);
                }
                
            }
            return;
        }
        case(Operation::power):
        {
            for(size_t r{} ; r<max_row ; r++)
            {
                for(size_t c{} ; c<max_col ; c++)
                {
                    mxb1(r%r1, c%c1) = std::pow(mxb1(r%r1, c%c1), mxb2(r%r2, c%c2)); 
                }
                
            }
            return;
        }
        case(Operation::root):
        {
            for(size_t r{} ; r<max_row ; r++)
            {
                for(size_t c{} ; c<max_col ; c++)
                {
                    mxb1(r%r1, c%c1) = std::pow(mxb1(r%r1, c%c1), 1.0 / mxb2(r%r2, c%c2)); 
                }
            }
            return;
        }
    }
}

void Matrix_method::broadcast(Matrix_base &mxb1, const double val, Operation op)
{
    Matrix temp_val{1, 1, val};
    broadcast(mxb1, temp_val, op);
}

//
//for r-val inplace operaatiot, esim Matrix{std(mx1, true)({2, 3}, {3, 6}) + 2};
//
    Matrix Matrix_method::broadcast(const Matrix_base &mxb1, const Matrix_base &mxb2, const Operation op)
    {
        Matrix temp_mx{mxb1};
        broadcast(temp_mx, mxb2, op);
        return temp_mx;
    }

    Matrix Matrix_method::broadcast(const Matrix_base &mxb1, const double val, const Operation op)
    {
        Matrix temp_mx{mxb1};
        broadcast(temp_mx, val, op);
        return temp_mx;
    }


Matrix Matrix_method::copy(const Matrix_base &mxb1)
{   
    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};
    Matrix mx{r1, c1};
    for(size_t r{} ; r<r1 ; r++){
        for(size_t c{} ; c<c1 ; c++){
            mx(r, c) = mxb1(r, c);
        }
    }
    return mx;
}

void Matrix_method::print(const Matrix_base &mxb1)
{
    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};

    for(size_t r{} ; r<r1 ; r++){
        std::cout<<"[";
        for(size_t c{} ; c<c1 ; c++){
            if(c<c1-1){std::cout<<mxb1(r, c)<<" | ";}
            else{std::cout<<mxb1(r, c);}
        }
        std::cout<<"]\n";
    }
    std::cout<<'\n';
}

void Matrix_method::print(std::ostream &os, const Matrix_base &mxb1)
{   

    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};

    for(size_t r{} ; r<r1 ; r++){
        os<<"[";
        for(size_t c{} ; c<c1 ; c++){
            if(c<c1-1){os<<mxb1(r, c)<<" | ";}
            else{os<<mxb1(r, c);}
        }
        os<<"]\n";
    }
    os<<'\n';
}


Matrix Matrix_method::get_transposed(const Matrix_base& mxb1)
{
    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};
    Matrix mx_t{c1, r1};

    for(size_t r{} ; r<r1 ; r++){
        for(size_t c{} ; c<c1 ; c++){
            mx_t(c, r) = mxb1(r, c);
        }
    }
    return mx_t;
}

Matrix Matrix_method::filter(const Matrix_base &mxb1, const double filter_val, const double if_under, const double otherwise)
{
    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};
    Matrix mask{c1, r1};
    for(size_t r{} ; r<r1 ; r++){
        for(size_t c{} ; c<c1 ; c++){
            mask(r, c) = (mxb1(r,c) < filter_val) ? if_under : otherwise;
        }
    }
    return mask;
}
