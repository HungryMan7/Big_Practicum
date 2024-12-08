#include "../includes/tid.h"

Cell::~Cell() {
    name_.clear();
    type_.first.clear();
    type_.second.clear();
    value_.clear();
}

void Cell::setName(std::string name) {
    name_ = name;
}

void Cell::setVariant(Variant variant) {
    variant_ = variant;
}

void Cell::setType(std::pair<std::vector<std::string>, std::vector<std::string>> type) {
    type_ = type;
}
    
void Cell::setValue(std::string value) {
    value_ = value;
}

std::string Cell::getName() {
    return name_;
}

Variant Cell::getVariant() {
    return variant_;
}

std::pair<std::vector<std::string>, std::vector<std::string>> Cell::getType() {
    return type_;
}

std::string Cell::getValue() {
    if (variant_ == Variant::Function) {
        throw "function doesn't contain any values";
    }
    return value_;
}

TID_Node::~TID_Node() {
    for (auto id : ID) {
        delete id.second;
    }
}

TID::TID() {
    root_ = new TID_Node;
}

TID::~TID() {
    while (root_) {
        TID_Node* pred_ = root_->pred;
        delete root_;
        root_ = pred_;
    }
}

void TID::AddID(std::pair<std::vector<std::string>, std::vector<std::string>> type, std::string id_name) {
    Cell* new_cell = new Cell;
    new_cell->setName(id_name);
    if (type.first[0] == "function") {
        type.first.erase(type.first.begin());
        new_cell->setVariant(Variant::Function);
    } else {
        new_cell->setVariant(Variant::Variable);
    }
    new_cell->setType(type);
    new_cell->setValue("");
    root_->ID[id_name] = new_cell;
}

bool TID::IsUsed(std::string id_name) {
    TID_Node* temp = root_;
    do {
        for (auto id : temp->ID) {
            if (id.first == id_name) {
                return true;
            }
        }
        temp = temp->pred;
    } while (temp);
    return false;
}

std::pair<std::vector<std::string>, std::vector<std::string>> TID::GetType(std::string id_name) {
    TID_Node* temp = root_;
    std::pair<std::vector<std::string>, std::vector<std::string>> return_value;
    do {
        for (auto id : temp->ID) {
            if (id.first == id_name) {
                return_value = id.second->getType();
                break;
            }
        }
        temp = temp->pred;
    } while (temp);
    return return_value;
}

void TID::ChangeType(std::string id_name, std::pair<std::vector<std::string>, std::vector<std::string>> &new_type) {
    TID_Node* temp = root_;
    do {
        for (auto id : temp->ID) {
            if (id.first == id_name) {
                id.second->setType(new_type);
                break;
            }
        }
        temp = temp->pred;
    } while(temp);
}

void TID::AddTable() {
    TID_Node *add_table = new TID_Node;
    TID_Node* temp = root_;
    root_->next = add_table;
    root_ = root_->next;
    root_->pred = temp;
}

void TID::RemoveTable() {
    TID_Node* temp = root_->pred;
    delete root_;
    root_ = temp;
}

TID_Node* TID::ReturnCurrentTable() {
    return root_;
}
