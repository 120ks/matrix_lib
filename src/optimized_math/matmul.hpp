#pragma once

#include "non_member_functions.hpp"
#include "micros_ps.hpp"
#include "micros_pd.hpp"
#include <iostream>
#include <algorithm>


// Overload for float
namespace machx{
    void matmul_ps(const float* __restrict mx1, const float* __restrict mx2, float* __restrict mx3, int ks, int rows, int cols){
        //MICRO sizes, row, col, k
        constexpr int ROW_M = 64; //keep divisible by 4, when calling ps 4x16
        constexpr int COL_M = 64; //KEEP divisible by 16
        constexpr int K_M = 64; //adjustable

        #pragma omp parallel for collapse(2)
        for(int row_batch = 0 ; row_batch<rows ; row_batch += ROW_M){
            for(int col_batch = 0 ; col_batch<cols ; col_batch += COL_M){
                int row_end = std::min(ROW_M+row_batch, rows);
                int col_end = std::min(COL_M+col_batch, cols);
                for(int k_batch{} ; k_batch<ks ; k_batch += K_M){
                    int local_k = std::min(K_M, ks-k_batch);
                    
                    int row = row_batch;
                    for(; row <= row_end- 4 ; row += 4){
                        int col = col_batch;
                        for(; col<=col_end-16 ; col += 16){
                            machx::micro::prec1::matmul_ps_4x16(&mx1[row*ks+k_batch], &mx2[k_batch*cols + col], &mx3[row*cols + col], cols, ks, local_k);
                        }
                        //cols remainer
                        if(col<col_end){
                            for(int r = row ; r<row+4 ; r++){
                                for(int k = k_batch ; k<k_batch+local_k ; k++){
                                    for(int c = col; c<col_end ; c++){
                                        mx3[r*cols + c] += mx1[r*ks + k] * mx2[k*cols + c];
                                    }
                                }
                            }
                        }
                    }
                    for(int r = row ; r<row_end ; r++){
                        for(int k = k_batch ; k<k_batch+local_k ; k++){
                            for(int c = col_batch; c<col_end ; c++){
                                mx3[r*cols + c] += mx1[r*ks + k] * mx2[k*cols + c];
                            }
                        }
                    }
                }
            }
        }
    }
    __attribute__((target("avx2,fma")))
    void matmul_ps_pack(const float* __restrict mx1, const float* __restrict mx2, float* __restrict mx3, int ks, int rows, int cols){
        //MICRO sizes, row, col, k
        constexpr int ROW_M = 64; //keep divisible by 4, when calling ps 4x16
        constexpr int COL_M = 64; //KEEP divisible by 16
        constexpr int K_M = 64; //adjustable

        std::vector<float> mx2p(ks * cols);
        #pragma omp parallel for collapse(2) schedule(dynamic)
        for(int col_batch = 0; col_batch < cols; col_batch += COL_M){
            for(int k_batch = 0; k_batch < ks; k_batch += K_M){
                int col_end = std::min(COL_M+col_batch, cols);
                int local_k = std::min(K_M, ks - k_batch);
                
                for(int k = k_batch; k<k_batch+local_k; k++){
                    int pack = col_batch/16;
                    for(int col = col_batch; col<=col_end-16 ; col+=16){
                        //include vectorized section
                        __m256 MX2V00 = _mm256_loadu_ps(&mx2[k*cols + col]);
                        __m256 MX2V04 = _mm256_loadu_ps(&mx2[k*cols + col+8]);
                        _mm256_storeu_ps(&mx2p[pack*(ks*16) + k*16], MX2V00);
                        _mm256_storeu_ps(&mx2p[pack*(ks*16) + k*16 + 8], MX2V04);
                        pack++;
                    }
                }
            }
        }

        #pragma omp parallel for collapse(2) schedule(dynamic, 8)
        for(int row_batch = 0 ; row_batch<rows ; row_batch += ROW_M){
            for(int col_batch = 0 ; col_batch<cols ; col_batch += COL_M){
                int row_end = std::min(ROW_M+row_batch, rows);
                int col_end = std::min(COL_M+col_batch, cols);
                for(int k_batch = 0 ; k_batch<ks ; k_batch += K_M){
                    
                    int local_k = std::min(K_M, ks-k_batch);
                    int row = row_batch;
                    //multithread here potentially (other function)
                    for(; row <= row_end- 4 ; row += 4){
                        int col = col_batch;
                        int local_col = col/16;
                        for(; col<=col_end-16 ; col += 16){
                            machx::micro::prec1::matmul_ps_4x16_packed(&mx1[row*ks+k_batch], &mx2p[local_col*(ks*16)+k_batch*16], &mx3[row*cols + col], cols, ks, local_k);
                            local_col++;
                        }
                        //cols remainer
                        if(col<col_end){
                            for(int r = row ; r<row+4 ; r++){
                                for(int k = k_batch ; k<k_batch+local_k ; k++){
                                    for(int c = col; c<col_end ; c++){
                                        mx3[r*cols + c] += mx1[r*ks + k] * mx2[k*cols + c];
                                    }
                                }
                            }
                        }   
                    }
                    for(int r = row ; r<row_end ; r++){
                        for(int k = k_batch ; k<k_batch+local_k ; k++){
                            for(int c = col_batch; c<col_end ; c++){
                                mx3[r*cols + c] += mx1[r*ks + k] * mx2[k*cols + c];
                            }
                        }
                    }
                }
            }
        }
    }

    void matmul_pd(const double* __restrict mx1, const double* __restrict mx2, double* __restrict mx3, int ks, int rows, int cols){
        //MICRO sizes, row, col, k
        constexpr int ROW_M = 32; //keep divisible by 4, when calling ps 4x16
        constexpr int COL_M = 32; //KEEP divisible by 8
        constexpr int K_M = 32; //adjustable

        #pragma omp parallel for collapse(2)
        for(int row_batch = 0 ; row_batch<rows ; row_batch += ROW_M){
            for(int col_batch = 0 ; col_batch<cols ; col_batch += COL_M){
                int row_end = std::min(ROW_M+row_batch, rows);
                int col_end = std::min(COL_M+col_batch, cols);
                for(int k_batch{} ; k_batch<ks ; k_batch += K_M){
                    int local_k = std::min(K_M, ks-k_batch);

                    int row = row_batch;
                    for(; row <= row_end- 4 ; row += 4){
                        int col = col_batch;
                        int local_col = 0;
                        for(; col<=col_end-8 ; col += 8){
                            machx::micro::prec2::matmul_pd_4x8(&mx1[row*ks+k_batch], &mx2[k_batch*cols + col], &mx3[row*cols + col], cols, ks, local_k);
                            local_col += 8;
                        }
                        //cols remainer
                        if(col<col_end){
                            for(int r = row ; r<row+4 ; r++){
                                for(int k = k_batch ; k<k_batch+local_k ; k++){
                                    for(int c = col; c<col_end ; c++){
                                        mx3[r*cols + c] += mx1[r*ks + k] * mx2[k*cols + c];
                                    }
                                }
                            }
                        }   
                    }
                    for(int r = row ; r<row_end ; r++){
                        for(int k = k_batch ; k<k_batch+local_k ; k++){
                            for(int c = col_batch; c<col_end ; c++){
                                mx3[r*cols + c] += mx1[r*ks + k] * mx2[k*cols + c];
                            }
                        }
                    }
                }
            }
        }
    }
    __attribute__((target("avx,fma")))
    void matmul_pd_pack(const double* __restrict mx1, const double* __restrict mx2, double* __restrict mx3, int ks, int rows, int cols){
        constexpr int ROW_M = 32; //keep div by 4
        constexpr int COL_M = 32; //KEEP div by 8
        constexpr int K_M = 32; //adjustable

        std::vector<double> mx2p(ks * cols);
        #pragma omp parallel for collapse(2)
        for(int col_batch = 0; col_batch < cols; col_batch += COL_M){
            for(int k_batch = 0; k_batch < ks; k_batch += K_M){
                int col_end = std::min(COL_M+col_batch, cols);
                int local_k = std::min(K_M, ks - k_batch);
                
                for(int k = k_batch; k<k_batch+local_k; k++){
                    int pack = col_batch/8;
                    for(int col = col_batch; col<=col_end-8 ; col+=8){
                        //include vectorized section
                        __m256d MX2V00 = _mm256_loadu_pd(&mx2[k*cols + col]);
                        __m256d MX2V04 = _mm256_loadu_pd(&mx2[k*cols + col+4]);
                        _mm256_storeu_pd(&mx2p[pack*(ks*8) + k*8], MX2V00);
                        _mm256_storeu_pd(&mx2p[pack*(ks*8) + k*8 + 4], MX2V04);
                        pack++;
                    }
                }
            }
        }
        
        #pragma omp parallel for collapse(2) schedule(dynamic)
        for(int row_batch = 0 ; row_batch<rows ; row_batch += ROW_M){
            for(int col_batch = 0 ; col_batch<cols ; col_batch += COL_M){
                int row_end = std::min(ROW_M+row_batch, rows);
                int col_end = std::min(COL_M+col_batch, cols);
                for(int k_batch = 0 ; k_batch<ks ; k_batch += K_M){
                    int local_k = std::min(K_M, ks-k_batch);

                    int row = row_batch;
                    for(; row <= row_end- 4 ; row += 4){
                        int col = col_batch;
                        int local_col = col/8;
                        for(; col<=col_end-8 ; col += 8){
                            machx::micro::prec2::matmul_pd_4x8_packed(&mx1[row*ks+k_batch], &mx2p[local_col*(ks*8)+k_batch*8], &mx3[row*cols + col], cols, ks, local_k);
                            local_col++;
                        }
                        //cols remainer
                        if(col<col_end){
                            for(int r = row ; r<row+4 ; r++){
                                for(int k = k_batch ; k<k_batch+local_k ; k++){
                                    for(int c = col; c<col_end ; c++){
                                        mx3[r*cols + c] += mx1[r*ks + k] * mx2[k*cols + c];
                                    }
                                }
                            }
                        }   
                    }
                    for(int r = row ; r<row_end ; r++){
                        for(int k = k_batch ; k<k_batch+local_k ; k++){
                            for(int c = col_batch; c<col_end ; c++){
                                mx3[r*cols + c] += mx1[r*ks + k] * mx2[k*cols + c];
                            }
                        }
                    }
                }
            }
        }
    }
}//namespace



