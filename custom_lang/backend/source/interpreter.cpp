#include "../includes/interpreter.h"
#include <stack>

void Interpreter::Run(std::vector<RPN_Node*> rpn) {
    rpn_ = rpn;
    tid_ = new RPN_TID;
    RunFunc("main", -1);
}

RPN_Node* Interpreter::RunFunc(std::string func_name, int label_num) { // label_num == -1 means starting from the title of the function (without seeking concrete label)
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
            stack.push(RunFunc(rpn_[ind]->GetName(), -1));
        }
        if (rpn_[ind]->GetType() == "other") {
            RPN_Node* second = stack.top();
            stack.pop();
            RPN_Node* first = stack.top();
            stack.pop();
            stack.push(Solve(first, second, rpn_[ind]->GetOperation()));
            delete first;
            delete second;
        } else if (rpn_[ind]->GetType() == "identifier" || rpn_[ind]->GetType() == "integer" ||
                   rpn_[ind]->GetType() == "double" || rpn_[ind]->GetType() == "bool" ||
                   rpn_[ind]->GetType() == "string" || rpn_[ind]->GetType() == "array") {
            stack.push(new RPN_Node(*rpn_[ind]));
            if (rpn_[ind]->GetType() == "identifier") {
                if (!tid_->FindID(rpn_[ind]->GetName())) {
                    tid_->AddID(rpn_[ind]->GetName(), new RPN_Node("0", "integer"));
                }
            }
        } else if (rpn_[ind]->GetType() == "label") {
            if (rpn_[ind]->IsGoFalseLabel()) {
                RPN_Node* condition = stack.top();
                stack.pop();
                if (!condition->GetBoolValue()) {
                    int target = rpn_[ind]->GetFalseLabelNumber();
                    ++ind;
                    while (!rpn_[ind]->IsFalseLabel() || rpn_[ind]->GetFalseLabelNumber() != target) {
                        ++ind;
                    }
                    --ind;
                }
                delete condition;
            } else if (rpn_[ind]->IsGoLabel()) {
                int target = rpn_[ind]->GetLabelNumber();
                ++ind;
                while (!rpn_[ind]->IsCommonLabel() || rpn_[ind]->GetLabelNumber() != target) {
                    ++ind;
                }
                --ind;
            } else if (rpn_[ind]->IsStartLabel()) {
                tid_->AddTable();
            } else if (rpn_[ind]->IsEndLabel()) {
                tid_->RemoveTable();
            }
        }
    }
    RPN_Node* result = stack.top();
    if (result->GetType() == "identifier") {
        result = tid_->getValue(result->GetName());
    }
    return result;
}

RPN_Node* Interpreter::Solve(RPN_Node* first, RPN_Node* second, std::string operation) {
    if (operation == "=") {
        std::string name = first->GetName();
        if (second->GetType() == "integer") {
            (tid_->getValue(first->GetName()))->SetIntegerValue(second->GetIntegerValue());
            (tid_->getValue(first->GetName()))->SetType(second->GetType());
            return new RPN_Node(*first);
        } else if (second->GetType() == "double") {
            first->SetDoubleValue(second->GetDoubleValue());
            return new RPN_Node(*first);
        } else if (second->GetType() == "bool") {
            first->SetBoolValue(second->GetBoolValue());
            return new RPN_Node(*first);
        } else if (second->GetType() == "string") {
            first->SetStringValue(second->GetStringValue());
            return new RPN_Node(*first);
        } else if (second->GetType() == "array") {
            first->SetElements(second->GetElements());
            first->SetSizes(second->GetSizesValue());
            return new RPN_Node(*first);
        } else {
            throw "ne nado..."; // юмор - это хорошо :)
                                // угу)))
        }
    }   
    if (first->GetType() == "identifier") {
        first = tid_->getValue(first->GetName());
    }
    if (second->GetType() == "identifier") {
        second = tid_->getValue(second->GetName());
    }
    if (operation == "==") {
        RPN_Node* result = new RPN_Node("", "bool");
        if (second->GetType() == "integer") {
            std::cout << first->GetIntegerValue() << " == " << second->GetIntegerValue() << "\n";
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
        } else if (second->GetType() == "array") {
            if (first->GetSizesValue() == second->GetSizesValue()) {
                if (first->GetElements().size() == second->GetElements().size()) {
                    for (int i = 0; i < first->GetElements().size(); ++i) {
                        if (result->GetBoolValue() != false) {
                            if (second->GetElements()[i]->GetType() == "integer") {
                                result->SetBoolValue(first->GetElements()[i]->GetIntegerValue() == second->GetElements()[i]->GetIntegerValue());
                            } else if (second->GetType() == "double") {
                                result->SetBoolValue(first->GetElements()[i]->GetDoubleValue() == second->GetElements()[i]->GetDoubleValue());
                            } else if (second->GetType() == "bool") {
                                result->SetBoolValue(first->GetElements()[i]->GetBoolValue() == second->GetElements()[i]->GetBoolValue());
                            } else if (second->GetType() == "string") {
                                result->SetBoolValue(first->GetElements()[i]->GetStringValue() == second->GetElements()[i]->GetStringValue());
                            }
                        }
                    }
                } else result = new RPN_Node("false", "bool");
            } else result = new RPN_Node("false", "bool");
            return result;
        } else {
            throw "ne nado...";
        }
    } else if (operation == "!=") {
        RPN_Node* result = new RPN_Node("", "bool");
        RPN_Node* equal = Solve(first, second, "==");
        result->SetBoolValue(!equal->GetBoolValue());
        return result;
    } else if (operation == ">") {
        RPN_Node* result = new RPN_Node("", "bool");
        if (second->GetType() == "integer") {
            result->SetBoolValue(first->GetIntegerValue() > second->GetIntegerValue());
            return result;
        } else if (second->GetType() == "double") {
            result->SetBoolValue(first->GetDoubleValue() > second->GetDoubleValue());
            return result;
        } else if (second->GetType() == "bool") {
            result->SetBoolValue(first->GetBoolValue() > second->GetBoolValue());
            return result;
        } else if (second->GetType() == "string") {
            result->SetBoolValue(first->GetStringValue() > second->GetStringValue());
            return result;
        } else {
            throw "ne nado...";
        }
    } else if (operation == "<") {
        RPN_Node* result = new RPN_Node("", "bool");
        RPN_Node* bigger = Solve(first, second, ">");
        RPN_Node* not_equal = Solve(first, second, "!=");
        result->SetBoolValue(!bigger->GetBoolValue() && not_equal->GetBoolValue());
        return result;
    } else if (operation == ">=") {
        RPN_Node* result = new RPN_Node("", "bool");
        RPN_Node* smaller = Solve(first, second, "<"); 
        result->SetBoolValue(!smaller->GetBoolValue());
        return result;
    } else if (operation == "<=") {
        RPN_Node* result = new RPN_Node("", "bool");
        RPN_Node* bigger = Solve(first, second, ">"); 
        result->SetBoolValue(!bigger->GetBoolValue());
        return result;
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
    } else if (operation == "&") {
        if (second->GetType() == "integer") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetIntegerValue() & second->GetIntegerValue()), "integer");
            return answer;
        }
    } else if (operation == "|") {
        if (second->GetType() == "integer") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetIntegerValue() | second->GetIntegerValue()), "integer");
            return answer;
        }
    } else if (operation == "^") {
        if (second->GetType() == "integer") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetIntegerValue() ^ second->GetIntegerValue()), "integer");
            return answer;
        }
    } else if (operation == "||") {
        if (second->GetType() == "bool") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetIntegerValue() || second->GetIntegerValue()), "bool");
            return answer;
        }
    } else if (operation == "&&") {
        if (second->GetType() == "bool") {
            RPN_Node* answer = new RPN_Node(std::to_string(first->GetIntegerValue() && second->GetIntegerValue()), "bool");
            return answer;
        }
    } else if (operation == ",") {
        return second;
    }
    return new RPN_Node;
}