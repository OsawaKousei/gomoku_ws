#pragma once

#include <QWidget>
#include "GomokuLib/Board.h"

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = nullptr);
    void setBoard(const GomokuLib::Board *board);

signals:
    void moveSelected(int row, int col);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    const GomokuLib::Board *board;
};