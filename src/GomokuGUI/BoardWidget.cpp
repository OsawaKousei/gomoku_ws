#include "GomokuGUI/BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>

BoardWidget::BoardWidget(QWidget *parent)
    : QWidget(parent),
      board(nullptr)
{
    setMinimumSize(400, 400);
}

void BoardWidget::setBoard(const GomokuLib::Board *newBoard)
{
    board = newBoard;
    update();
}

void BoardWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    // 背景
    painter.fillRect(rect(), QColor(240, 217, 181));

    if (!board)
        return;
    int boardSize = board->getSize();

    // グリッド線を描画
    for (int i = 0; i < boardSize; i++)
    {
        // 水平線
        painter.drawLine(10, 10 + i * 30, 10 + (boardSize - 1) * 30, 10 + i * 30);
        // 垂直線
        painter.drawLine(10 + i * 30, 10, 10 + i * 30, 10 + (boardSize - 1) * 30);
    }

    // 石を描画
    for (int r = 0; r < boardSize; r++)
    {
        for (int c = 0; c < boardSize; c++)
        {
            auto s = board->getStone(r, c);
            if (s == GomokuLib::Stone::BLACK || s == GomokuLib::Stone::WHITE)
            {
                int x = 10 + c * 30;
                int y = 10 + r * 30;
                QColor color = (s == GomokuLib::Stone::BLACK) ? Qt::black : Qt::white;
                painter.setBrush(color);
                painter.drawEllipse(x - 10 + 15, y - 10 + 15, 20, 20);
            }
        }
    }
}

void BoardWidget::mousePressEvent(QMouseEvent *event)
{
    if (!board)
        return;
    int x = event->x() - 10;
    int y = event->y() - 10;

    int col = x / 30;
    int row = y / 30;

    if (row >= 0 && row < board->getSize() && col >= 0 && col < board->getSize())
    {
        emit moveSelected(row, col);
    }
    QWidget::mousePressEvent(event);
}