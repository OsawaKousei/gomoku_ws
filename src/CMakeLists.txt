# GomokuLibのソースファイル
set(GOMOKU_LIB_SOURCES
    Board.cpp
    Game.cpp
)

# ソースファイルをライブラリに追加
target_sources(GomokuLib PRIVATE ${GOMOKU_LIB_SOURCES})
