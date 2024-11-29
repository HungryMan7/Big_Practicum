#include "../includes/tid.h"

TID::TID() {
    root_ = new TID_Node;
}

void TID::AddID(std::pair<std::vector<std::string>, std::vector<std::string>> type, std::string id_name) {
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

std::pair<std::vector<std::string>, std::vector<std::string>> TID::GetType(std::string id_name) {
    TID_Node* temp = root_;
    while (temp->pred != nullptr) {
        for (auto id : temp->ID) {
            if (id.first == id_name) {
                return id.second;
            }
        }
        temp = temp->pred;
    }
    std::pair<std::vector<std::string>, std::vector<std::string>> return_value;
    for (auto id : temp->ID) {
        if (id.first == id_name) {
            return_value = id.second;
            break;
        }
    }
    return return_value;
}

void TID::ChangeType(std::string id_name, std::pair<std::vector<std::string>, std::vector<std::string>> &new_type) {
    while (this->root_->pred != nullptr) {
        for (auto id : this->root_->ID) {
            if (id.first == id_name) {
                id.second = new_type;
            }
        }
        this->root_ = this->root_->pred;
    }
    for (auto id : this->root_->ID) {
        if (id.first == id_name) {
            this->root_->ID[id.first] = new_type;
        }
    }
    while (this->root_->next != nullptr) {
        this->root_ = this->root_->next;
    }
    return;
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
