#include "GomokuCLI/GomokuCLI.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>

GomokuCLI::GomokuCLI() : isRunning(true), gameLoaded(false)
{
    registerCommands();
}

void GomokuCLI::registerCommands()
{
    // コマンドとハンドラーを登録
    commandHandlers["start"] = [this](const auto &args)
    { handleStart(args); };
    commandHandlers["place"] = [this](const auto &args)
    { handlePlace(args); };
    commandHandlers["show"] = [this](const auto &args)
    { handleShow(args); };
    commandHandlers["moves"] = [this](const auto &args)
    { handleMoves(args); };
    commandHandlers["save"] = [this](const auto &args)
    { handleSave(args); };
    commandHandlers["load"] = [this](const auto &args)
    { handleLoad(args); };
    commandHandlers["undo"] = [this](const auto &args)
    { handleUndo(args); };
    commandHandlers["exit"] = [this](const auto &args)
    { handleExit(args); };
    commandHandlers["quit"] = [this](const auto &args)
    { handleExit(args); };
    commandHandlers["help"] = [this](const auto &args)
    { handleHelp(args); };
}

void GomokuCLI::run()
{
    std::cout << "Welcome to Gomoku CLI!" << std::endl;
    std::cout << "Type 'help' for a list of available commands." << std::endl;

    std::string input;
    while (isRunning)
    {
        std::cout << "\nGomoku> ";
        std::getline(std::cin, input);

        if (input.empty())
            continue;

        auto tokens = parseCommand(input);
        if (!tokens.empty())
        {
            if (!executeCommand(tokens))
            {
                std::cerr << "Unknown command: " << tokens[0] << std::endl;
                std::cerr << "Type 'help' for a list of available commands." << std::endl;
            }
        }
    }
}

std::vector<std::string> GomokuCLI::parseCommand(const std::string &input)
{
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;

    while (iss >> token)
    {
        // コマンドは小文字に統一
        std::transform(token.begin(), token.end(), token.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });
        tokens.push_back(token);
    }

    return tokens;
}

bool GomokuCLI::executeCommand(const std::vector<std::string> &tokens)
{
    const std::string &command = tokens[0];

    auto it = commandHandlers.find(command);
    if (it != commandHandlers.end())
    {
        it->second(tokens);
        return true;
    }

    return false;
}

bool GomokuCLI::isGameStarted() const
{
    return game != nullptr;
}

// コマンド実装: start
void GomokuCLI::handleStart(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cerr << "Error: Please specify board size. Usage: start <size>" << std::endl;
        return;
    }

    try
    {
        int size = std::stoi(args[1]);
        if (size < 5)
        {
            std::cerr << "Error: Board size must be at least 5." << std::endl;
            return;
        }

        game = std::make_unique<GomokuLib::Game>(size);
        gameLoaded = true;
        clearScreen();
        std::cout << "New game started with board size " << size << "x" << size << std::endl;
        displayBoard();
        displayGameStatus();
    }
    catch (const std::invalid_argument &)
    {
        std::cerr << "Error: Invalid board size. Please enter a valid number." << std::endl;
    }
}

// コマンド実装: place
void GomokuCLI::handlePlace(const std::vector<std::string> &args)
{
    if (!isGameStarted())
    {
        std::cerr << "Error: No game in progress. Use 'start <size>' to start a new game." << std::endl;
        return;
    }

    if (args.size() < 3)
    {
        std::cerr << "Error: Please specify row and column. Usage: place <row> <col>" << std::endl;
        return;
    }

    try
    {
        int row = std::stoi(args[1]);
        int col = std::stoi(args[2]);

        auto result = game->playTurn(row, col);
        clearScreen();

        switch (result)
        {
        case GomokuLib::MoveResult::SUCCESS:
            std::cout << "Stone placed at (" << row << "," << col << ")" << std::endl;
            displayBoard();

            if (game->isGameOver())
            {
                auto winner = game->getWinner();
                if (winner == GomokuLib::Stone::BLACK)
                {
                    std::cout << "Black wins!" << std::endl;
                }
                else if (winner == GomokuLib::Stone::WHITE)
                {
                    std::cout << "White wins!" << std::endl;
                }
                else if (winner == GomokuLib::Stone::DRAW)
                {
                    std::cout << "The game ended in a draw!" << std::endl;
                }
            }
            else
            {
                displayGameStatus();
            }
            break;

        case GomokuLib::MoveResult::INVALID_MOVE:
            std::cerr << "Error: Invalid move. The position is either occupied or out of bounds." << std::endl;
            displayBoard();
            displayGameStatus();
            break;

        case GomokuLib::MoveResult::GAME_OVER:
            std::cerr << "Error: The game is already over." << std::endl;
            displayBoard();
            std::cout << "Game is over. Start a new game or load a saved one." << std::endl;
            break;
        }
    }
    catch (const std::invalid_argument &)
    {
        std::cerr << "Error: Invalid coordinates. Please enter valid numbers." << std::endl;
    }
}

// コマンド実装: show
void GomokuCLI::handleShow(const std::vector<std::string> &args)
{
    if (!isGameStarted())
    {
        std::cerr << "Error: No game in progress. Use 'start <size>' to start a new game." << std::endl;
        return;
    }

    clearScreen();
    displayBoard();
    displayGameStatus();
}

// コマンド実装: moves
void GomokuCLI::handleMoves(const std::vector<std::string> &args)
{
    if (!isGameStarted())
    {
        std::cerr << "Error: No game in progress. Use 'start <size>' to start a new game." << std::endl;
        return;
    }

    clearScreen();
    displayMoves();
    displayBoard();
    displayGameStatus();
}

// コマンド実装: save
void GomokuCLI::handleSave(const std::vector<std::string> &args)
{
    if (!isGameStarted())
    {
        std::cerr << "Error: No game in progress. Use 'start <size>' to start a new game." << std::endl;
        return;
    }

    if (args.size() < 2)
    {
        std::cerr << "Error: Please specify a filename. Usage: save <filename>" << std::endl;
        return;
    }

    try
    {
        std::string filename = args[1];
        game->saveGame(filename);
        std::cout << "Game saved to " << filename << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error saving game: " << e.what() << std::endl;
    }
}

// コマンド実装: load
void GomokuCLI::handleLoad(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cerr << "Error: Please specify a filename. Usage: load <filename> [--resume]" << std::endl;
        return;
    }

    try
    {
        std::string filename;
        bool resume = false;

        // --resume オプションの確認
        if (args.size() >= 3 && args[1] == "--resume" && args.size() >= 3)
        {
            filename = args[2];
            resume = true;
        }
        else if (args.size() >= 3 && args[2] == "--resume")
        {
            filename = args[1];
            resume = true;
        }
        else
        {
            filename = args[1];
        }

        // ゲームの読み込み
        game = std::make_unique<GomokuLib::Game>(GomokuLib::Game::loadGame(filename));
        gameLoaded = true;
        clearScreen();

        std::cout << "Game loaded from " << filename << std::endl;
        displayBoard();

        if (resume)
        {
            displayGameStatus();
        }
        else
        {
            std::cout << "Game loaded for viewing. Type 'show' to see the board or 'moves' to see the move history." << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading game: " << e.what() << std::endl;
    }
}

// コマンド実装: undo
void GomokuCLI::handleUndo(const std::vector<std::string> &args)
{
    if (!isGameStarted())
    {
        std::cerr << "Error: No game in progress. Use 'start <size>' to start a new game." << std::endl;
        return;
    }

    if (game->undoMove())
    {
        clearScreen();
        std::cout << "Last move undone." << std::endl;
        displayBoard();
        displayGameStatus();
    }
    else
    {
        std::cerr << "Error: Cannot undo. No moves to undo." << std::endl;
    }
}

// コマンド実装: exit/quit
void GomokuCLI::handleExit(const std::vector<std::string> &args)
{
    isRunning = false;
    std::cout << "Thank you for playing Gomoku. Goodbye!" << std::endl;
}

// コマンド実装: help
void GomokuCLI::handleHelp(const std::vector<std::string> &args)
{
    clearScreen();
    std::cout << "Available commands:" << std::endl;
    std::cout << "-----------------" << std::endl;
    std::cout << "start <size>              - Start a new game with specified board size" << std::endl;
    std::cout << "place <row> <col>         - Place a stone at the specified position" << std::endl;
    std::cout << "show                      - Display the current board" << std::endl;
    std::cout << "moves                     - Display the move history" << std::endl;
    std::cout << "save <filename>           - Save the current game to a file" << std::endl;
    std::cout << "load <filename>           - Load a game from a file for viewing" << std::endl;
    std::cout << "load --resume <filename>  - Load a game from a file and resume playing" << std::endl;
    std::cout << "undo                      - Undo the last move" << std::endl;
    std::cout << "exit / quit               - Exit the application" << std::endl;
    std::cout << "help                      - Display this help message" << std::endl;
}

// ユーティリティメソッド: 盤面表示
void GomokuCLI::displayBoard() const
{
    if (!isGameStarted())
        return;

    const auto &board = game->getBoard();
    int size = board.getSize();

    // 列番号の表示
    std::cout << "  ";
    for (int col = 0; col < size; col++)
    {
        std::cout << " " << col;
    }
    std::cout << std::endl;

    // 盤面の表示
    for (int row = 0; row < size; row++)
    {
        std::cout << std::setw(2) << row << " ";
        for (int col = 0; col < size; col++)
        {
            auto stone = board.getStone(row, col);
            char symbol = '.';
            if (stone == GomokuLib::Stone::BLACK)
                symbol = 'B';
            else if (stone == GomokuLib::Stone::WHITE)
                symbol = 'W';
            std::cout << " " << symbol;
        }
        std::cout << std::endl;
    }
}

// ユーティリティメソッド: ゲーム状態表示
void GomokuCLI::displayGameStatus() const
{
    if (!isGameStarted())
        return;

    std::cout << std::endl;
    std::cout << "Current Player: " << stoneToString(game->getCurrentPlayer()) << std::endl;
}

// ユーティリティメソッド: 棋譜表示
void GomokuCLI::displayMoves() const
{
    if (!isGameStarted())
        return;

    const auto &moves = game->getMoves();
    if (moves.empty())
    {
        std::cout << "No moves have been made yet." << std::endl;
        return;
    }

    std::cout << "Move history:" << std::endl;
    std::cout << "------------" << std::endl;

    GomokuLib::Stone stone = GomokuLib::Stone::BLACK; // 最初の手は黒
    for (size_t i = 0; i < moves.size(); i++)
    {
        const auto &move = moves[i];
        std::cout << i + 1 << ". " << stoneToString(stone) << " (" << move.first << "," << move.second << ")" << std::endl;
        // プレイヤーの交代
        stone = (stone == GomokuLib::Stone::BLACK) ? GomokuLib::Stone::WHITE : GomokuLib::Stone::BLACK;
    }
    std::cout << std::endl;
}

// ユーティリティメソッド: 画面クリア
void GomokuCLI::clearScreen() const
{
    // クロスプラットフォーム対応のため、単純な方法でクリア
    std::cout << std::string(50, '\n');
}

// ユーティリティメソッド: 石の文字列表現
std::string GomokuCLI::stoneToString(GomokuLib::Stone stone) const
{
    switch (stone)
    {
    case GomokuLib::Stone::BLACK:
        return "Black (B)";
    case GomokuLib::Stone::WHITE:
        return "White (W)";
    case GomokuLib::Stone::DRAW:
        return "Draw";
    default:
        return "Empty";
    }
}
