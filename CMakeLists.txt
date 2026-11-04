cmake_minimum_required (VERSION 3.10 )

project(matrix_lib)

set(OSBitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OSBitness 64)
endif()

add_compile_options(-O3) #change to o3 when optimizing for speed

add_executable(${PROJECT_NAME}
    main.cpp
    )
target_include_directories(matrix_lib PRIVATE
    interface_based/matrix_math
    interface_based/statistics

    src/matrix_core
    src/core_helper_f
    src/subtypes
    src
    )
