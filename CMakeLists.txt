cmake_minimum_required(VERSION 3.10)
project(GomokuLib VERSION 1.0.0 LANGUAGES CXX)

# C++11以上を要求
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# インクルードディレクトリを設定
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# ソースファイルを収集
file(GLOB_RECURSE SOURCES "src/GomokuLib/*.cpp")

# ライブラリをビルド
add_library(GomokuLib STATIC ${SOURCES})

# インストール設定
install(TARGETS GomokuLib
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(DIRECTORY include/GomokuLib
        DESTINATION include)

# テストの有効化
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
    add_subdirectory(tests)
endif()

# サンプルアプリケーションの有効化
option(BUILD_SAMPLES "Build sample applications" OFF)
if(BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

# プロジェクト情報の出力
message(STATUS "GomokuLib version: ${PROJECT_VERSION}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
