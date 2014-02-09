cmake_minimum_required (VERSION 2.6)

project (tp2)
 
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -fopenmp -O3")

add_executable(tp2 premiers.cpp)
