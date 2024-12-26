#pragma once
#include "rpn.h"
#include <stack>

class Interpreter {
public:
    RPN_Node* Run(std::vector<RPN_Node*> rpn);
    RPN_Node* RunFunction(std::string);
    RPN_Node* Solve(RPN_Node*, RPN_Node*, std::string);

private:
    std::vector<RPN_Node*> rpn_;
    RPN_TID* tid_;
    std::stack<RPN_Node*> stack;
};