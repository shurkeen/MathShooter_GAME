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
    //setFixedSize(size());

    m_field = new Field(this);

    fireBtn = new QPushButton("Fire");
    quitBtn = new QPushButton("Quit");

    functionInput = new QLineEdit();
    functionInput->setFixedSize(500, 20);

    historyInput = new QLabel();
    historyInput->setFrameStyle(QFrame::Box);

    QVBoxLayout* layoutLeft = new QVBoxLayout;
    layoutLeft->addWidget(functionInput);
    layoutLeft->addWidget(fireBtn);
    layoutLeft->addWidget(quitBtn);

    QHBoxLayout* layoutBack = new QHBoxLayout;
    layoutBack->addLayout(layoutLeft);
    layoutBack->addWidget(historyInput);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(m_field);
    layout->addLayout(layoutBack);

    this->setLayout(layout);

    setWindowTitle("MathShooter");
}

void Game::initGame()
{
    m_field->updateField();
    // цикл перехода хода игроков
}
