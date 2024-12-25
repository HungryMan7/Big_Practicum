#pragma once
#include "rpn.h"

class Interpreter {
public:
    void Run(std::vector<RPN_Node*> rpn);
    RPN_Node* RunFunc(std::string, int);
    RPN_Node* Solve(RPN_Node*, RPN_Node*, std::string);

private:
    std::vector<RPN_Node*> rpn_;
    RPN_TID* tid_;
};