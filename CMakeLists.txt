cmake_minimum_required(VERSION 3.16)
project(TTC LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

include(FetchContent)

FetchContent_Declare(
    eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG        5.0.0
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(eigen)

FetchContent_Declare(
    gemmi
    GIT_REPOSITORY https://github.com/project-gemmi/gemmi.git
    GIT_TAG        v0.6.7
)
FetchContent_MakeAvailable(gemmi)

set(SOURCES
    src/TensorToCIF.cpp
    src/LinearAlgebraUtils.cpp
)

set(HEADERS
    include/TensorToCIF.h
    include/LinearAlgebraUtils.h
)

add_executable(TTC ${SOURCES} ${HEADERS})

target_compile_features(TTC PRIVATE cxx_std_17)

target_include_directories(TTC PRIVATE src)

target_link_libraries(TTC PRIVATE Qt::Widgets Eigen3::Eigen gemmi::gemmi_cpp)
