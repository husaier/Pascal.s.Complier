cmake_minimum_required(VERSION 3.16)
project(LR_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(LR_1 main.cpp FirstSetCalculator.cpp FirstSetCalculator.h LR1TableMaker.cpp LR1TableMaker.h LR1Runner.cpp LR1Runner.h
        lexical_analyzer.cpp lexical_analyzer.h)