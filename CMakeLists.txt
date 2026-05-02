cmake_minimum_required(VERSION 3.16)
project(QtTest LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(QtTest main.cpp)
target_link_libraries(QtTest PRIVATE Qt::Widgets)