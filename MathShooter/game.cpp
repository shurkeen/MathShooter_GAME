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

    resize(Game::SIZE);

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

    { // связь изменений массивов декартовых координат между классами Game и Field
        connect(this, &Game::coordUpdated, this->m_field, [this]{this->m_field->updateCoordGraph(this->m_dots);});
    }

    { // связь изменений количества игроков между классами Game и Field
        connect(this, &Game::countPlayersUpdated, this->m_field, [this]{this->m_field->updateCountPlayers(this->numberOfPlayers);});
    }

    { // связь полученных координат графика функции и начала хода игрока
        connect(this, &Game::gotTheCoordinates, this->m_field, &Field::movePlayer);
    }

    { // связь кнопки "Fire" c вычислением координат графика на поле
        connect(fireBtn, &QPushButton::clicked, this, &Game::gettingCoord);
    }

    { // historyInput
        connect(fireBtn, &QPushButton::clicked, this->historyInput, [this]{this->historyInput->setText(this->functionInput->text());});
    }

    setWindowTitle("MathShooter");
}

void Game::initGame()
{
    gettingCountPlayers();
    // gettingCoord();
}

void Game::gettingCoord()
{

    Graph graph;
    graph.Input(functionInput->text().toStdString());
    graph.Evaluate(-Field::X_LENGTH / 2, Field::X_LENGTH / 2);

    m_dots.clear();
    for(int i = 1; i < graph.getNumberOfGraphPoints(); i++){
        m_dots.push_back({graph.getDekartPosInGraphForX_Axis(i), graph.getDekartPosInGraphForY_Axis(i)});
    }
    //m_dots.push_back({1, 2});

    emit this->coordUpdated();
    emit this->gotTheCoordinates();
}

void Game::gettingCountPlayers()
{
    numberOfPlayers = 4;
    emit this->countPlayersUpdated();
}
