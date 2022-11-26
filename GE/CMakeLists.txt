cmake_minimum_required(VERSION 3.25)

set(CORE_PROJECT_NAME GameEngineCore)

add_library(${CORE_PROJECT_NAME} STATIC 
	src/core/utils/main.cpp
	includes/core/utils/main.h
)
target_include_directories(${CORE_PROJECT_NAME} PUBLIC includes)
target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_17)