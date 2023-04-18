#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include "field.h"
#include "graph.h"

class Game : public QWidget
{
    Q_OBJECT

public:
    Game(QWidget* parent = 0);
    void initGame();
    void gettingCountPlayers();

public slots:
    void gettingCoord();

signals:
    void coordUpdated();
    void countPlayersUpdated();
    void gotTheCoordinates();

private:

    Field* m_field;
    QPushButton* fireBtn;
    QPushButton* quitBtn;
    QLineEdit* functionInput;
    QLabel* historyInput;
    QVector<QPair<double, double> > m_dots;
    int numberOfPlayers;

};

#endif // GAME_H
