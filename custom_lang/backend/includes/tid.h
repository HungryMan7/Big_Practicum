#pragma once
#include <vector>
#include <string>
#include <map>

struct Variable {
    Variable(std::string name, std::pair<std::vector<std::string>, std::vector<std::string>> type):
    name_(name), type_(type), value_("") {}
    Variable(std::string name, std::pair<std::vector<std::string>, std::vector<std::string>> type, 
    std::string value): name_(name), type_(type), value_(value) {}
    std::string name_;
    std::pair<std::vector<std::string>, std::vector<std::string>> type_;
    std::string value_;
};

struct Function {
    Function(std::string name, std::pair<std::vector<std::string>, std::vector<std::string>> type):
    name_(name), type_(type) {}
    std::string name_;
    std::pair<std::vector<std::string>, std::vector<std::string>> type_;
};

struct TID_Node {
    TID_Node* pred = nullptr, *next = nullptr;
    std::map<std::string, Variable*> VAR;
    std::map<std::string, Function*> FUNC;
    std::map<std::string, std::pair<std::vector<std::string>, std::vector<std::string>>> ID;
};

class TID {
public:
    TID();
    virtual ~TID();
    bool IsUsed(std::string id_name);
    void AddID(std::pair<std::vector<std::string>, std::vector<std::string>> type, std::string id_name);
    std::pair<std::vector<std::string>, std::vector<std::string>> GetType(std::string id_name);
    void ChangeType(std::string id_name, std::pair<std::vector<std::string>, std::vector<std::string>> &new_type);
    void RemoveTable();
    void AddTable();
    TID_Node* ReturnCurrentTable();

private:
    TID_Node* root_;
};
