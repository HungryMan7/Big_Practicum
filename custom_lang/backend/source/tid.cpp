#include "../includes/tid.h"

Variable::Variable(std::string name, Type type, std::string value) {
    name_ = name;
    type_ = type;
    value_ = value;
}

Variable::Variable(const Variable& variable) {
    name_ = variable.name_;
    type_ = variable.type_;
    value_ = variable.value_;
}

std::string Variable::GetName() {
    return name_;
}

Function::Function(std::string name, Type return_type, TID* variables) {
    name_ = name;
    return_type_ = return_type;
    variables_ = new TID(*variables);
}

Function::Function(const Function& function) {
    name_ = function.name_;
    return_type_ = function.return_type_;
    variables_ = new TID(*(function.variables_));
}

std::string Function::GetName() {
    return name_;
}

TID::TID(const TID& other) {
    variables_ = other.variables_;
    functions_ = other.functions_;
}

void TID::AddVariable(std::string name, Type type, std::string value) {
    variables_.push_back(new Variable(name, type, value));
}

void TID::AddVariable(const Variable*& variable) {
    variables_.push_back(new Variable(*variable));
}

void TID::AddVariable(const Variable& variable) {
    variables_.push_back(new Variable(variable));
}

void TID::AddFunction(std::string name, Type return_type, TID* variables) {
    functions_.push_back(new Function(name, return_type, variables));
}

void TID::AddFunction(const Function*& function) {
    functions_.push_back(new Function(*function));
}

void TID::AddFunction(const Function& function) {
    functions_.push_back(new Function(function));
}

bool TID::Contains(Variable* variable) {
    for (auto var : variables_) {
        if (var->GetName() == variable->GetName()) {
            return true;
        }
    }
    return false;
}

bool TID::Contains(Function* function) {
    for (auto func : functions_) {
        if (func->GetName() == function->GetName()) {
            return true;
        }
    }
    return false;
}

void TID::Connect(TID* ancestor) {
    ancestor_ = ancestor;
}
