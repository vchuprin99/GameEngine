cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME GameEngine)

add_subdirectory(GESDK)
add_subdirectory(GE)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GameEngineSDK)