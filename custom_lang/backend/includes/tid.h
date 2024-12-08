#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>

enum class Variant { Variable, Function };

class Cell {
public:
    void setName(std::string);
    void setVariant(Variant);
    void setType(std::pair<std::vector<std::string>, std::vector<std::string>>);
    void setValue(std::string);
    std::string getName();
    Variant getVariant();
    std::pair<std::vector<std::string>, std::vector<std::string>> getType();
    std::string getValue();

private:
    std::string name_;
    Variant variant_;
    std::pair<std::vector<std::string>, std::vector<std::string>> type_;
    std::string value_;
};

class TID_Node {
public:
    virtual ~TID_Node();
    TID_Node* pred = nullptr, *next = nullptr;
    std::map<std::string, Cell*> ID;
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
