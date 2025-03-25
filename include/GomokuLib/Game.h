#pragma once

#include "Board.h"
#include <string>
#include <vector>
#include <utility>

namespace GomokuLib
{

    // ゲーム全体の進行を管理するクラス
    class Game
    {
    private:
        Board board;                            // 盤面
        Stone currentPlayer;                    // 現在のプレイヤー
        std::vector<std::pair<int, int>> moves; // 棋譜 (行, 列)

    public:
        // コンストラクタ
        Game(int boardSize);

        // 現在のプレイヤーが指定位置に石を置く
        MoveResult playTurn(int row, int col);

        // 現在のプレイヤーを取得
        Stone getCurrentPlayer() const;

        // 盤面を取得
        const Board &getBoard() const;

        // ゲームが終了しているか
        bool isGameOver() const;

        // 勝者を取得
        Stone getWinner() const;

        // 棋譜を取得
        std::vector<std::pair<int, int>> getMoves() const;

        // 一手戻す
        bool undoMove();

        // 棋譜からゲームを復元
        static Game loadGame(const std::string &filepath);

        // 現在の棋譜を保存
        void saveGame(const std::string &filepath) const;
    };

} // namespace GomokuLib
