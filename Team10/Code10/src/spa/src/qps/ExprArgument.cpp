#include "ExprArgument.h"

#include <utility>

ExprArgument::ExprArgument(ArgType type, std::string value, std::shared_ptr<ExprNode> exprAST) : Argument(type, std::move(value)), exprAST(std::move(exprAST)) {}

bool ExprArgument::isPartialMatch() {
    if (type == EXPRESSION) {
        return this->value.substr(0, 2) == "_\"";
    } else {
        return false;
    }
};

bool ExprArgument::isFullMatch() {
    if (type == EXPRESSION) {
        return (this->value[0]) == '\"';
    } else {
        return false;
    }
};

std::shared_ptr<ExprNode> ExprArgument::getExprAST() {
    return this->exprAST;
}
