# Googleテストの検索
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# テスト実行ファイルのソース
set(TEST_SOURCES
    BoardTest.cpp
    GameTest.cpp
    main_test.cpp
)

# テスト実行ファイルの作成
add_executable(gomoku_test ${TEST_SOURCES})

# ライブラリとGoogleテストをリンク
target_link_libraries(gomoku_test 
    GomokuLib
    ${GTEST_LIBRARIES}
    pthread
)

# テストの自動検出と実行の設定
gtest_discover_tests(gomoku_test)
