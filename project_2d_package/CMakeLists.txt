cmake_minimum_required(VERSION 3.21)
project(project_2d_package)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(project_2d_package
        circlealgorithm.cpp
        circlealgorithm.h
        clippingalgorithm.cpp
        clippingalgorithm.h
        ellipsealgorithm.cpp
        ellipsealgorithm.h
        ids.h
        linealgorithm.cpp
        linealgorithm.h
        main.cpp)
