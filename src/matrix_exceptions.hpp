#pragma once

#include <stdexcept>
#include <string>
#include <format>

class dimension_mismatch : public std::runtime_error
{
    public:
        dimension_mismatch(const std::string &message) 
        : std::runtime_error(message){}
};

class slicing_mismatch : public std::runtime_error
{
    public:
        slicing_mismatch(const std::string &message)
        : std::runtime_error(message){}
};

class indexing_mismatch : public std::runtime_error
{
    public:
        indexing_mismatch(const std::string &message)
        : std::runtime_error(message){}
};

