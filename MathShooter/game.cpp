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

    {
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
    m_dots.resize(1000);
    for(int i = 0; i < 1000; i++){
        m_dots[i].rx() = i;
        m_dots[i].ry() = i;
    }
    emit this->coordUpdated();
}

void Game::testt(QVector<int> zzz)
{
    qDebug() << "XXXXX ZZZZZ.  " << zzz[0];
}
