#ifndef FIELD_H
#define FIELD_H

#include <QFrame>
#include <QPainter>
#include <QSignalMapper>
#include <QPair>
#include <QRandomGenerator>
#include <QTime>
#include <QDebug>
#include "player.h"
#include "obstacle.h"

class Field : public QFrame
{
    Q_OBJECT

public:
    Field(QWidget* parent = 0);
    int getNumberOfPlayers();
    void movePlayer();
        void setNextPlayer();

public slots:
    void updateCoordGraph(const QVector<QPair<double, double>>& m_dots);
    void updateCountPlayers(int countPlayers);

protected:
    void timerEvent(QTimerEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;

private:
    static const int DELAY = 1;
    static const int distanceBetweenPlayerAndAxisX = 3;
    static const int distanceBetweenPlayerAndAxisY = 3;
    constexpr static const double KILL_EPS = 2e-3;
    constexpr static const double DIFF_EPS = 1e-6;
    constexpr static const double X_LENGTH = 50.0;
    constexpr static const double Y_LENGTH = 30.0;
    double distFactorForX;
    double distFactorForY;

    int timerId;
    int idxPoint;
    int idxPlayer;
    int inPaintingGraph;

    void doDrawing();
    void graphDrawing();
    void playersDrawing();
    void dekartSystemDrawing();
    void convertToScreenSystem();
    int convertX_Axes(double);
    int convertY_Axes(double);
    void updateDistFactor();
    bool checkingOutside(QPair<double, double> point);
    bool checkingIntersectionWithPlayers(QPair<double, double>, int);
    void attemptToKill(QPair<double, double> point);
    bool endLength();
    void endDrawingGraph();
    void initPlayers();
    void initCenterPosForPlayers();
    void initDotsForPlayers();
    double differenceBetweenY0andYi(const QVector<QPair<double, double>>& );
    void updateField();

    QVector<QPoint> screenDotsGraph;
    QVector<QPoint> tempScreenDotsGraph;
    QVector<QPair<double, double> > dekartDotsGraph;
    QFrame* fieldFrame;
    QPainter m_paint;
    QVector<Player> m_players;
};

#endif // FIELD_H
