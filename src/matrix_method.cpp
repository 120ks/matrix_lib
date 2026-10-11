#include "matrix_method.h"
#include "matrix.h"

class Matrix;


Matrix Matrix_method::dot(Matrix_base &mxb1, Matrix_base &mxb2)
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

void Matrix_method::broadcast(Matrix_base &mxb1, const Matrix_base &mxb2, Operation op, const bool tile)
{
    auto r1{mxb1.get_rows()}, r2{mxb2.get_rows()};
    auto c1{mxb1.get_cols()}, c2{mxb2.get_cols()};

    if(op == Operation::divide || op == Operation::root){
        if(has_zero(mxb2)){return;}
    }
    assert(r1!=0 && c1!=0 && r2!=0 && c2!=0);

    switch(op)
    {
        case(Operation::multiply):
        {
            for(size_t r{} ; r<std::max(r1, r2) ; r++)
            {
                size_t iter_r1 = (tile) ? r%r1 : std::min(r, r1-1);
                size_t iter_r2 = (tile) ? r%r2 : std::min(r, r2-1);

                for(size_t c{} ; c<std::max(c1, c2) ; c++)
                {
                    size_t iter_c1 = (tile) ? c%c1 : std::min(c, c1-1);
                    size_t iter_c2 = (tile) ? c%c2 : std::min(c, c2-1);

                    mxb1(iter_r1, iter_c1) = mxb1(iter_r1, iter_c1) * mxb2(iter_r2, iter_c2); 
                }
                
            }
            return;
        }
        case(Operation::divide):
        {
            for(size_t r{} ; r<std::max(r1, r2) ; r++)
            {
                size_t iter_r1 = (tile) ? r%r1 : std::min(r, r1-1);
                size_t iter_r2 = (tile) ? r%r2 : std::min(r, r2-1);

                for(size_t c{} ; c<std::max(c1, c2) ; c++)
                {
                    size_t iter_c1 = (tile) ? c%c1 : std::min(c, c1-1);
                    size_t iter_c2 = (tile) ? c%c2 : std::min(c, c2-1);

                    mxb1(iter_r1, iter_c1) = mxb1(iter_r1, iter_c1) / mxb2(iter_r2, iter_c2); 
                }
                
            }
            return;
        }
        case(Operation::add):
        {
            for(size_t r{} ; r<std::max(r1, r2) ; r++)
            {
                size_t iter_r1 = (tile) ? r%r1 : std::min(r, r1-1);
                size_t iter_r2 = (tile) ? r%r2 : std::min(r, r2-1);

                for(size_t c{} ; c<std::max(c1, c2) ; c++)
                {
                    size_t iter_c1 = (tile) ? c%c1 : std::min(c, c1-1);
                    size_t iter_c2 = (tile) ? c%c2 : std::min(c, c2-1);

                    mxb1(iter_r1, iter_c1) = mxb1(iter_r1, iter_c1)+mxb2(iter_r2, iter_c2); 
                }
                
            }
            return;
        }               
        case(Operation::substract):
        {
            for(size_t r{} ; r<std::max(r1, r2) ; r++)
            {
                size_t iter_r1 = (tile) ? r%r1 : std::min(r, r1-1);
                size_t iter_r2 = (tile) ? r%r2 : std::min(r, r2-1);

                for(size_t c{} ; c<std::max(c1, c2) ; c++)
                {
                    size_t iter_c1 = (tile) ? c%c1 : std::min(c, c1-1);
                    size_t iter_c2 = (tile) ? c%c2 : std::min(c, c2-1);

                    mxb1(iter_r1, iter_c1) = mxb1(iter_r1, iter_c1) - mxb2(iter_r2, iter_c2); 
                }
                
            }
            return;
        }
        case(Operation::equal):
        {
            for(size_t r{} ; r<std::max(r1, r2) ; r++)
            {
                size_t iter_r1 = (tile) ? r%r1 : std::min(r, r1-1);
                size_t iter_r2 = (tile) ? r%r2 : std::min(r, r2-1);

                for(size_t c{} ; c<std::max(c1, c2) ; c++)
                {
                    size_t iter_c1 = (tile) ? c%c1 : std::min(c, c1-1);
                    size_t iter_c2 = (tile) ? c%c2 : std::min(c, c2-1);

                    mxb1(iter_r1, iter_c1) =  mxb2(iter_r2, iter_c2); 
                }
                
            }
            return;
        }
        case(Operation::power):
        {
            for(size_t r{} ; r<std::max(r1, r2) ; r++)
            {
                size_t iter_r1 = (tile) ? r%r1 : std::min(r, r1-1);
                size_t iter_r2 = (tile) ? r%r2 : std::min(r, r2-1);

                for(size_t c{} ; c<std::max(c1, c2) ; c++)
                {
                    size_t iter_c1 = (tile) ? c%c1 : std::min(c, c1-1);
                    size_t iter_c2 = (tile) ? c%c2 : std::min(c, c2-1);

                    mxb1(iter_r1, iter_c1) = std::pow(mxb1(iter_r1, iter_c1), mxb2(iter_r2, iter_c2)); 
                }
                
            }
            return;
        }
        case(Operation::root):
        {
            for(size_t r{} ; r<std::max(r1, r2) ; r++)
            {
                size_t iter_r1 = (tile) ? r%r1 : std::min(r, r1-1);
                size_t iter_r2 = (tile) ? r%r2 : std::min(r, r2-1);

                for(size_t c{} ; c<std::max(c1, c2) ; c++)
                {
                    size_t iter_c1 = (tile) ? c%c1 : std::min(c, c1-1);
                    size_t iter_c2 = (tile) ? c%c2 : std::min(c, c2-1);

                    mxb1(iter_r1, iter_c1) = std::pow(mxb1(iter_r1, iter_c1), 1.0/mxb2(iter_r2, iter_c2)); 
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

double Matrix_method::sum(const Matrix_base &mxb1)
{
    Matrix mx{1, 1};
    broadcast(mx, mxb1, Operation::add);
    return mx(0,0);
}

Matrix Matrix_method::sum(const Matrix_base &mxb1, bool row_wise)
{   
    Matrix mx{(row_wise)? Matrix{mxb1.get_rows(), 1} : Matrix{1, mxb1.get_cols()}};
    broadcast(mx, mxb1, Operation::add);
    return mx;
}

double Matrix_method::avg(const Matrix_base &mxb1)
{   
    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};

    double avarage{sum(mxb1)};
    return avarage / (r1*c1);
}

Matrix Matrix_method::avg(const Matrix_base &mxb1, bool row_wise)
{
    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};

    Matrix avarage{sum(mxb1, (row_wise)? true : false)};
    broadcast(avarage, (row_wise)? c1 : r1, Operation::divide);
    return avarage;
}

double Matrix_method::std(const Matrix_base &mxb1, int degree)
{   
    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};
    double mean{avg(mxb1)};

    Matrix standard_deviation{mxb1};
    broadcast(standard_deviation, mean, Operation::substract);
    broadcast(standard_deviation, standard_deviation, Operation::multiply);
    double std_out{sum(standard_deviation)};
    std_out /= (r1*c1-degree);
    return std::sqrt(std_out);
}

Matrix Matrix_method::std(const Matrix_base &mxb1, bool row_wise, int degree)
{   
    auto r1{mxb1.get_rows()};
    auto c1{mxb1.get_cols()};
    Matrix mean{avg(mxb1, row_wise)};

    Matrix standard_deviation{mxb1};
    broadcast(standard_deviation, mean, Operation::substract);
    broadcast(standard_deviation, standard_deviation, Operation::multiply);
    Matrix std_out {sum(standard_deviation, row_wise)};
    broadcast(std_out, (row_wise)? c1-degree : r1-degree, Operation::divide);
    broadcast(std_out, 2.0, Operation::root);
    return std_out;
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
