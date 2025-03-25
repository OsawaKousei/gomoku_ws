#include <gtest/gtest.h>
#include "GomokuLib/Game.h"
#include <fstream>
#include <cstdio> // for remove()

using namespace GomokuLib;

// Gameクラスのテスト
class GameTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // 標準サイズのゲームを作成
        game = new Game(15);

        // テスト用の小さいサイズのゲームも作成
        smallGame = new Game(5);
    }

    void TearDown() override
    {
        delete game;
        delete smallGame;
    }

    Game *game;
    Game *smallGame;
};

// 初期化のテスト
TEST_F(GameTest, Initialization)
{
    // 初期プレイヤーは黒
    EXPECT_EQ(game->getCurrentPlayer(), Stone::BLACK);

    // 初期盤面は空
    const Board &board = game->getBoard();
    for (int i = 0; i < board.getSize(); ++i)
    {
        for (int j = 0; j < board.getSize(); ++j)
        {
            EXPECT_EQ(board.getStone(i, j), Stone::EMPTY);
        }
    }

    // 初期状態は棋譜が空
    EXPECT_TRUE(game->getMoves().empty());

    // 初期状態はゲーム終了していない
    EXPECT_FALSE(game->isGameOver());

    // 初期状態は勝者がいない
    EXPECT_EQ(game->getWinner(), Stone::EMPTY);
}

// 手番のテスト
TEST_F(GameTest, PlayTurn)
{
    // 黒が打つ
    EXPECT_EQ(game->playTurn(7, 7), MoveResult::SUCCESS);
    EXPECT_EQ(game->getBoard().getStone(7, 7), Stone::BLACK);

    // 手番が白に変わる
    EXPECT_EQ(game->getCurrentPlayer(), Stone::WHITE);

    // 白が打つ
    EXPECT_EQ(game->playTurn(7, 8), MoveResult::SUCCESS);
    EXPECT_EQ(game->getBoard().getStone(7, 8), Stone::WHITE);

    // 手番が黒に戻る
    EXPECT_EQ(game->getCurrentPlayer(), Stone::BLACK);

    // 同じ場所には打てない
    EXPECT_EQ(game->playTurn(7, 7), MoveResult::INVALID_MOVE);

    // 盤面外には打てない
    EXPECT_EQ(game->playTurn(15, 15), MoveResult::INVALID_MOVE);
}

// 棋譜のテスト
TEST_F(GameTest, MovesRecord)
{
    // 複数手打つ
    game->playTurn(7, 7);
    game->playTurn(7, 8);
    game->playTurn(8, 7);

    // 棋譜の確認
    std::vector<std::pair<int, int>> moves = game->getMoves();
    EXPECT_EQ(moves.size(), 3);
    EXPECT_EQ(moves[0], std::make_pair(7, 7));
    EXPECT_EQ(moves[1], std::make_pair(7, 8));
    EXPECT_EQ(moves[2], std::make_pair(8, 7));
}

// 手を戻すテスト
TEST_F(GameTest, UndoMove)
{
    // 複数手打つ
    game->playTurn(7, 7);
    game->playTurn(7, 8);
    EXPECT_EQ(game->getCurrentPlayer(), Stone::BLACK);

    // 1手戻す
    EXPECT_TRUE(game->undoMove());

    // 手番と盤面を確認
    EXPECT_EQ(game->getCurrentPlayer(), Stone::WHITE);
    EXPECT_EQ(game->getBoard().getStone(7, 8), Stone::EMPTY);
    EXPECT_EQ(game->getBoard().getStone(7, 7), Stone::BLACK);
    EXPECT_EQ(game->getMoves().size(), 1);

    // もう1手戻す
    EXPECT_TRUE(game->undoMove());

    // 盤面が空になっていることを確認
    EXPECT_EQ(game->getCurrentPlayer(), Stone::BLACK);
    EXPECT_EQ(game->getBoard().getStone(7, 7), Stone::EMPTY);
    EXPECT_EQ(game->getMoves().size(), 0);

    // これ以上戻せないことを確認
    EXPECT_FALSE(game->undoMove());
}

// 勝利判定のテスト
TEST_F(GameTest, GameWinning)
{
    // 黒が水平に5つ並べる
    smallGame->playTurn(0, 0); // 黒
    smallGame->playTurn(1, 0); // 白
    smallGame->playTurn(0, 1); // 黒
    smallGame->playTurn(1, 1); // 白
    smallGame->playTurn(0, 2); // 黒
    smallGame->playTurn(1, 2); // 白
    smallGame->playTurn(0, 3); // 黒

    // ゲーム終了前
    EXPECT_FALSE(smallGame->isGameOver());

    // 勝利の手
    EXPECT_EQ(smallGame->playTurn(1, 3), MoveResult::SUCCESS); // 白
    smallGame->playTurn(0, 4);                                 // 黒 - これで黒の勝ち

    // ゲーム終了後
    EXPECT_TRUE(smallGame->isGameOver());
    EXPECT_EQ(smallGame->getWinner(), Stone::BLACK);

    // ゲーム終了後に打とうとしてもエラー
    EXPECT_EQ(smallGame->playTurn(2, 2), MoveResult::GAME_OVER);
}

// 棋譜の保存と読み込みテスト
TEST_F(GameTest, SaveAndLoadGame)
{
    // テスト用ファイルパス
    const std::string testFilePath = "test_game.gomoku";

    // ゲームを進める
    smallGame->playTurn(0, 0); // 黒
    smallGame->playTurn(1, 0); // 白
    smallGame->playTurn(0, 1); // 黒

    // 棋譜を保存
    smallGame->saveGame(testFilePath);

    // ファイルが作成されたか確認
    std::ifstream testFile(testFilePath);
    EXPECT_TRUE(testFile.good());
    testFile.close();

    // 棋譜を読み込む
    Game loadedGame = Game::loadGame(testFilePath);

    // 読み込まれたゲームの状態を確認
    EXPECT_EQ(loadedGame.getBoard().getSize(), 5);
    EXPECT_EQ(loadedGame.getBoard().getStone(0, 0), Stone::BLACK);
    EXPECT_EQ(loadedGame.getBoard().getStone(1, 0), Stone::WHITE);
    EXPECT_EQ(loadedGame.getBoard().getStone(0, 1), Stone::BLACK);
    EXPECT_EQ(loadedGame.getCurrentPlayer(), Stone::WHITE);
    EXPECT_EQ(loadedGame.getMoves().size(), 3);

    // テスト後にファイルを削除
    remove(testFilePath.c_str());
}

// エラーケースのテスト
TEST_F(GameTest, ErrorCases)
{
    // 不正なファイルパスからの読み込み
    EXPECT_THROW(Game::loadGame("non_existent_file.gomoku"), std::runtime_error);
}
