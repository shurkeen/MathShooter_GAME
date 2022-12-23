#ifndef MATHCALCULATOR_H
#define MATHCALCULATOR_H


#include "mathexpression.h"
#include <cmath>
#include <vector>

class MathCalculator {
private:
    std::vector<std::string> list;
    MathChecker checker;
public:
    MathCalculator();

    explicit MathCalculator(MathExpression);

    double Calculate(double);
};


#endif // MATHCALCULATOR_H
