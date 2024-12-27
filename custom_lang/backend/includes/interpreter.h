#pragma once
#include "rpn.h"
#include <stack>

class Interpreter {
public:
    void Run(std::vector<RPN_Node*> rpn);
    void RunFunc(std::string);
    RPN_Node* Solve(RPN_Node*, RPN_Node*, std::string);

private:
    std::vector<RPN_Node*> rpn_;
    RPN_TID* tid_;
    std::stack<RPN_Node*> stack;
    bool is_func = false;
};