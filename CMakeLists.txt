cmake_minimum_required (VERSION 3.10 )

project(matrix_lib)

set(OSBitness 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OSBitness 64)
endif()

add_executable(${PROJECT_NAME}
    main.cpp
    src/matrix_container.cpp
    src/matrix.cpp
    src/matrix_view.cpp
    src/matrix_method.cpp
    src/matrix_statistic.cpp
    src/matrix_learn.cpp
    src/matrix_mask.cpp
    )
target_include_directories(matrix_lib PRIVATE
    src
    )
