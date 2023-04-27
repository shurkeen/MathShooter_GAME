#ifndef OBSTACLE_H
#define OBSTACLE_H

#include<QVector>
#include<QPair>
#include<QPoint>

class Obstacle
{
public:
    Obstacle();
    Obstacle(double, double);
    void setCenterPosDekartX(double);
    void setCenterPosDekartY(double);
    void setCenterPosScreenX(int);
    void setCenterPosScreenY(int);
    void setRadius(double);
    double getCenterPosDekartX();
    double getCenterPosDekartY();
    int getCenterPosScreenX();
    int getCenterPosScreenY();
    double getRadius();

    const static int MAX_RADIUS = 3;
    const static int MIN_RADIUS = 1;
    constexpr static const double EXTRA_EPS_RADIUS = 0.3;
    constexpr static const double K_POLYGON = 0.01;

private:
    double centerPosDekartX;
    double centerPosDekartY;
    int centerPosScreenX;
    int centerPosScreenY;
    double m_radius;
};

#endif // OBSTACLE_H
