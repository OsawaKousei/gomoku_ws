#include "GomokuLib/Game.h"
#include <fstream>
#include <stdexcept>

namespace GomokuLib
{

    Game::Game(int boardSize) : board(boardSize), currentPlayer(Stone::BLACK)
    {
        // ゲームの初期化
    }

    MoveResult Game::playTurn(int row, int col)
    {
        // ゲーム終了チェック
        if (isGameOver())
        {
            return MoveResult::GAME_OVER;
        }

        // 石を置く
        if (!board.placeStone(row, col, currentPlayer))
        {
            return MoveResult::INVALID_MOVE;
        }

        // 着手を記録
        moves.push_back(std::make_pair(row, col));

        // プレイヤー交代
        currentPlayer = (currentPlayer == Stone::BLACK) ? Stone::WHITE : Stone::BLACK;

        return MoveResult::SUCCESS;
    }

    Stone Game::getCurrentPlayer() const
    {
        return currentPlayer;
    }

    const Board &Game::getBoard() const
    {
        return board;
    }

    bool Game::isGameOver() const
    {
        return board.checkWinner() != Stone::EMPTY || board.isFull();
    }

    Stone Game::getWinner() const
    {
        return board.checkWinner();
    }

    std::vector<std::pair<int, int>> Game::getMoves() const
    {
        return moves;
    }

    bool Game::undoMove()
    {
        // 着手がない場合
        if (moves.empty())
        {
            return false;
        }

        // 最後の着手を取得して削除
        auto lastMove = moves.back();
        moves.pop_back();

        // 盤面から石を取り除く
        int row = lastMove.first;
        int col = lastMove.second;
        board.placeStone(row, col, Stone::EMPTY);

        // プレイヤーを前の手番に戻す
        currentPlayer = (currentPlayer == Stone::BLACK) ? Stone::WHITE : Stone::BLACK;

        return true;
    }

    Game Game::loadGame(const std::string &filepath)
    {
        std::ifstream file(filepath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + filepath);
        }

        std::string line;
        int boardSize = 0;
        Game game(15); // デフォルトサイズで初期化

        while (std::getline(file, line))
        {
            // コメント行をスキップ
            if (line.empty() || line[0] == '#')
            {
                continue;
            }

            // 盤面サイズの行
            if (line.find("SIZE:") == 0)
            {
                boardSize = std::stoi(line.substr(5));
                game = Game(boardSize);
            }
            // 着手の行
            else if (line.find("MOVES:") != std::string::npos)
            {
                continue; // MOVES: 行はスキップ
            }
            // 着手データ
            else
            {
                size_t pos1 = line.find(',');
                size_t pos2 = line.find(',', pos1 + 1);

                if (pos1 != std::string::npos && pos2 != std::string::npos)
                {
                    int row = std::stoi(line.substr(0, pos1));
                    int col = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
                    char stoneChar = line[pos2 + 1];

                    Stone stone = (stoneChar == 'B') ? Stone::BLACK : Stone::WHITE;

                    // ゲーム状態を進めるため、currentPlayerを正しい色に設定
                    game.currentPlayer = stone;
                    game.playTurn(row, col);
                }
            }
        }

        return game;
    }

    void Game::saveGame(const std::string &filepath) const
    {
        std::ofstream file(filepath);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file for writing: " + filepath);
        }

        // 盤面サイズを書き出す
        file << "SIZE: " << board.getSize() << std::endl;
        file << "MOVES:" << std::endl;

        Stone currentStone = Stone::BLACK; // 最初は黒から
        for (const auto &move : moves)
        {
            char stoneChar = (currentStone == Stone::BLACK) ? 'B' : 'W';
            file << move.first << "," << move.second << "," << stoneChar << std::endl;

            // 次の石の色を切り替え
            currentStone = (currentStone == Stone::BLACK) ? Stone::WHITE : Stone::BLACK;
        }

        file.close();
    }

} // namespace GomokuLib
