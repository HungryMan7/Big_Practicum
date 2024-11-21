#include "tid.h"

TID::TID() {
    root_ = new TID_Node;
}

void TID::AddID(std::vector<std::string> type, std::string id_name) {
    root_->ID[id_name] = type;
}

bool TID::CheckAddingID(std::string id_name) {
    TID_Node* temp = root_;
    while (temp->pred != nullptr) {
        for (auto id : temp->ID) {
            if (id.first == id_name) {
                return false;
            }
        }
        temp = temp->pred;
    }
    for (auto id : temp->ID) {
        if (id.first == id_name) {
            return false;
        }
    }
    return true;
}

bool TID::CheckUsingID(std::string id_name) {
    TID_Node* temp = root_;
    while (temp->pred != nullptr) {
        for (auto id : temp->ID) {
            if (id.first == id_name) {
                return true;
            }
        }
        temp = temp->pred;
    }
    for (auto id : temp->ID) {
        if (id.first == id_name) {
            return true;
        }
    }
    return false;
}

void TID::AddTable() {
    TID_Node *add_table = new TID_Node;
    TID_Node* temp = root_;
    root_->next = add_table;
    root_ = root_->next;
    root_->pred = temp;
}

void TID::RemoveTable() {
    TID_Node* temp = root_;
    temp = temp->pred;
    delete root_;
    root_ = temp;
}

TID_Node* TID::ReturnCurrentTable() {
    return root_;
}

TID::~TID() {
    delete root_;
}
