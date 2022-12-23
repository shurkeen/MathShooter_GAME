#include "graph.h"
#include<QDebug>

Graph::Graph()
{

}

void Graph::Evaluate(double leftBorderField, double rightBorderField) {
    double st = leftBorderField;
    double fn = rightBorderField;
    double dx = Graph::RENDER_STEP_GRAPH;
    while(st < fn){
        valueTable.emplace_back(st, calculator.Calculate(st));
        st += dx;
    }
}

void Graph::Input(std::string initString) {
    std::string temp = initString;
    MathExpression expression;
    expression.SetExpression(temp);
    //qDebug() << "XXXX " <<  expression.SetExpression(temp);
    calculator = MathCalculator(expression);
}

int Graph::getNumberOfGraphPoints()
{
    return valueTable.size();
}

double Graph::getDekartPosInGraphForX_Axis(int idx)
{
    return valueTable[idx].first;
}

double Graph::getDekartPosInGraphForY_Axis(int idx)
{
        return valueTable[idx].second;
}
