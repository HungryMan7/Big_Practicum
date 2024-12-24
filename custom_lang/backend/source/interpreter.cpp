#include "../includes/interpreter.h"

void Interpreter::Run(std::vector<RPN_Node*> rpn) {
    rpn_ = rpn;
    RunFunc("main", -1);
}

void Interpreter::RunFunc(std::string func_name, int label_num) {
    int ind = 0;
    while (ind < rpn_.size() && rpn_[ind]->GetType() != "function" ||
           rpn_[ind]->GetName() != func_name) {
        ++ind;
    }
    if (ind == rpn_.size()) {
        throw "function or label not found!";
    }
    ++ind;
    if (label_num != -1) {
        while (ind < rpn_.size() && !rpn_[ind]->IsCommonLabel() ||
               rpn_[ind]->GetLabelNumber() != label_num) {
            ++ind;
        }
        if (ind == rpn_.size()) {
            throw "function or label not found!";
        }
        ++ind;
    }
    for (; rpn_[ind]->GetType() != "utility" || rpn_[ind]->GetKeyword() != "return"; ++ind) {
        // code
    }
    // return
}