#pragma once
#include <immintrin.h>



namespace machx::micro::prec1{

//FLOATING 4X16 //no packing //might remove (or save to storage)
    __attribute__((target("avx2,fma")))
    inline void matmul_ps_4x16(const float* __restrict MX1, const float* __restrict MX2, float* __restrict MX3, int cols, int global_ks, int local_ks){
    //MX3 result vectors R's

        __m256 MX3V00 = _mm256_loadu_ps(&MX3[0 * cols + 0]), MX3V08 = _mm256_loadu_ps(&MX3[0 * cols + 8]);
        __m256 MX3V10 = _mm256_loadu_ps(&MX3[1 * cols + 0]), MX3V18 = _mm256_loadu_ps(&MX3[1 * cols + 8]);
        __m256 MX3V20 = _mm256_loadu_ps(&MX3[2 * cols + 0]), MX3V28 = _mm256_loadu_ps(&MX3[2 * cols + 8]);
        __m256 MX3V30 = _mm256_loadu_ps(&MX3[3 * cols + 0]), MX3V38 = _mm256_loadu_ps(&MX3[3 * cols + 8]);

        for(int k{}; k<local_ks ; k++){    
            //LOAD CORRECT MX1 trough broadcast(4 different), given if k = 0, takes the first col of all rows in row_block
            //prefetch next

            __m256 MX1V00 = _mm256_set1_ps(MX1[0*global_ks + k]), MX1V10 = _mm256_set1_ps(MX1[1*global_ks + k]);
            __m256 MX1V20 = _mm256_set1_ps(MX1[2*global_ks + k]), MX1V30 = _mm256_set1_ps(MX1[3*global_ks + k]);

            //LOAD corresponding col blocks
            __m256 MX2V00 = _mm256_loadu_ps(&MX2[k*cols]);
            __m256 MX2V08 = _mm256_loadu_ps(&MX2[k*cols+8]);

            MX3V00 = _mm256_fmadd_ps(MX1V00, MX2V00, MX3V00);
            MX3V08 = _mm256_fmadd_ps(MX1V00, MX2V08, MX3V08);

            MX3V10 = _mm256_fmadd_ps(MX1V10, MX2V00, MX3V10);
            MX3V18 = _mm256_fmadd_ps(MX1V10, MX2V08, MX3V18);

            MX3V20 = _mm256_fmadd_ps(MX1V20, MX2V00, MX3V20);
            MX3V28 = _mm256_fmadd_ps(MX1V20, MX2V08, MX3V28);

            MX3V30 = _mm256_fmadd_ps(MX1V30, MX2V00, MX3V30);
            MX3V38 = _mm256_fmadd_ps(MX1V30, MX2V08, MX3V38);
            
        }
        //store result back to mx3 ptrs
        _mm256_storeu_ps(&MX3[0 * cols + 0], MX3V00); _mm256_storeu_ps(&MX3[0 * cols + 8], MX3V08);
        _mm256_storeu_ps(&MX3[1 * cols + 0], MX3V10); _mm256_storeu_ps(&MX3[1 * cols + 8], MX3V18);
        _mm256_storeu_ps(&MX3[2 * cols + 0], MX3V20); _mm256_storeu_ps(&MX3[2 * cols + 8], MX3V28);
        _mm256_storeu_ps(&MX3[3 * cols + 0], MX3V30); _mm256_storeu_ps(&MX3[3 * cols + 8], MX3V38);
    }

    //packed operations
    __attribute__((target("avx2,fma")))
    inline void matmul_ps_4x16_packed(const float* __restrict MX1, const float* __restrict MX2, float* __restrict MX3, int cols, int global_ks, int local_ks){
    //MX3 result vectors R's

        __m256 MX3V00 = _mm256_loadu_ps(&MX3[0 * cols + 0]), MX3V08 = _mm256_loadu_ps(&MX3[0 * cols + 8]);
        __m256 MX3V10 = _mm256_loadu_ps(&MX3[1 * cols + 0]), MX3V18 = _mm256_loadu_ps(&MX3[1 * cols + 8]);
        __m256 MX3V20 = _mm256_loadu_ps(&MX3[2 * cols + 0]), MX3V28 = _mm256_loadu_ps(&MX3[2 * cols + 8]);
        __m256 MX3V30 = _mm256_loadu_ps(&MX3[3 * cols + 0]), MX3V38 = _mm256_loadu_ps(&MX3[3 * cols + 8]);

        for(int k{}; k<local_ks ; k++){    
            //LOAD CORRECT MX1 trough broadcast(4 different), given if k = 0, takes the first col of all rows in row_block
            //prefetch next

            __m256 MX1V00 = _mm256_set1_ps(MX1[0*global_ks + k]), MX1V10 = _mm256_set1_ps(MX1[1*global_ks + k]);
            __m256 MX1V20 = _mm256_set1_ps(MX1[2*global_ks + k]), MX1V30 = _mm256_set1_ps(MX1[3*global_ks + k]);

            //LOAD corresponding col blocks //packed so aligned in memory at every 16 steps
            __m256 MX2V00 = _mm256_loadu_ps(&MX2[k*16]);
            __m256 MX2V08 = _mm256_loadu_ps(&MX2[k*16+8]);

            MX3V00 = _mm256_fmadd_ps(MX1V00, MX2V00, MX3V00);
            MX3V08 = _mm256_fmadd_ps(MX1V00, MX2V08, MX3V08);

            MX3V10 = _mm256_fmadd_ps(MX1V10, MX2V00, MX3V10);
            MX3V18 = _mm256_fmadd_ps(MX1V10, MX2V08, MX3V18);

            MX3V20 = _mm256_fmadd_ps(MX1V20, MX2V00, MX3V20);
            MX3V28 = _mm256_fmadd_ps(MX1V20, MX2V08, MX3V28);

            MX3V30 = _mm256_fmadd_ps(MX1V30, MX2V00, MX3V30);
            MX3V38 = _mm256_fmadd_ps(MX1V30, MX2V08, MX3V38);
            
        }
        //store result back to mx3 ptrs
        _mm256_storeu_ps(&MX3[0 * cols + 0], MX3V00); _mm256_storeu_ps(&MX3[0 * cols + 8], MX3V08);
        _mm256_storeu_ps(&MX3[1 * cols + 0], MX3V10); _mm256_storeu_ps(&MX3[1 * cols + 8], MX3V18);
        _mm256_storeu_ps(&MX3[2 * cols + 0], MX3V20); _mm256_storeu_ps(&MX3[2 * cols + 8], MX3V28);
        _mm256_storeu_ps(&MX3[3 * cols + 0], MX3V30); _mm256_storeu_ps(&MX3[3 * cols + 8], MX3V38);
    }


    //BROADCASTS
    __attribute__((target("avx2,fma")))
    void broadcast_ps_4x8_mul(float* __restrict MX1, const float* __restrict MX2, int block, int cols){

        int b = 0;
        for(; b<=block-8; b+=8){
            //MX1
            __m256 MX1V0 = _mm256_loadu_ps(&MX1[0*cols + b]);
            __m256 MX1V1 = _mm256_loadu_ps(&MX1[1*cols + b]);
            __m256 MX1V2 = _mm256_loadu_ps(&MX1[2*cols + b]);
            __m256 MX1V3 = _mm256_loadu_ps(&MX1[3*cols + b]);

            //MX2
            __m256 MX2V0 = _mm256_loadu_ps(&MX2[0*cols + b]);
            __m256 MX2V1 = _mm256_loadu_ps(&MX2[1*cols + b]);
            __m256 MX2V2 = _mm256_loadu_ps(&MX2[2*cols + b]);
            __m256 MX2V3 = _mm256_loadu_ps(&MX2[3*cols + b]);

            __m256 R0 = _mm256_mul_ps(MX1V0, MX2V0);
            __m256 R1 = _mm256_mul_ps(MX1V1, MX2V1);
            __m256 R2 = _mm256_mul_ps(MX1V2, MX2V2);
            __m256 R3 = _mm256_mul_ps(MX1V3, MX2V3);

            _mm256_storeu_ps(&MX1[0*cols + b], R0);
            _mm256_storeu_ps(&MX1[1*cols + b], R1);
            _mm256_storeu_ps(&MX1[2*cols + b], R2);
            _mm256_storeu_ps(&MX1[3*cols + b], R3);
        }
        for(; b<block ; b++){
            MX1[0*cols + b] *= MX2[0*cols + b];
            MX1[1*cols + b] *= MX2[1*cols + b];
            MX1[2*cols + b] *= MX2[2*cols + b];
            MX1[3*cols + b] *= MX2[3*cols + b];
        }
    }

    __attribute__((target("avx2,fma")))
    void broadcast_ps_4x8_add(float* __restrict MX1, const float* __restrict MX2, int block, int cols){

        int b = 0;
        for(; b<=block-8; b+=8){
            //MX1
            __m256 MX1V0 = _mm256_loadu_ps(&MX1[0*cols + b]);
            __m256 MX1V1 = _mm256_loadu_ps(&MX1[1*cols + b]);
            __m256 MX1V2 = _mm256_loadu_ps(&MX1[2*cols + b]);
            __m256 MX1V3 = _mm256_loadu_ps(&MX1[3*cols + b]);

            //MX2
            __m256 MX2V0 = _mm256_loadu_ps(&MX2[0*cols + b]);
            __m256 MX2V1 = _mm256_loadu_ps(&MX2[1*cols + b]);
            __m256 MX2V2 = _mm256_loadu_ps(&MX2[2*cols + b]);
            __m256 MX2V3 = _mm256_loadu_ps(&MX2[3*cols + b]);

            __m256 R0 = _mm256_add_ps(MX1V0, MX2V0);
            __m256 R1 = _mm256_add_ps(MX1V1, MX2V1);
            __m256 R2 = _mm256_add_ps(MX1V2, MX2V2);
            __m256 R3 = _mm256_add_ps(MX1V3, MX2V3);

            _mm256_storeu_ps(&MX1[0*cols + b], R0);
            _mm256_storeu_ps(&MX1[1*cols + b], R1);
            _mm256_storeu_ps(&MX1[2*cols + b], R2);
            _mm256_storeu_ps(&MX1[3*cols + b], R3);
        }
        for(; b<block ; b++){
            MX1[0*cols + b] += MX2[0*cols + b];
            MX1[1*cols + b] += MX2[1*cols + b];
            MX1[2*cols + b] += MX2[2*cols + b];
            MX1[3*cols + b] += MX2[3*cols + b];
        }
    }

    __attribute__((target("avx2,fma")))
    void broadcast_ps_4x8_div(float* __restrict MX1, const float* __restrict MX2, int block, int cols){


        int b = 0;
        for(; b<=block-8; b+=8){
            //MX1
            __m256 MX1V0 = _mm256_loadu_ps(&MX1[0*cols + b]);
            __m256 MX1V1 = _mm256_loadu_ps(&MX1[1*cols + b]);
            __m256 MX1V2 = _mm256_loadu_ps(&MX1[2*cols + b]);
            __m256 MX1V3 = _mm256_loadu_ps(&MX1[3*cols + b]);

            //MX2
            __m256 MX2V0 = _mm256_loadu_ps(&MX2[0*cols + b]);
            __m256 MX2V1 = _mm256_loadu_ps(&MX2[1*cols + b]);
            __m256 MX2V2 = _mm256_loadu_ps(&MX2[2*cols + b]);
            __m256 MX2V3 = _mm256_loadu_ps(&MX2[3*cols + b]);

            __m256 R0 = _mm256_div_ps(MX1V0, MX2V0);
            __m256 R1 = _mm256_div_ps(MX1V1, MX2V1);
            __m256 R2 = _mm256_div_ps(MX1V2, MX2V2);
            __m256 R3 = _mm256_div_ps(MX1V3, MX2V3);

            _mm256_storeu_ps(&MX1[0*cols + b], R0);
            _mm256_storeu_ps(&MX1[1*cols + b], R1);
            _mm256_storeu_ps(&MX1[2*cols + b], R2);
            _mm256_storeu_ps(&MX1[3*cols + b], R3);
        }
        for(; b<block ; b++){
            MX1[0*cols + b] /= MX2[0*cols + b];
            MX1[1*cols + b] /= MX2[1*cols + b];
            MX1[2*cols + b] /= MX2[2*cols + b];
            MX1[3*cols + b] /= MX2[3*cols + b];
        }
    }

    __attribute__((target("avx2,fma")))
    void broadcast_ps_4x8_sub(float* __restrict MX1, const float* __restrict MX2, int block, int cols){

        int b = 0;
        for(; b<=block-8; b+=8){
            //MX1
            __m256 MX1V0 = _mm256_loadu_ps(&MX1[0*cols + b]);
            __m256 MX1V1 = _mm256_loadu_ps(&MX1[1*cols + b]);
            __m256 MX1V2 = _mm256_loadu_ps(&MX1[2*cols + b]);
            __m256 MX1V3 = _mm256_loadu_ps(&MX1[3*cols + b]);

            //MX2
            __m256 MX2V0 = _mm256_loadu_ps(&MX2[0*cols + b]);
            __m256 MX2V1 = _mm256_loadu_ps(&MX2[1*cols + b]);
            __m256 MX2V2 = _mm256_loadu_ps(&MX2[2*cols + b]);
            __m256 MX2V3 = _mm256_loadu_ps(&MX2[3*cols + b]);

            __m256 R0 = _mm256_sub_ps(MX1V0, MX2V0);
            __m256 R1 = _mm256_sub_ps(MX1V1, MX2V1);
            __m256 R2 = _mm256_sub_ps(MX1V2, MX2V2);
            __m256 R3 = _mm256_sub_ps(MX1V3, MX2V3);

            _mm256_storeu_ps(&MX1[0*cols + b], R0);
            _mm256_storeu_ps(&MX1[1*cols + b], R1);
            _mm256_storeu_ps(&MX1[2*cols + b], R2);
            _mm256_storeu_ps(&MX1[3*cols + b], R3);
        }
        //remaining 1-7
        for(; b<block ; b++){
            MX1[0*cols + b] -= MX2[0*cols + b];
            MX1[1*cols + b] -= MX2[1*cols + b];
            MX1[2*cols + b] -= MX2[2*cols + b];
            MX1[3*cols + b] -= MX2[3*cols + b];
        }
    }

    __attribute__((target("avx2")))
    void broadcast_ps_8x8_equal(float* __restrict MX1, const float* __restrict MX2, int block, int cols){

        int b = 0;
        for(; b<=block-8; b+=8){
            //MX2
            __m256 MX2V0 = _mm256_loadu_ps(&MX2[0*cols + b]);
            __m256 MX2V1 = _mm256_loadu_ps(&MX2[1*cols + b]);
            __m256 MX2V2 = _mm256_loadu_ps(&MX2[2*cols + b]);
            __m256 MX2V3 = _mm256_loadu_ps(&MX2[3*cols + b]);
            __m256 MX2V4 = _mm256_loadu_ps(&MX2[4*cols + b]);
            __m256 MX2V5 = _mm256_loadu_ps(&MX2[5*cols + b]);
            __m256 MX2V6 = _mm256_loadu_ps(&MX2[6*cols + b]);
            __m256 MX2V7 = _mm256_loadu_ps(&MX2[7*cols + b]);

            _mm256_storeu_ps(&MX1[0*cols + b], MX2V0);
            _mm256_storeu_ps(&MX1[1*cols + b], MX2V1);
            _mm256_storeu_ps(&MX1[2*cols + b], MX2V2);
            _mm256_storeu_ps(&MX1[3*cols + b], MX2V3);
            _mm256_storeu_ps(&MX1[4*cols + b], MX2V4);
            _mm256_storeu_ps(&MX1[5*cols + b], MX2V5);
            _mm256_storeu_ps(&MX1[6*cols + b], MX2V6);
            _mm256_storeu_ps(&MX1[7*cols + b], MX2V7);
        }
        //remaining 1-7
        for(; b<block ; b++){
            MX1[0*cols + b] = MX2[0*cols + b];
            MX1[1*cols + b] = MX2[1*cols + b];
            MX1[2*cols + b] = MX2[2*cols + b];
            MX1[3*cols + b] = MX2[3*cols + b];
            MX1[4*cols + b] = MX2[4*cols + b];
            MX1[5*cols + b] = MX2[5*cols + b];
            MX1[6*cols + b] = MX2[6*cols + b];
            MX1[7*cols + b] = MX2[7*cols + b];
        }
    }

}//micros namespace


