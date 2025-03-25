#pragma once

#include "Common.h"
#include <vector>

namespace GomokuLib
{

    // 盤面を表現するクラス
    class Board
    {
    private:
        int size;                             // 盤面のサイズ（一辺のマス数）
        std::vector<std::vector<Stone>> grid; // 盤面の状態

        // 指定された位置が盤面内かチェック
        bool isValidPosition(int row, int col) const;

        // 勝利判定のためのヘルパーメソッド
        bool checkLine(int row, int col, int dRow, int dCol, Stone stone) const;

    public:
        // コンストラクタ
        Board(int size);

        // 石を配置
        bool placeStone(int row, int col, Stone stone);

        // 指定位置の石を取得
        Stone getStone(int row, int col) const;

        // 勝者の判定
        Stone checkWinner() const;

        // 盤面が全て埋まっているか
        bool isFull() const;

        // 盤面サイズの取得
        int getSize() const;
    };

} // namespace GomokuLib
