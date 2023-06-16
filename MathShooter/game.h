#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QPainter>
#include <QVector>
#include <QLayout>
#include <QDialog>
#include <QTimer>
#include "field.h"
#include "graph.h"

class Game : public QWidget
{
    Q_OBJECT

public:
    Game(QWidget* parent = 0);
    void initGame(int);
    void initGame(QVector<Obstacle>, QVector<Player>);
    Field* getPointField();
    constexpr static const QSize SIZE = QSize(885, 624);
    void gettingCoordForString();
    QString getFireFunction();
    void setFireFunction();
    int getCountPlayers();
    void lockFireBtn();
    void unlockFireBtn();
    void fireGraph();
    void updateMoveTime(int);
    void setColorTimeText(QColor);
    void startTimerGettingCoord();
    void startTimerGettingCoordForString(QString);

public slots:
    void gettingCoord();

signals:
    void coordUpdated();
    void countPlayersUpdated();
    void gotTheCoordinates();
    void sendFunctionToServer();
    void shotTimeEndSignal();
    void pushFireBtn();
    void readyGettingCoord();

private:

    Field* m_field;
    QPushButton* fireBtn;
    QPushButton* quitBtn;
    QLineEdit* functionInput;
    QLabel* historyInput;
    QLabel* timeLabel;
    QFont* fontTimeText;
    QTimer* timerStartGettingCoord;
    QTimer* timerStartGettingCoordForString;
    QVBoxLayout* layoutLeft;
    QHBoxLayout* layoutBack;
    QVBoxLayout* layout;
    QHBoxLayout* buttonsAndTimeLayout;
    QVector<QPair<double, double> > m_dots;
    int numberOfPlayers;
    QString fireFunction;

};

#endif // GAME_H
