#include "matrix.h"
#include <memory>

class Matrix;

class Matrix_mask : public Matrix_base
{
    private:
        const Matrix_base& mask_ref;
        const Condition condition;
        const double filter{};

        Matrix_mask(const Matrix_base &mx, Condition con, const double val);
        Matrix_mask(const Matrix_mask &other);
        Matrix_mask(Matrix_mask &&other)noexcept;

        //overloads
    public:
        double& operator()(const size_t row, const size_t col) override;
        double operator()(const size_t row, const size_t col)const override;
        Matrix_view operator()(const Slice, const Slice) override;

        inline size_t get_rows()const override { return mask_ref.get_rows();}
        inline size_t get_cols()const override { return mask_ref.get_rows();}

        bool check_condition(const double &val);

};
