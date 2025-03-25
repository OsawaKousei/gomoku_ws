#pragma once

#include <QMainWindow>
#include <QListWidget>

#include "GomokuLib/Game.h"
#include "GomokuGUI/BoardWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGame();
    void openGame();
    void saveGame();
    void undoMove();
    void onMovePlayed(int row, int col);

private:
    GomokuLib::Game *game;
    BoardWidget *boardWidget;
    QListWidget *moveHistoryList;

    void resetGame(int boardSize = 15);
    void updateUI();
};