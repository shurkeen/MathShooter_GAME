#ifndef PLAYER_H
#define PLAYER_H

#include <QVector>
#include <QPair>
#include <QPoint>

class Player
{
public:
    Player();
    void setCenterPosDekartX(int);
    void setCenterPosDekartY(int);
    void setCenterPosScreenX(int);
    void setCenterPosScreenY(int);

//private:
    constexpr static const double M_RADIUS = 1;
    constexpr static const double K_POLYGON = 0.005;
    int centerPosDekartX;
    int centerPosDekartY;
    int centerPosScreenX;
    int centerPosScreenY;
    QVector<QPair<double, double> > dekartDotsPlayer;
    QVector<QPoint> screenDotsPlayer;
};

#endif // PLAYER_H
