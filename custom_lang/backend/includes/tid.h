#pragma once
#include <vector>
#include <string>
#include <map>

struct TID_Node {
    TID_Node* pred = nullptr, *next = nullptr;
    std::map<std::string, std::vector<std::string>> ID;
};

class TID {
public:
    TID();
    bool CheckAddingID(std::string id_name);
    bool CheckUsingID(std::string id_name);
    void AddID(std::vector<std::string> type, std::string id_name);
    void RemoveTable();
    void AddTable();
    TID_Node* ReturnCurrentTable();
    ~TID();
private:
    TID_Node* root_;
};

