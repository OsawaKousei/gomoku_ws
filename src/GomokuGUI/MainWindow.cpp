#include "GomokuGUI/MainWindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), game(nullptr)
{
    setWindowTitle("GomokuGUI");
    resize(800, 600);

    // メニュー
    QMenu *fileMenu = menuBar()->addMenu(tr("ファイル"));
    QAction *newGameAction = fileMenu->addAction(tr("新規ゲーム"));
    QAction *openAction = fileMenu->addAction(tr("開く"));
    QAction *saveAction = fileMenu->addAction(tr("保存"));
    QAction *exitAction = fileMenu->addAction(tr("終了"));

    QMenu *gameMenu = menuBar()->addMenu(tr("ゲーム"));
    QAction *undoAction = gameMenu->addAction(tr("一手戻す"));

    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);
    connect(openAction, &QAction::triggered, this, &MainWindow::openGame);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveGame);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undoMove);

    // ツールバー
    QToolBar *toolBar = addToolBar(tr("MainToolbar"));
    toolBar->addAction(newGameAction);
    toolBar->addAction(openAction);
    toolBar->addAction(saveAction);
    toolBar->addAction(undoAction);

    // 中央ウィジェット
    QWidget *central = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(central);

    boardWidget = new BoardWidget(this);
    moveHistoryList = new QListWidget(this);
    connect(boardWidget, &BoardWidget::moveSelected, this, &MainWindow::onMovePlayed);

    QSplitter *splitter = new QSplitter(this);
    QWidget *boardContainer = new QWidget();
    QVBoxLayout *boardLayout = new QVBoxLayout(boardContainer);
    boardLayout->addWidget(boardWidget);

    QWidget *historyContainer = new QWidget();
    QVBoxLayout *historyLayout = new QVBoxLayout(historyContainer);
    QLabel *label = new QLabel(tr("棋譜"));
    historyLayout->addWidget(label);
    historyLayout->addWidget(moveHistoryList);

    splitter->addWidget(boardContainer);
    splitter->addWidget(historyContainer);

    layout->addWidget(splitter);
    setCentralWidget(central);

    resetGame();
}

MainWindow::~MainWindow()
{
    delete game;
}

void MainWindow::resetGame(int boardSize)
{
    if (game)
    {
        delete game;
    }
    game = new GomokuLib::Game(boardSize);

    boardWidget->setBoard(&(game->getBoard()));
    moveHistoryList->clear();
    updateUI();
}

void MainWindow::newGame()
{
    bool ok;
    int size = QInputDialog::getInt(this, tr("新規ゲーム"), tr("盤面サイズ:"), 15, 5, 25, 1, &ok);
    if (ok)
    {
        resetGame(size);
    }
}

void MainWindow::openGame()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("棋譜を開く"), "", tr("棋譜ファイル (*.sgf)"));
    if (filePath.isEmpty())
        return;

    try
    {
        GomokuLib::Game loaded = GomokuLib::Game::loadGame(filePath.toStdString());
        if (game)
        {
            delete game;
        }
        game = new GomokuLib::Game(loaded);
        boardWidget->setBoard(&(game->getBoard()));
        updateUI();
    }
    catch (...)
    {
        QMessageBox::critical(this, tr("エラー"), tr("棋譜の読み込みに失敗しました"));
    }
}

void MainWindow::saveGame()
{
    if (!game)
        return;

    QString filePath = QFileDialog::getSaveFileName(this, tr("棋譜を保存"), "", tr("棋譜ファイル (*.sgf)"));
    if (filePath.isEmpty())
        return;

    try
    {
        game->saveGame(filePath.toStdString());
    }
    catch (...)
    {
        QMessageBox::critical(this, tr("エラー"), tr("棋譜の保存に失敗しました"));
    }
}

void MainWindow::undoMove()
{
    if (game && game->undoMove())
    {
        updateUI();
    }
}

void MainWindow::onMovePlayed(int row, int col)
{
    if (!game)
        return;
    auto result = game->playTurn(row, col);
    if (result == GomokuLib::MoveResult::INVALID_MOVE)
    {
        statusBar()->showMessage(tr("無効な手です"), 2000);
    }
    updateUI();
}

void MainWindow::updateUI()
{
    if (!game)
        return;
    moveHistoryList->clear();
    auto moves = game->getMoves();
    GomokuLib::Stone s = GomokuLib::Stone::BLACK;
    for (size_t i = 0; i < moves.size(); i++)
    {
        QString color = (s == GomokuLib::Stone::BLACK) ? tr("黒") : tr("白");
        moveHistoryList->addItem(QString("%1: (%2, %3) %4").arg(i + 1).arg(moves[i].first).arg(moves[i].second).arg(color));
        s = (s == GomokuLib::Stone::BLACK) ? GomokuLib::Stone::WHITE : GomokuLib::Stone::BLACK;
    }
    boardWidget->update();
}