#include "../includes/interpreter.h"
#include <stack>

void Interpreter::Run(std::vector<RPN_Node*> rpn) {
    rpn_ = rpn;
    RunFunc("main", -1);
}

void Interpreter::RunFunc(std::string func_name, int label_num) { // label_num == -1 means starting from the title of the function (without seeking concrete label)
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
        }
        if (rpn_[ind]->GetType() == "other") {
            RPN_Node* second = stack.top();
            stack.pop();
            RPN_Node* first = stack.top();
            stack.pop();
            stack.push(Solve(first, second, rpn_[ind]->GetOperation()));
            std::cout << (stack.top())->GetBoolValue() << "\n";
        } else if (rpn_[ind]->GetType() == "identifier" || rpn_[ind]->GetType() == "integer" ||
                   rpn_[ind]->GetType() == "double" || rpn_[ind]->GetType() == "bool" ||
                   rpn_[ind]->GetType() == "string" || rpn_[ind]->GetType() == "array" ||
                   rpn_[ind]->GetType() == "function") {
            RPN_Node* copy = new RPN_Node(*rpn_[ind]);
            stack.push(copy);
            if (rpn_[ind]->GetType() == "identifier") {
                if (!tid->FindID(rpn_[ind]->GetName())) {
                    tid->AddID(rpn_[ind]->GetName(), copy);
                }
            }
        }
    }

}

RPN_Node* Interpreter::Solve(RPN_Node*& first, RPN_Node*& second, std::string operation) {
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
            throw "ne nado..."; // юмор - это хорошо :)
        }
    } else if (operation == "==") {
        RPN_Node* result = new RPN_Node("", "bool");
        if (second->GetType() == "integer") {
            result->SetBoolValue(first->GetIntegerValue() == second->GetIntegerValue());
            return result;
        } else if (second->GetType() == "double") {
            result->SetBoolValue(first->GetDoubleValue() == second->GetDoubleValue());
            return result;
        } else if (second->GetType() == "bool") {
            result->SetBoolValue(first->GetBoolValue() == second->GetBoolValue());
            return result;
        } else if (second->GetType() == "string") {
            result->SetBoolValue(first->GetStringValue() == second->GetStringValue());
            return result;
        } else {
            throw "ne nado...";
        }
    } else if (operation == "+") {
        if (second->GetType() == "integer") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetIntegerValue() + second->GetIntegerValue()), "integer");
            return answer;
        } else if (second->GetType() == "double") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetDoubleValue() + second->GetDoubleValue()), "double");
            return answer;
        } else if (second->GetType() == "string") {
            RPN_Node* answer = new RPN_Node(first->GetStringValue() + second->GetStringValue(), "string");
            return answer;
        }
    } else if (operation == "-") {
        if (second->GetType() == "integer") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetIntegerValue() - second->GetIntegerValue()), "integer");
            return answer;
        } else if (second->GetType() == "double") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetDoubleValue() - second->GetDoubleValue()), "double");
            return answer;
        }
    } else if (operation == "*") {
        if (second->GetType() == "integer") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetIntegerValue() * second->GetIntegerValue()), "integer");
            return answer;
        } else if (second->GetType() == "double") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetDoubleValue() * second->GetDoubleValue()), "double");
            return answer;
        }
    } else if (operation == "/") {
        if (second->GetType() == "integer") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetIntegerValue() / second->GetIntegerValue()), "integer");
            return answer;
        } else if (second->GetType() == "double") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetDoubleValue() / second->GetDoubleValue()), "double");
            return answer;
        }
    } else if (operation == "%") {
        if (second->GetType() == "integer") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetIntegerValue() % second->GetIntegerValue()), "integer");
            return answer;
        }
    } else if (operation == "<<") {
        if (second->GetType() == "integer") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetIntegerValue() << second->GetIntegerValue()), "integer");
            return answer;
        }
    } else if (operation == ">>") {
        if (second->GetType() == "integer") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetIntegerValue() >> second->GetIntegerValue()), "integer");
            return answer;
        }
    } else if (operation == ">>") {
        if (second->GetType() == "integer") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetIntegerValue() >> second->GetIntegerValue()), "integer");
            return answer;
        }
    } else if (operation == ",") {
        return second;
    }
    return new RPN_Node;
}