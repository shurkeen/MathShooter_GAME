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
    int getCenterPosDekartX();
    int getCenterPosDekartY();
    int getCenterPosScreenX();
    int getCenterPosScreenY();
    int getCoordinateQuarter();

    constexpr static const double M_RADIUS = 0.75;
    constexpr static const double K_POLYGON = 0.005;

private:
    int centerPosDekartX;
    int centerPosDekartY;
    int centerPosScreenX;
    int centerPosScreenY;
};

#endif // PLAYER_H
