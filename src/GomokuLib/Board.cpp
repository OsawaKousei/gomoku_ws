#include "GomokuLib/Board.h"

namespace GomokuLib
{

    Board::Board(int size) : size(size)
    {
        // 盤面の初期化
        grid.resize(size, std::vector<Stone>(size, Stone::EMPTY));
    }

    bool Board::isValidPosition(int row, int col) const
    {
        return row >= 0 && row < size && col >= 0 && col < size;
    }

    bool Board::placeStone(int row, int col, Stone stone)
    {
        // 位置の妥当性チェック
        if (!isValidPosition(row, col))
        {
            return false;
        }

        // 石を取り除く場合
        if (stone == Stone::EMPTY)
        {
            grid[row][col] = Stone::EMPTY;
            return true;
        }

        // すでに石がある場合は配置できない
        if (grid[row][col] != Stone::EMPTY)
        {
            return false;
        }

        // 石を配置
        grid[row][col] = stone;
        return true;
    }

    Stone Board::getStone(int row, int col) const
    {
        if (!isValidPosition(row, col))
        {
            return Stone::EMPTY;
        }
        return grid[row][col];
    }

    bool Board::checkLine(int row, int col, int dRow, int dCol, Stone stone) const
    {
        int count = 0;

        // 正方向にカウント
        for (int i = 0; i < 5; i++)
        {
            int newRow = row + i * dRow;
            int newCol = col + i * dCol;

            if (!isValidPosition(newRow, newCol) || grid[newRow][newCol] != stone)
            {
                break;
            }
            count++;
        }

        return count >= 5;
    }

    Stone Board::checkWinner() const
    {
        // 勝者判定の方向: 水平、垂直、右下がり対角線、左下がり対角線
        const int directions[4][2] = {
            {0, 1}, // 水平
            {1, 0}, // 垂直
            {1, 1}, // 右下がり対角線
            {1, -1} // 左下がり対角線
        };

        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {
                Stone stone = grid[row][col];

                // 空マスはスキップ
                if (stone == Stone::EMPTY)
                {
                    continue;
                }

                // 4方向チェック
                for (int d = 0; d < 4; d++)
                {
                    if (checkLine(row, col, directions[d][0], directions[d][1], stone))
                    {
                        return stone;
                    }
                }
            }
        }

        // 勝者がいない場合
        if (isFull())
        {
            return Stone::DRAW;
        }

        return Stone::EMPTY;
    }

    bool Board::isFull() const
    {
        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {
                if (grid[row][col] == Stone::EMPTY)
                {
                    return false;
                }
            }
        }
        return true;
    }

    int Board::getSize() const
    {
        return size;
    }

} // namespace GomokuLib
