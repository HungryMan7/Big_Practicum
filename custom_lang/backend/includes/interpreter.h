#pragma once
#include "rpn.h"

class Interpreter {
public:
    void Run(RPN* rpn_chain);

private:
    std::map<std::string, std::vector<RPN_Node*>> functions;
    int ind_;
};