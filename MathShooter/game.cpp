#include "game.h"
#include "QtWidgets/qapplication.h"

Game::Game(QWidget* parent) : QWidget(parent)
{

    resize(Game::SIZE);

    { // компоновка виджетов
        m_field = new Field(this);

        timerStartGettingCoord = new QTimer(this);
        timerStartGettingCoordForString = new QTimer(this);

        fireBtn = new QPushButton("Fire");
        fireBtn->setEnabled(false);
        quitBtn = new QPushButton("Quit");

        functionInput = new QLineEdit();
        functionInput->setFixedSize(500, 20);

        fontTimeText = new QFont("Arial", 60);

        timeLabel = new QLabel();
        timeLabel->setFont(*fontTimeText);
        timeLabel->setAlignment(Qt::AlignCenter);
        timeLabel->setFrameStyle(QFrame::Box);
        timeLabel->setStyleSheet("QLabel { border-radius: 8px; border-style: solid; border-width: 1px; padding: 8px; }");
        timeLabel->setFixedWidth(100);

        historyInput = new QLabel();
        historyInput->setFrameStyle(QFrame::Box);
        historyInput->setFixedWidth(230);

        layoutLeft = new QVBoxLayout();
        layoutLeft->addWidget(functionInput);
        layoutLeft->addWidget(fireBtn);
        layoutLeft->addWidget(quitBtn);

        buttonsAndTimeLayout = new QHBoxLayout();
        buttonsAndTimeLayout->addLayout(layoutLeft);
        buttonsAndTimeLayout->addWidget(timeLabel);

        layoutBack = new QHBoxLayout();
        layoutBack->addLayout(buttonsAndTimeLayout);
        layoutBack->addWidget(historyInput);

        layout = new QVBoxLayout(this);
        layout->addWidget(m_field);
        layout->addLayout(layoutBack);
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

    { // связь назатия fireBtn и изменения цвета цвета времени и запись в историю игры
        connect(fireBtn, &QPushButton::clicked, this, [this]{ this->historyInput->setText(this->functionInput->text()); emit pushFireBtn(); });
    }

    { // связь таймера с началом получения координат
        connect(timerStartGettingCoord, &QTimer::timeout, this, &Game::gettingCoord);
    }

    { // связь таймера с началом получения координат от пересланной строки от клиента
        connect(timerStartGettingCoordForString, &QTimer::timeout, this, &Game::gettingCoordForString);
    }

    { // сигнал о конце отрисовки графика
        connect(m_field, &Field::shotTimeEndSignal, this, [this]{ emit shotTimeEndSignal(); });
    }

    setWindowTitle("MathShooter");
}

void Game::initGame(int countPlayers)
{
    numberOfPlayers = countPlayers + 1;
    emit this->countPlayersUpdated();
}

void Game::initGame(QVector<Obstacle> m_obstacles, QVector<Player> m_players)
{
    this->m_field->updateCountPlayersAndInitStaticObjects(m_obstacles, m_players);
}

void Game::gettingCoord()
{
    timerStartGettingCoord -> stop();

    Graph graph;
    graph.Input(functionInput->text().toStdString());

    graph.Evaluate(-Field::X_LENGTH / 2, Field::X_LENGTH / 2);

    m_dots.clear();
    for(int i = 1; i < graph.getNumberOfGraphPoints(); i++){
        m_dots.push_back({graph.getDekartPosInGraphForX_Axis(i), graph.getDekartPosInGraphForY_Axis(i)});
    }

    emit readyGettingCoord();
}

void Game::gettingCoordForString()
{
    timerStartGettingCoordForString -> stop();

    Graph graph;
    graph.Input(fireFunction.toStdString());
    graph.Evaluate(-Field::X_LENGTH / 2, Field::X_LENGTH / 2);

    m_dots.clear();
    for(int i = 1; i < graph.getNumberOfGraphPoints(); i++){
        m_dots.push_back({graph.getDekartPosInGraphForX_Axis(i), graph.getDekartPosInGraphForY_Axis(i)});
    }

    emit readyGettingCoord();
}

QString Game::getFireFunction()
{
    return fireFunction;
}

void Game::setFireFunction()
{
    fireFunction = functionInput->text();
}

int Game::getCountPlayers()
{
    return m_field->getNumberOfPlayers();
}

void Game::lockFireBtn()
{
    fireBtn->setEnabled(false);
}

void Game::unlockFireBtn()
{
    fireBtn->setEnabled(true);
}

void Game::fireGraph()
{
    emit this->coordUpdated();
    emit this->gotTheCoordinates();
}

void Game::updateMoveTime(int time)
{
    QString str = "";
    if(time < 10){
        str += "0";
    }
    str += QString::number(time);
    timeLabel -> setText(str);
}

void Game::setColorTimeText(QColor color)
{
    timeLabel->setStyleSheet("QLabel { border-radius: 8px; border-style: solid; border-width: 1px; padding: 8px; color: " + color.name() +  "; }");
}

void Game::startTimerGettingCoord()
{
    timerStartGettingCoord -> start(30);
}

void Game::startTimerGettingCoordForString(QString str)
{
    fireFunction = str;

    timerStartGettingCoordForString -> start(30);
}

Field *Game::getPointField()
{
    return m_field;
}
