#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "field.h"

class Game : public QWidget
{
public:
    Game(QWidget* parent = 0);
    void initGame();

private:
    Field* m_field;
    QPushButton* fireBtn;
    QPushButton* quitBtn;
    QLineEdit* functionInput;
    QLabel* historyInput;
};

#endif // GAME_H
