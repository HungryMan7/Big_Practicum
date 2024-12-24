#include "../includes/interpreter.h"

void Interpreter::Run(RPN* rpn_chain) {
    std::vector<RPN_Node*> rpn = rpn_chain->GetChain();
    ind_ = 0;
    std::string name = rpn[ind_]->GetName();
    std::vector<RPN_Node*> new_func;
    ++ind_;
    while (ind_ < rpn.size()) {
        while (ind_ < rpn.size() && !rpn[ind_]->IsEmptyLabel()) {
            new_func.push_back(rpn[ind_]);
            ++ind_;
        }
        functions[name] = new_func;
        new_func.clear();
        ++ind_;
        if (ind_ < rpn.size()) {
            name = rpn[ind_]->GetName();
            ++ind_;
        }
    }
    // now code is divided in functions
}