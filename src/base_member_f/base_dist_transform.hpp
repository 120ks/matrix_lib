#pragma once

#include "non_member_functions.hpp"

namespace machx
{   
    //non linear transformation  

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::rank(bool ascending, bool dp_share){

        auto rows = this->get_rows();
        auto cols = this->get_cols();

        long long start{0};
        auto rank_f = [&](auto self, machx::Mask_view<T> mv) -> void {
            if(mv.get_cols() == 0){
                return;
            }
            if(mv.get_cols() == 1){
                mv(0, 0) = start++;
                return;
            }
            auto pivot = mv(0, 0);
            if(ascending){
                auto left_mask = mv<pivot;
                auto right_mask = mv>pivot;

                auto left = mv[left_mask];
                auto right = mv[right_mask];
                auto middle = mv[!(left_mask || right_mask)];

                self(self, left);

                if(dp_share){ //duplicates share
                    middle = start++;
                }
                else{
                    for(long long col{} ; col<middle.get_cols() ; col++){
                        middle(0, col) = start++;
                    }
                }
                self(self, right);
                return;
            }
            else{
                auto left_mask = mv>pivot;
                auto right_mask = mv<pivot;

                auto left = mv[left_mask];
                auto right = mv[right_mask];
                auto middle = mv[!(left_mask || right_mask)];

                self(self, left);
                if(dp_share){
                    middle = start++;
                }
                else{
                    for(long long col{} ; col<middle.get_cols() ; col++){
                        middle(0, col) = start++;
                    }
                }
                self(self, right);
                return;
            }
        };
        Matrix<bool> mask{rows, cols, true};
        rank_f(rank_f, this -> operator[](mask));

        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::quantile(){
        this->rank();
        this->min_max(0, 1);
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::bucket(long long buckets){
        this->quantile();
        double bucket_threshold = 1.0/buckets;
        double start{0};
        Matrix_base<Tmx, T>& mx = *this;
        auto copy_mx = mx.copy();
        for(long long buc{0} ; buc < buckets ; buc++){
            copy_mx[mx>start] = static_cast<T>(buc);
            start += bucket_threshold;
        }
        *this = copy_mx;
        return static_cast<Tmx&>(*this);
    }

    template<typename Tmx, typename T>
    Tmx& Matrix_base<Tmx, T>::fit_gaussian(){
        auto rows = this->get_rows();
        auto cols = this->get_cols();

        this->rank();
        this->z_scores();
        Matrix<double> e_mx{rows, cols, machx::E};
        auto power_mx = this->power(2) /= 2;
        power_mx *= -1;
        e_mx.power(power_mx);
        (*this) = e_mx;

        (*this) /= std::sqrt(2*machx::PI);
        return static_cast<Tmx&>(*this);
    }
}
