#include "mask_view.h"



Mask_view::Mask_view(const Matrix_base &mxb1, Mask m)
: mask{std::move(m)}
, mask_ptr{const_cast<double*>(mxb1.start_ptr())}
, index_arr(m.true_elements)
{
    for(size_t r{} ; r<m.rows ; r++){
        for(size_t c{} ; c<m.cols ; c++){
            if(m(r, c) == true) {index_arr.push_back(r*m.cols+c);}
        }
    }
}

Mask_view::Mask_view(Matrix_base &&base, Mask m)
: mask{std::move(m)}
, mask_ptr{base.start_ptr()}
, index_arr(m.true_elements)
{
}

Mask_view::Mask_view(Mask_view &&other)
: mask{std::move(other.mask)}
, mask_ptr{other.mask_ptr}
, index_arr(std::move(other.index_arr))
{
    other.mask_ptr = nullptr;
}

Mask_view::Mask_view(const Mask_view &other)
: mask{other.mask}
, mask_ptr{const_cast<double*>(other.mask_ptr)}
, index_arr(other.index_arr)
{
}

double& Mask_view::operator()(const size_t row, const size_t col){
    return *(mask_ptr + index_arr[row]);
}


double Mask_view::operator()(const size_t row, const size_t col) const{
    return *(mask_ptr + index_arr[row]);
}

