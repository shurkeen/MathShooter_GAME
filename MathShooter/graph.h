#ifndef GRAPH_H
#define GRAPH_H


#include "mathcalculator.h"
#include <vector>

class Graph {
private:
    std::string color;
    std::vector <std::pair<double, double>> valueTable;
    MathCalculator calculator;
public:
    Graph();

    void Evaluate(double, double);

    void Input(std::string);
    int getNumberOfGraphPoints();
    double getDekartPosInGraphForX_Axis(int);
    double getDekartPosInGraphForY_Axis(int);

    constexpr static const double RENDER_STEP_GRAPH = 0.001;
};

#endif // GRAPH_H
