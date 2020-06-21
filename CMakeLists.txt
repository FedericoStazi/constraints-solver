cmake_minimum_required(VERSION 3.16)
project(constraints_solver)

set(CMAKE_CXX_STANDARD 17)

add_executable(constraints_solver main.cpp ConstraintsSolver.h Variable.h)