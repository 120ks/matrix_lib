cmake_minimum_required (VERSION 3.10 )

project(matrix_lib)

set(OSBitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OSBitness 64)
endif()

add_executable(matrix_lib main.cpp)

find_package(OpenMP REQUIRED)
target_link_libraries(matrix_lib PRIVATE OpenMP::OpenMP_CXX)
add_compile_options(-O3 -march=native -ffast-math -mavx -mfma -fopenmp)



target_include_directories(matrix_lib PRIVATE
    interface_based/matrix_math
    interface_based/statistics
    
    src/optimized_math
    src/base_member_f
    src/matrix_core
    src/core_helper_f
    src/subtypes
    src
    )
