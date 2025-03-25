# GomokuLibのソースファイル
set(GOMOKU_LIB_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/Board.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Game.cpp
)

# ソースファイルをライブラリに追加
target_sources(GomokuLib PRIVATE ${GOMOKU_LIB_SOURCES})