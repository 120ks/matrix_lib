cmake_minimum_required (VERSION 3.10 )

project(matrix)

set(OSBitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OSBitness 64)
endif()

add_executable(${PROJECT_NAME}
    main.cpp
    )
target_include_directories(matrix PRIVATE
    src
    src/matrix_math
    )


#cmake -S . -B build

#cmake --build build

#./build/matrix