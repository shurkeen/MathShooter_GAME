#ifndef FIELD_H
#define FIELD_H

#include <QFrame>
#include <QPainter>
#include <QSignalMapper>
#include <QPair>
#include <QRandomGenerator>
#include <QTime>
#include <QDebug>
#include <QThread>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <algorithm>
#include <thread>
#include "player.h"
#include "obstacle.h"

class Field : public QFrame
{
    Q_OBJECT

public:
    Field(QWidget* parent = 0);
    int getNumberOfPlayers();
    QVector<Player> getPlayers();
    QVector<Obstacle> getPlayersIndestructibleObject();
    void updateCountPlayersAndInitStaticObjects(QVector<Obstacle>, QVector<Player>);
    void movePlayer();
    void setNextPlayer();

    constexpr static const double X_LENGTH = 50.0;
    constexpr static const double Y_LENGTH = 30.0;

public slots:
    void updateCoordGraph(const QVector<QPair<double, double>>& m_dots);
    void updateCountPlayers(int countPlayers);

protected:
    void timerEvent(QTimerEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;

private:
    static const int FIELD_WEIGHT = 860;
    static const int FIELD_HEIGHT = 500;
    constexpr static const QColor colorField = QColor(237, 237, 237);
    static const int DELAY = 3;
    static const int distanceBetweenPlayerAndAxisX = 7;
    static const int distanceBetweenPlayerAndAxisY = 3;
    constexpr static const double KILL_EPS = 2e-1;
    constexpr static const double DIFF_GRAPH_PLAYER_EPS = 2e-1;
    constexpr static const double INTERSECTION_EPS = 5e-1;
    constexpr static const double OUTSIDE_EPS = 6e-2;
    constexpr static const double DIFF_EPS = 1e-6;
    double distFactorForX;
    double distFactorForY;

    int timerId;
    int idxPoint;
    int idxPlayer;
    int inPaintingGraph;

    void doDrawing();
    void convertToScreenSystem();
    int convertX_Axes(double);
    int convertY_Axes(double);
    void updateDistFactor();
    bool checkingOutside(QPair<double, double> point);
    bool checkingIntersectionGraphWithPlayers(QPair<double, double>, int);
    bool checkingIntersectionGraphWithObstracles(QPair<double, double>);
    bool checkingIntersectionPlayersWithObstracle(double, double, double);
    bool attemptToKill(QPair<double, double> point);
    bool endLength();
    void endDrawingGraph();
    void initPlayers();
    void initPlayersFromServer(QVector<Player>&);
    void initCenterPosForPlayers();
    void setPixmapStaticObjects();
    void setDekartSystem();
    void setIndestructibleObstracle();
    void setPlayers();
    void setPointGraph();
    void setGroupPointsGraph();
    double differenceBetweenY0andYi(const QVector<QPair<double, double>>& );
    void updateField();
    void initCenterPosForIndestructibleObstracle();
    void initIndestructibleObstracle();
    void initObstacleFromServer(QVector<Obstacle>& );
    double distanceBetweenTwoPoints(double, double, double, double);
    double findOffsetXForCenterPosObstracle(double, double, double);
    double angleTanBetweenDxDyObstracle(double, double);

    QVector<QPoint> screenDotsGraph;
    QVector<QPoint> tempScreenDotsGraph;
    QVector<QPair<double, double> > dekartDotsGraph;
    QFrame* fieldFrame;
    QPixmap* fieldPixmapForGraph;
    QPixmap* fieldPixmapForStaticObjects;
    QPainter m_paint;
    QVector<Player> m_players;
    QVector<Obstacle> indestructibleObject;
};

#endif // FIELD_H
