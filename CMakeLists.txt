cmake_minimum_required(VERSION 3.10)
project(Gomoku VERSION 0.1.0)

# C++標準の設定
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# インクルードディレクトリの設定
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# GomokuLibのライブラリ作成
add_library(GomokuLib STATIC)

# GomokuLibのソースファイルを取得
include(src/GomokuLib/CMakeLists.txt)

# GomokuCLIの実行ファイル作成
add_executable(GomokuCLI
    src/GomokuCLI/main.cpp
    src/GomokuCLI/GomokuCLI.cpp
)

# GomokuCLIがGomokuLibに依存
target_link_libraries(GomokuCLI GomokuLib)

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

# GomokuGUIの有効化
option(BUILD_GUI "Build GomokuGUI (Qt required)" ON)
if(BUILD_GUI)
    add_subdirectory(src/GomokuGUI)
endif()

# プロジェクト情報の出力
message(STATUS "GomokuLib version: ${PROJECT_VERSION}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
