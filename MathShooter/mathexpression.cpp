#include "mathexpression.h"

MathExpression::MathExpression()
{

}

bool MathExpression::SetExpression(const std::string &str) {
    if(str.empty()) {
        return false;
    }
    initial = str;
    MathParser parser;
    parser.Parse(initial);
    MathChecker checker(parser.GetList());
    if (!checker.CheckCorrectness()) {
        return false;
    }
    token_list = checker.GetList();
    return true;
}

std::string MathExpression::GetInitial() {
    return initial;
}

std::list<std::string> MathExpression::GetPostfix() {
    return token_list;
}
