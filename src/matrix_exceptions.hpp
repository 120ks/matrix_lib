#pragma once

#include <stdexcept>
#include <string>
#include <format>
#include <matrix_base.hpp>
#include <matrix_subtypes.hpp>

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
}
