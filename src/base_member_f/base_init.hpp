#pragma once

#include "non_member_functions.hpp"

namespace machx
{
    template<typename Tmx, typename T>
    template<typename R, typename U>
    Tmx& Matrix_base<Tmx, T>::randomize_uniform(const R min, const U max){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        auto& mx = *this;
        
        auto min_v = static_cast<T>(min);
        auto max_v = static_cast<T>(max);
        
        if constexpr(std::is_integral_v<T>){
            std::uniform_int_distribution distrib(min_v, max_v);
            for(long long row{} ; row<rows ; row++){
                for(long long col{} ; col<cols ; col++){   
                    mx(row, col) = distrib(machx::generator);
                }
            }
        }
        else{
            std::uniform_real_distribution<T> distrib(min_v, max_v);
            for(long long row{} ; row<rows ; row++){
                for(long long col{} ; col<cols ; col++){   
                    mx(row, col) = distrib(machx::generator);
                }
            }
        }
        
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    template<typename R, typename U>
    Tmx& Matrix_base<Tmx, T>::randomize_normal(const R mean_, const U std_){
        auto rows = this->get_rows();
        auto cols = this->get_cols();
        auto& mx = *this;
        
        auto mean_v = static_cast<T>(mean_);
        auto std_v = static_cast<T>(std_);

        std::normal_distribution<T> distrib(mean_v, std_v);
        for(long long row{} ; row<rows ; row++){
            for(long long col{} ; col<cols ; col++){   
                mx(row, col) = distrib(machx::generator);
            }
        }
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::init_he_normal(long long features){
        features = (features<=0) ? this->get_cols() : features;
        auto sdev = std::sqrt(2.0 / features);
        this->randomize_normal(0, std::sqrt(2.0/features));
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::init_he_uniform(long long features){
        auto cols = this->get_cols();
        features = (features<=0) ? this->get_cols() : features;
        auto limit = std::sqrt(6.0 / features);
        this->randomize_uniform(-limit, limit);
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::init_xav_normal(long long features, long long output_features){
        features = (features<=0) ? this->get_cols() : features;
        output_features = (output_features<=0) ? this->get_cols() : output_features;
        auto sdev = std::sqrt(2.0 / (features+output_features));
        this->randomize_normal(0, sdev);
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::init_xav_uniform(long long features, long long output_features){
        features = (features<=0) ? this->get_cols() : features;
        output_features = (output_features<=0) ? this->get_cols() : output_features;
        auto limit = std::sqrt(6.0 / (features+output_features));
        this->randomize_uniform(-limit, limit);
        return static_cast<Tmx&>(*this);
    }
}
