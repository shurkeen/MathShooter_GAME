#include "game.h"
#include <QDebug>
#include <QPainter>
#include <QVector>
#include <QLabel>
#include <QLayout>
#include <QFrame>
#include <QDialog>

Game::Game(QWidget* parent) : QWidget(parent)
{
    resize(900, 600);
    setFixedSize(size());

    field = new Field(this);

    btn = new QPushButton("button");
    btn2 = new QPushButton("button2");
    btn3 = new QPushButton("button3");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(field);
    layout->addWidget(btn);
    layout->addWidget(btn2);
    layout->addWidget(btn3);

    this->setLayout(layout);

    setWindowTitle("MathShooter");
}

void Game::initGame()
{
    field->updateField();
}
