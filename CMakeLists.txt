cmake_minimum_required(VERSION 3.16)
project(TTC LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Eigen3 5.0 REQUIRED NO_MODULE)

set(SOURCES
    TensorToCIF.cpp
    LinearAlgebraUtils.cpp
)

set(HEADERS
    TensorToCIF.h
    LinearAlgebraUtils.h
)

add_executable(TTC ${SOURCES} ${HEADERS})

target_include_directories(TTC PRIVATE include)

target_link_libraries(TTC PRIVATE Qt::Widgets Eigen3::Eigen)