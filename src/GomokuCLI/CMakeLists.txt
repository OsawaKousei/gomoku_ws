# GomokuCLIのソースファイル
set(GOMOKU_CLI_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GomokuCLI.cpp
)

# ソースファイルをターゲットに追加
target_sources(GomokuCLI PRIVATE ${GOMOKU_CLI_SOURCES})
