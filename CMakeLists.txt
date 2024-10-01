cmake_minimum_required(VERSION 3.5.0)
project(labwork1 VERSION 0.1.0 LANGUAGES C CXX)

SET(CMAKE_CXX_STANDARD 23)

add_executable(AnalyzeLog main.cpp merge_sort.cpp merge_sort.h parsing_logs_functions.cpp parsing_logs_functions.h string_functions.cpp string_functions.h structures.h timestamp_functions.cpp timestamp_functions.h vector_functions.cpp vector_functions.h)