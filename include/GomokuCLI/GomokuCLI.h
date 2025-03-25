#pragma once

#include "GomokuLib/Game.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>

class GomokuCLI
{
private:
    std::unique_ptr<GomokuLib::Game> game; // ゲームインスタンス
    bool isRunning;                        // アプリケーションの実行状態
    bool gameLoaded;                       // ゲームがロードされているか

    // コマンドハンドラーの型定義
    using CommandHandler = std::function<void(const std::vector<std::string> &)>;

    // コマンドとハンドラーのマッピング
    std::map<std::string, CommandHandler> commandHandlers;

    // コマンド実行のヘルパーメソッド
    void registerCommands();
    std::vector<std::string> parseCommand(const std::string &input);
    bool executeCommand(const std::vector<std::string> &tokens);

    // 各コマンドの実装
    void handleStart(const std::vector<std::string> &args);
    void handlePlace(const std::vector<std::string> &args);
    void handleShow(const std::vector<std::string> &args);
    void handleMoves(const std::vector<std::string> &args);
    void handleSave(const std::vector<std::string> &args);
    void handleLoad(const std::vector<std::string> &args);
    void handleUndo(const std::vector<std::string> &args);
    void handleExit(const std::vector<std::string> &args);
    void handleHelp(const std::vector<std::string> &args);

    // ユーティリティメソッド
    void displayBoard() const;
    void displayGameStatus() const;
    void displayMoves() const;
    void clearScreen() const;
    std::string stoneToString(GomokuLib::Stone stone) const;

public:
    // コンストラクタ
    GomokuCLI();

    // メインループ
    void run();

    // ゲームが開始されているか確認
    bool isGameStarted() const;
};
