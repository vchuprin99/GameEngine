cmake_minimum_required(VERSION 3.25)

set(SDK_PROJECT_NAME GameEngineSDK)

add_executable(${SDK_PROJECT_NAME} src/main.cpp)

target_link_libraries(${SDK_PROJECT_NAME} GameEngineCore)
target_compile_features(${SDK_PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${CORE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
