#include "../includes/interpreter.h"

void Interpreter::Run(std::vector<RPN_Node*> rpn) {
    rpn_ = rpn;
    tid_ = new RPN_TID;
    int ind = 0;
    while (ind < rpn_.size() && rpn_[ind]->GetType() != "function" ||
           rpn_[ind]->GetName() != "main") {
        ++ind;
    }
    if (ind == rpn_.size()) {
        throw "function or label not found!";
    }
    ++ind;
    for (; ind < rpn_.size(); ++ind) {
        if (rpn_[ind]->GetType() == "utility") {
            if (rpn_[ind]->GetKeyword() == "return") {
                RPN_Node* res = stack.top();
                stack.pop();
                if (res->GetType() == "identifier") {
                    std::string nnnnnnn = res->GetName(); 
                    delete res;
                    res = tid_->getValue(nnnnnnn);
                }
                stack.push(res);
            }
        } else if (rpn_[ind]->GetType() == "function") {
            std::string zzz = rpn_[ind]->GetName();
            ind = 0;
            while (rpn_[ind]->GetType() != "function" || rpn_[ind]->GetName() != zzz) {
                ++ind;
            }
        } else if (rpn_[ind]->GetType() == "other") {
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
                    ind = 0;
                    while (!rpn_[ind]->IsFalseLabel() || rpn_[ind]->GetFalseLabelNumber() != target) {
                        ++ind;
                    }
                    --ind;
                }
                delete condition;
            } else if (rpn_[ind]->IsGoLabel()) {
                int target = rpn_[ind]->GetLabelNumber();
                ind = 0;
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
}

RPN_Node* Interpreter::Solve(RPN_Node* first, RPN_Node* second, std::string operation) {
    if (first->GetType() == "identifier") {
        std::string name = first->GetName();
        delete first;
        first = tid_->getValue(name);
    }
    if (second->GetType() == "identifier") {
        std::string name = second->GetName();
        delete second;
        second = tid_->getValue(name);
    }
    if (operation == "=") {
        if (second->GetType() == "integer") {
            first->SetIntegerValue(second->GetIntegerValue());
        } else if (second->GetType() == "double") {
            first->SetDoubleValue(second->GetDoubleValue());
        } else if (second->GetType() == "bool") {
            first->SetBoolValue(second->GetBoolValue());
        } else if (second->GetType() == "string") {
            first->SetStringValue(second->GetStringValue());
        } else if (second->GetType() == "array") {
            first->SetElements(second->GetElements());
            first->SetSizes(second->GetSizesValue());
        }
        first->SetType(second->GetType());
        return new RPN_Node(*first);
    } else if (operation == "+=") {
        return Solve(first, Solve(first, second, "+"), "=");
    } else if (operation == "-=") {
        return Solve(first, Solve(first, second, "-"), "=");
    } else if (operation == "*=") {
        return Solve(first, Solve(first, second, "*"), "=");
    } else if (operation == "/=") {
        return Solve(first, Solve(first, second, "/"), "=");
    } else if (operation == "%=") {
        return Solve(first, Solve(first, second, "%"), "=");
    } else if (operation == "<<=") {
        return Solve(first, Solve(first, second, "<<"), "=");
    } else if (operation == ">>=") {
        return Solve(first, Solve(first, second, ">>"), "=");
    } else if (operation == "&=") {
        return Solve(first, Solve(first, second, "&"), "=");
    } else if (operation == "|=") {
        return Solve(first, Solve(first, second, "|"), "=");
    } else if (operation == "^=") {
        return Solve(first, Solve(first, second, "^"), "=");
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
        }
    } else if (operation == "!=") {
        RPN_Node* result = new RPN_Node("", "bool");
        RPN_Node* equal = Solve(first, second, "==");
        result->SetBoolValue(!equal->GetBoolValue());
        return result;
    } else if (operation == ">") {
        RPN_Node* result = new RPN_Node("", "bool");
        if (second->GetType() == "integer") {
            std::cout << first->GetIntegerValue() << " > " << second->GetIntegerValue() << "\n";
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
            return new RPN_Node(std::to_string(first->GetIntegerValue() + second->GetIntegerValue()), "integer");
        } else if (second->GetType() == "double") {
            return new RPN_Node(std::to_string(first->GetDoubleValue() + second->GetDoubleValue()), "double");
        } else if (second->GetType() == "string") {
            return new RPN_Node(first->GetStringValue() + second->GetStringValue(), "string");
        }
    } else if (operation == "-") {
        if (second->GetType() == "integer") {
            return new RPN_Node(std::to_string(first->GetIntegerValue() - second->GetIntegerValue()), "integer");
        } else if (second->GetType() == "double") {
            return new RPN_Node(std::to_string(first->GetDoubleValue() - second->GetDoubleValue()), "double");
        }
    } else if (operation == "*") {
        if (second->GetType() == "integer") {
            return new RPN_Node(std::to_string(first->GetIntegerValue() * second->GetIntegerValue()), "integer");
        } else if (second->GetType() == "double") {
            return new RPN_Node(std::to_string(first->GetDoubleValue() * second->GetDoubleValue()), "double");
        }
    } else if (operation == "/") {
        if (second->GetType() == "integer") {
            return new RPN_Node(std::to_string(first->GetIntegerValue() / second->GetIntegerValue()), "integer");
        } else if (second->GetType() == "double") {
            return new RPN_Node(std::to_string(first->GetDoubleValue() / second->GetDoubleValue()), "double");
        }
    } else if (operation == "%") {
        return new RPN_Node(std::to_string(first->GetIntegerValue() % second->GetIntegerValue()), "integer");
    } else if (operation == "<<") {
        return new RPN_Node(std::to_string(first->GetIntegerValue() << second->GetIntegerValue()), "integer");
    } else if (operation == ">>") {
        return new RPN_Node(std::to_string(first->GetIntegerValue() >> second->GetIntegerValue()), "integer");
    } else if (operation == "&") {
        return new RPN_Node(std::to_string(first->GetIntegerValue() & second->GetIntegerValue()), "integer");
    } else if (operation == "|") {
        return new RPN_Node(std::to_string(first->GetIntegerValue() | second->GetIntegerValue()), "integer");
    } else if (operation == "^") {
        return new RPN_Node(std::to_string(first->GetIntegerValue() ^ second->GetIntegerValue()), "integer");
    } else if (operation == "||") {
        return new RPN_Node(std::to_string(first->GetBoolValue() || second->GetBoolValue()), "bool");
    } else if (operation == "&&") {
        return new RPN_Node(std::to_string(first->GetBoolValue() && second->GetBoolValue()), "bool");
    } else if (operation == ",") {
        return second;
    }
    return new RPN_Node;
}