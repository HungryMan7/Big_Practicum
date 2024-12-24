#include "../includes/interpreter.h"
#include <stack>

void Interpreter::Run(std::vector<RPN_Node*> rpn) {
    rpn_ = rpn;
    RunFunc("main", -1);
}

void Interpreter::RunFunc(std::string func_name, int label_num) {
    RPN_TID* tid = new RPN_TID;
    std::stack<RPN_Node*> stack;
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
        if (rpn_[ind]->GetType() == "function") {
            RunFunc(rpn_[ind]->GetName(), -1);
            
        } else if (rpn_[ind]->GetType() == "other") {
            RPN_Node* second = stack.top();
            stack.pop();
            RPN_Node* first = stack.top();
            stack.pop();
            stack.push(Solve(first, second, rpn_[ind]->GetOperation()));
        } else if (rpn_[ind]->GetType() == "identifier") {
            RPN_Node* copy = new RPN_Node(rpn_[ind]);
            stack.push(copy);
        }
    }
}

RPN_Node*& Interpreter::Solve(RPN_Node*& first, RPN_Node*& second, std::string operation) {
    if (operation == "=") {
        if (second->GetType() == "integer") {
            first->SetIntegerValue(second->GetIntegerValue());
            return first;
        } else if (second->GetType() == "double") {
            first->SetDoubleValue(second->GetDoubleValue());
            return first;
        } else if (second->GetType() == "bool") {
            first->SetBoolValue(second->GetBoolValue());
            return first;
        } else if (second->GetType() == "string") {
            first->SetStringValue(second->GetStringValue());
            return first;
        } else {
            throw "ne nado...";
        }
        std::cout << "it workssss!!!!\n";
    }
}