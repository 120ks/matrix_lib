#pragma once

#include <stdexcept>
#include <string>
#include <format>

#include "matrix_base.hpp"
#include "matrix_subtypes.hpp"

namespace machine
{   
    template<typename>
    class Matrix_base;
    class dimension_mismatch : public std::runtime_error
    {
        public:
            dimension_mismatch(const std::string &message) 
            : std::runtime_error(message){}

            template<typename T, typename R>
            dimension_mismatch(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2, std::string message = "")
            : std::runtime_error(std::format("dimension mismatch with mxb1({}x{}) and mxb2({}x{}): "
                , mxb1.get_rows(), mxb1.get_cols(), mxb2.get_rows(), mxb2.get_cols()) + message){}
    };

    class slicing_mismatch : public std::runtime_error
    {
        public:
            slicing_mismatch(const std::string &message)
            : std::runtime_error(message){}

            template<typename T>
            slicing_mismatch(const Matrix_base<T> &mxb1, const Slice r_sl, const Slice c_sl, std::string message = "")
            : std::runtime_error(std::format("slicing mismatch with mxb1({}x{}) and slices row_slice({}, {}) and col_slice({}, {}):  "
                , mxb1.get_rows(), mxb1.get_cols(), r_sl.begin, r_sl.end, c_sl.begin, c_sl.end) + message){}
    };

    class indexing_mismatch : public std::runtime_error
    {
        public:
            indexing_mismatch(const std::string &message)
            : std::runtime_error(message){}

            template<typename T>
            indexing_mismatch(const Matrix_base<T> &mxb1, size_t row_index, size_t col_index, std::string message = "")
            : std::runtime_error(std::format("indexint mxb1({}x{}, with row_index {}, col_index {})", mxb1.get_rows(), mxb1.get_cols(), row_index, col_index) + message){}
    };

    class data_view_exception : public std::runtime_error
    {
        public:
            data_view_exception(const std::string &message)
            : std::runtime_error(message){}

            data_view_exception(const size_t id, std::string message = "")
            : std::runtime_error(std::format("trying to access data that does not exist in matrix with id {}: ", id) + message){}
    };
}

namespace machine
{
    template<typename T, typename R>
    bool same_dims(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2)
    {
        return (mxb1.get_rows() == mxb2.get_rows()) && (mxb1.get_cols() == mxb2.get_cols());
    }

    template<typename T, typename R>
    bool rows_and_cols_are(const Matrix<T> &mxb1, const size_t rows, const size_t cols)
    {
        return ((mxb1.get_rows() == rows) && mxb1.get_cols() == cols);
    }
}

namespace machine::quick_throw
{   

    template<typename T>
    inline void throw_if_no_data(const Matrix_base<T> &mxb1, std::string message = "")
    {
        if(!mxb1)
        {
            throw data_view_exception{mxb1.get_id(), message};
        }
    }

    template<typename T, typename R>
    inline void throw_if_dimension_mismatch(const Matrix_base<T> &mxb1, const Matrix_base<R> &mxb2, std::string message = "")
    {
        if(!same_dims(mxb1, mxb2)){
            throw dimension_mismatch{mxb1, mxb2, message};
        }
    }

    template<typename T>
    inline void throw_if_slice_mismatch(const Matrix_base<T> &mxb1, Slice sl1, Slice sl2, std::string message = "")
    {
        if((mxb1.get_rows()<sl1.end) || (mxb1.get_cols()<sl2.end))
        {
            throw slicing_mismatch{mxb1, sl1, sl2, message};
        }
    }

    template<typename T>
    inline void throw_if_indexing_mismatch(const Matrix_base<T> &mxb1, size_t row, size_t col, std::string message = "")
    {
        if((row>= mxb1.get_rows()) || (col >= mxb1.get_cols())){
            throw indexing_mismatch(mxb1, row, col, message);
        }
    }
}
