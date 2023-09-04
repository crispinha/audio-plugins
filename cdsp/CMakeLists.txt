cmake_minimum_required (VERSION 3.20)
project(cdsp CXX)

add_library(cdsp)

set_target_properties(cdsp PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(cdsp PRIVATE cxx_std_20)
target_sources(cdsp PUBLIC
        cdsp.h
        cdsp/filters.cpp cdsp/filters.hpp
        )
target_include_directories(cdsp PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${CMAKE_CURRENT_LIST_DIR}/common")


