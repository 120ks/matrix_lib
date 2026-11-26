#pragma once
#include <immintrin.h>


namespace machx::micro::prec2
{   

    //packed operations
    __attribute__((target("avx,fma")))
    inline void matmul_pd_4x8(const double* __restrict MX1, const double* __restrict MX2, double* __restrict MX3, int cols, int global_ks, int local_ks){
    //MX3 result vectors R's

        __m256d MX3V00 = _mm256_loadu_pd(&MX3[0 * cols + 0]), MX3V04 = _mm256_loadu_pd(&MX3[0 * cols + 4]);
        __m256d MX3V10 = _mm256_loadu_pd(&MX3[1 * cols + 0]), MX3V14 = _mm256_loadu_pd(&MX3[1 * cols + 4]);
        __m256d MX3V20 = _mm256_loadu_pd(&MX3[2 * cols + 0]), MX3V24 = _mm256_loadu_pd(&MX3[2 * cols + 4]);
        __m256d MX3V30 = _mm256_loadu_pd(&MX3[3 * cols + 0]), MX3V34 = _mm256_loadu_pd(&MX3[3 * cols + 4]);

        for(int k{}; k<local_ks ; k++){    
            //LOAD CORRECT MX1 trough broadcast(4 different), given if k = 0, takes the first col of all rows in row_block
            //prefetch next

            __m256d MX1V00 = _mm256_set1_pd(MX1[0*global_ks + k]), MX1V10 = _mm256_set1_pd(MX1[1*global_ks + k]);
            __m256d MX1V20 = _mm256_set1_pd(MX1[2*global_ks + k]), MX1V30 = _mm256_set1_pd(MX1[3*global_ks + k]);

            //LOAD corresponding col blocks //packed so aligned in memory at every 16 steps
            __m256d MX2V00 = _mm256_loadu_pd(&MX2[k*cols]);
            __m256d MX2V04 = _mm256_loadu_pd(&MX2[k*cols+4]);

            MX3V00 = _mm256_fmadd_pd(MX1V00, MX2V00, MX3V00);
            MX3V04 = _mm256_fmadd_pd(MX1V00, MX2V04, MX3V04);

            MX3V10 = _mm256_fmadd_pd(MX1V10, MX2V00, MX3V10);
            MX3V14 = _mm256_fmadd_pd(MX1V10, MX2V04, MX3V14);

            MX3V20 = _mm256_fmadd_pd(MX1V20, MX2V00, MX3V20);
            MX3V24 = _mm256_fmadd_pd(MX1V20, MX2V04, MX3V24);

            MX3V30 = _mm256_fmadd_pd(MX1V30, MX2V00, MX3V30);
            MX3V34 = _mm256_fmadd_pd(MX1V30, MX2V04, MX3V34);
            
        }
        //store result back to mx3 ptrs
        _mm256_storeu_pd(&MX3[0 * cols + 0], MX3V00); _mm256_storeu_pd(&MX3[0 * cols + 4], MX3V04);
        _mm256_storeu_pd(&MX3[1 * cols + 0], MX3V10); _mm256_storeu_pd(&MX3[1 * cols + 4], MX3V14);
        _mm256_storeu_pd(&MX3[2 * cols + 0], MX3V20); _mm256_storeu_pd(&MX3[2 * cols + 4], MX3V24);
        _mm256_storeu_pd(&MX3[3 * cols + 0], MX3V30); _mm256_storeu_pd(&MX3[3 * cols + 4], MX3V34);
    }

    //packed operations
    __attribute__((target("avx,fma")))
    inline void matmul_pd_4x8_packed(const double* __restrict MX1, const double* __restrict MX2, double* __restrict MX3, int cols, int global_ks, int local_ks){
    //MX3 result vectors R's

        __m256d MX3V00 = _mm256_loadu_pd(&MX3[0 * cols + 0]), MX3V04 = _mm256_loadu_pd(&MX3[0 * cols + 4]);
        __m256d MX3V10 = _mm256_loadu_pd(&MX3[1 * cols + 0]), MX3V14 = _mm256_loadu_pd(&MX3[1 * cols + 4]);
        __m256d MX3V20 = _mm256_loadu_pd(&MX3[2 * cols + 0]), MX3V24 = _mm256_loadu_pd(&MX3[2 * cols + 4]);
        __m256d MX3V30 = _mm256_loadu_pd(&MX3[3 * cols + 0]), MX3V34 = _mm256_loadu_pd(&MX3[3 * cols + 4]);

        for(int k{}; k<local_ks ; k++){    
            //LOAD CORRECT MX1 trough broadcast(4 different), given if k = 0, takes the first col of all rows in row_block
            //prefetch next

            __m256d MX1V00 = _mm256_set1_pd(MX1[0*global_ks + k]), MX1V10 = _mm256_set1_pd(MX1[1*global_ks + k]);
            __m256d MX1V20 = _mm256_set1_pd(MX1[2*global_ks + k]), MX1V30 = _mm256_set1_pd(MX1[3*global_ks + k]);

            //LOAD corresponding col blocks //packed so aligned in memory at every 16 steps
            __m256d MX2V00 = _mm256_loadu_pd(&MX2[k*8]);
            __m256d MX2V04 = _mm256_loadu_pd(&MX2[k*8+4]);

            MX3V00 = _mm256_fmadd_pd(MX1V00, MX2V00, MX3V00);
            MX3V04 = _mm256_fmadd_pd(MX1V00, MX2V04, MX3V04);

            MX3V10 = _mm256_fmadd_pd(MX1V10, MX2V00, MX3V10);
            MX3V14 = _mm256_fmadd_pd(MX1V10, MX2V04, MX3V14);

            MX3V20 = _mm256_fmadd_pd(MX1V20, MX2V00, MX3V20);
            MX3V24 = _mm256_fmadd_pd(MX1V20, MX2V04, MX3V24);

            MX3V30 = _mm256_fmadd_pd(MX1V30, MX2V00, MX3V30);
            MX3V34 = _mm256_fmadd_pd(MX1V30, MX2V04, MX3V34);
            
        }
        //store result back to mx3 ptrs
        _mm256_storeu_pd(&MX3[0 * cols + 0], MX3V00); _mm256_storeu_pd(&MX3[0 * cols + 4], MX3V04);
        _mm256_storeu_pd(&MX3[1 * cols + 0], MX3V10); _mm256_storeu_pd(&MX3[1 * cols + 4], MX3V14);
        _mm256_storeu_pd(&MX3[2 * cols + 0], MX3V20); _mm256_storeu_pd(&MX3[2 * cols + 4], MX3V24);
        _mm256_storeu_pd(&MX3[3 * cols + 0], MX3V30); _mm256_storeu_pd(&MX3[3 * cols + 4], MX3V34);
    }
}
