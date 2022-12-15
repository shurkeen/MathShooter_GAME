#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include "field.h"

class Game : public QWidget
{
public:
    Game(QWidget* parent = 0);
    void initGame();

private:
    Field* field;
    QPushButton* btn;
    QPushButton* btn2;
    QPushButton* btn3;
};

#endif // GAME_H
