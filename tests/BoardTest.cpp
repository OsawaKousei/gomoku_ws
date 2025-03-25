#include <gtest/gtest.h>
#include "GomokuLib/Board.h"

using namespace GomokuLib;

// Boardクラスのテスト
class BoardTest : public ::testing::Test
{
protected:
    // 各テストケース前に実行
    void SetUp() override
    {
        // 標準的な15x15の盤面を作成
        board15 = new Board(15);

        // 小さい5x5の盤面も作成（境界条件テスト用）
        board5 = new Board(5);
    }

    // 各テストケース後に実行
    void TearDown() override
    {
        delete board15;
        delete board5;
    }

    Board *board15;
    Board *board5;
};

// 初期化のテスト
TEST_F(BoardTest, Initialization)
{
    EXPECT_EQ(board15->getSize(), 15);
    EXPECT_EQ(board5->getSize(), 5);

    // 初期状態はすべて空
    for (int i = 0; i < 15; ++i)
    {
        for (int j = 0; j < 15; ++j)
        {
            EXPECT_EQ(board15->getStone(i, j), Stone::EMPTY);
        }
    }
}

// 石の配置と取得のテスト
TEST_F(BoardTest, PlaceAndGetStone)
{
    // 適切な位置に石を置く
    EXPECT_TRUE(board15->placeStone(7, 7, Stone::BLACK));
    EXPECT_EQ(board15->getStone(7, 7), Stone::BLACK);

    // 同じ位置に再度石を置けないことを確認
    EXPECT_FALSE(board15->placeStone(7, 7, Stone::WHITE));
    EXPECT_EQ(board15->getStone(7, 7), Stone::BLACK);

    // 盤面外に石を置けないことを確認
    EXPECT_FALSE(board15->placeStone(15, 7, Stone::BLACK));
    EXPECT_FALSE(board15->placeStone(7, 15, Stone::BLACK));
    EXPECT_FALSE(board15->placeStone(-1, 7, Stone::BLACK));
    EXPECT_FALSE(board15->placeStone(7, -1, Stone::BLACK));

    // 盤面外の取得は空を返すことを確認
    EXPECT_EQ(board15->getStone(15, 7), Stone::EMPTY);
}

// 勝利判定のテスト - 水平方向
TEST_F(BoardTest, CheckWinnerHorizontal)
{
    // 水平方向に5つ並べる
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_TRUE(board15->placeStone(7, i, Stone::BLACK));
    }

    // 黒の勝利を確認
    EXPECT_EQ(board15->checkWinner(), Stone::BLACK);

    // 別の盤面で白の勝利確認
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_TRUE(board5->placeStone(2, i, Stone::WHITE));
    }

    EXPECT_EQ(board5->checkWinner(), Stone::WHITE);
}

// 勝利判定のテスト - 垂直方向
TEST_F(BoardTest, CheckWinnerVertical)
{
    // 垂直方向に5つ並べる
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_TRUE(board15->placeStone(i, 7, Stone::BLACK));
    }

    // 黒の勝利を確認
    EXPECT_EQ(board15->checkWinner(), Stone::BLACK);
}

// 勝利判定のテスト - 右下がり対角線
TEST_F(BoardTest, CheckWinnerDiagonalDown)
{
    // 右下がり対角線に5つ並べる
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_TRUE(board15->placeStone(i, i, Stone::WHITE));
    }

    // 白の勝利を確認
    EXPECT_EQ(board15->checkWinner(), Stone::WHITE);
}

// 勝利判定のテスト - 左下がり対角線
TEST_F(BoardTest, CheckWinnerDiagonalUp)
{
    // 左下がり対角線に5つ並べる
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_TRUE(board15->placeStone(i, 4 - i, Stone::WHITE));
    }

    // 白の勝利を確認
    EXPECT_EQ(board15->checkWinner(), Stone::WHITE);
}

// 盤面が埋まっているかのテスト
TEST_F(BoardTest, IsFull)
{
    // 初期状態は埋まっていない
    EXPECT_FALSE(board5->isFull());

    // 盤面を埋める
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            EXPECT_TRUE(board5->placeStone(i, j, Stone::BLACK));
        }
    }

    // すべて埋まっていることを確認
    EXPECT_TRUE(board5->isFull());
}

// 引き分け判定のテスト
TEST_F(BoardTest, DrawGame)
{
    // 5x5のパターンを配置
    Stone pattern[5][5] = {
        {Stone::BLACK, Stone::WHITE, Stone::BLACK, Stone::WHITE, Stone::BLACK},
        {Stone::WHITE, Stone::BLACK, Stone::BLACK, Stone::WHITE, Stone::BLACK},
        {Stone::BLACK, Stone::BLACK, Stone::WHITE, Stone::BLACK, Stone::WHITE},
        {Stone::WHITE, Stone::BLACK, Stone::WHITE, Stone::BLACK, Stone::WHITE},
        {Stone::BLACK, Stone::WHITE, Stone::BLACK, Stone::WHITE, Stone::WHITE}};

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            EXPECT_TRUE(board5->placeStone(i, j, pattern[i][j]));
        }
    }

    // 盤面が埋まっていることを確認
    EXPECT_TRUE(board5->isFull());

    // 引き分けを確認
    EXPECT_EQ(board5->checkWinner(), Stone::DRAW);
}