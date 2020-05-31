cmake_minimum_required(VERSION 3.16)
project(LR_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(LR_1 main.cpp LR1Runner.cpp LR1Runner.h
        lexical_analyzer.cpp lexical_analyzer.h LR1Table.h LR1Table.cpp Production.h Production.cpp cJSON.h cJSON.c
        CJsonObject.hpp CJsonObject.cpp SymbolBlock.h SymbolBlock.cpp TranslateScheme.cpp TranslateScheme.h)