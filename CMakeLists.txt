cmake_minimum_required(VERSION 3.16)
project(TensorToCIF LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

include(FetchContent)

FetchContent_Declare(
    eigen
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 5.0.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(eigen)

FetchContent_Declare(
    gemmi
    GIT_REPOSITORY https://github.com/project-gemmi/gemmi.git
    GIT_TAG v0.6.7
)
FetchContent_MakeAvailable(gemmi)

include(CTest)

add_library(ttc_cif STATIC src/cif/CifParser.cpp)
target_include_directories(ttc_cif PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(ttc_cif PUBLIC Eigen3::Eigen gemmi::gemmi_cpp)
target_compile_features(ttc_cif PUBLIC cxx_std_17)

set(HEADERS
    src/core/CrystalData.h
    src/cif/CifParser.h
    src/math/LinearAlgebraUtils.h
)

add_executable(TensorToCIF src/main.cpp ${HEADERS})
target_compile_features(TensorToCIF PRIVATE cxx_std_17)
target_link_libraries(TensorToCIF PRIVATE ttc_cif Qt::Widgets)

if(BUILD_TESTING)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.4
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(ttc_tests tests/test_cif_parser.cpp ${HEADERS})
    target_compile_features(ttc_tests PRIVATE cxx_std_17)
    target_link_libraries(ttc_tests PRIVATE ttc_cif Catch2::Catch2WithMain)
    target_compile_definitions(ttc_tests PRIVATE
        TTC_SAMPLES_DIR="${CMAKE_SOURCE_DIR}/data/samples")

    add_test(NAME cif_parser COMMAND ttc_tests)
endif()
