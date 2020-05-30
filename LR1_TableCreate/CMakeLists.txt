cmake_minimum_required(VERSION 3.16)
project(LR1_TableCreate)

set(CMAKE_CXX_STANDARD 14)

add_executable(LR1_TableCreate main.cpp FirstSetCalculator.h FirstSetCalculator.cpp LR1TableMaker.h LR1TableMaker.cpp
        CJsonObject.hpp CJsonObject.cpp cJSON.h cJSON.c LR1Table.cpp LR1Table.h Production.cpp Production.h)