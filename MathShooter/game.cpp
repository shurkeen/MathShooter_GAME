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

    { // компоновка виджетов
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
    }

    { // связь изменений массивов декартовых координат между классами Game и Fiedl
        connect(this, &Game::coordUpdated, this->m_field, [this]{this->m_field->updateCoordGraph(this->m_dots);});
    }

    setWindowTitle("MathShooter");
}

void Game::initGame()
{
    gettingCoord();
    m_field->updateField();
    // цикл перехода хода игроков
}

void Game::gettingCoord()
{
    m_dots.clear();
    int sz = 500;
    m_dots.resize(sz);
    double h = 0.05;
    for(int i = 0; i < sz; i++){
        m_dots[i].first = -25.0 + h;
        m_dots[i].second = 1.0;
        h += 0.05;
    }
    emit this->coordUpdated();
}
