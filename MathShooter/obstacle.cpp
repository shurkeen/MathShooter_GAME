#include "obstacle.h"

Obstacle::Obstacle()
{

}

Obstacle::Obstacle(double newPosCenterX, double newPosCenterY)
{
    centerPosDekartX = newPosCenterX;
    centerPosDekartY = newPosCenterY;
}

void Obstacle::setCenterPosDekartX(double newPosCenterX)
{
    centerPosDekartX = newPosCenterX;
}

void Obstacle::setCenterPosDekartY(double newPosCenterY)
{
    centerPosDekartY = newPosCenterY;
}

void Obstacle::setCenterPosScreenX(int newPosCenterX)
{
    centerPosScreenX = newPosCenterX;
}

void Obstacle::setCenterPosScreenY(int newPosCenterY)
{
    centerPosScreenY = newPosCenterY;
}

void Obstacle::setRadius(double radius)
{
    m_radius = radius;
}

double Obstacle::getCenterPosDekartX()
{
    return centerPosDekartX;
}

double Obstacle::getCenterPosDekartY()
{
    return centerPosDekartY;
}

int Obstacle::getCenterPosScreenX()
{
    return centerPosScreenX;
}

int Obstacle::getCenterPosScreenY()
{
    return centerPosScreenY;
}

double Obstacle::getRadius()
{
    return m_radius;
}
