#pragma once
#include "rpn.h"

class Interpreter {
public:
    void Run(std::vector<RPN_Node*> rpn);
    void RunFunc(std::string, int);

private:
    std::vector<RPN_Node*> rpn_;
    std::string curr_func, prev_func;
};