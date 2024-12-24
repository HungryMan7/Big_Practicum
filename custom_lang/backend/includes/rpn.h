#pragma once
#include <vector>
#include <iostream>
#include <algorithm>

#include "../includes/lexem.h"
#include "../includes/tid.h"

struct RPN_Node {
public:
    RPN_Node(Lexem* lex, TID &table) {
        if (lex->GetType() == LexemType::Integer) {
            value_int = std::stoi(lex->GetValue());
            type_ = "integer";
        } else if (lex->GetType() == LexemType::Float) {
            value_double = std::stod(lex->GetValue());
            type_ = "double";
        } else if (lex->GetValue() == "true" || lex->GetValue() == "false") {
            value_bool = (lex->GetValue() == "true");
            type_ = "bool";
        } else if (lex->GetType() == LexemType::Identifier) {
            id_name = lex->GetValue();
            if (table.getType(id_name).first[0] == "function") {
                type_ = "function";
            } else type_ = "identifier";
        } else {
            operation = lex->GetValue();
            type_ = "other";
        }
        line = lex->GetLine();
        column = lex->GetColumn();
    }
    RPN_Node(std::vector<std::vector<int>> value, std::vector<RPN_Node*> arg) {
        type_ = "array";
        value_array = value;
        array_elements = arg;
    }
    RPN_Node(int number) {
        type_ = "label";
        label_number = number;
        rpn_label = true;
    }
    RPN_Node(bool flag, int number) {
        type_ = "label";
        label_number = number;
        rpn_go_label = true;
    }
    RPN_Node(Lexem* lex, std::string utility) {
        type_ = "utility";
        keyword = lex->GetValue();
    }
    RPN_Node() {
        type_ = "label";
        rpn_empty_label = true;
    }
    RPN_Node(std::string name, std::string type) {
        type_ = type;
        if (type == "other") {
            operation = name;
        } else if (type == "identifier" || type == "function") {
            id_name = name;
        } else if (type == "utility") {
            keyword = name;
        }
    }
    RPN_Node(bool flag) {
        type_ = "label";
        rpn_true_label = true;
    }
    std::string GetType() { return type_; }
    std::string GetName() { return id_name; }
    bool IsEmptyLabel() { return rpn_empty_label; }
    void Print() {
        std::cout << type_;
        if (type_ == "identifier" || type_ == "function") {
            std::cout << " " << id_name;
        } else if (type_ == "label") {
            if (rpn_label) std::cout << label_number << " common ";
            if (rpn_true_label) std::cout << " true ";
            if (rpn_go_label) std::cout << label_number << " go ";
            if (rpn_empty_label) std::cout << " empty ";
        } else if (type_ == "integer") {
            std::cout << " " << value_int;
        } else if (type_ == "double") {
            std::cout << " " << value_double;
        } else if (type_ == "bool") {
            std::cout << " " << value_bool;
        } else if (type_ == "string") {
            std::cout << " " << value_string;        
        } else if (type_ == "utility") {
            std::cout << " " << keyword;
        } else if (type_ == "other") {
            std::cout << " " << operation;
        }
    }

private:
    std::string type_;
    int label_number;
    int value_int;
    double value_double;
    bool value_bool;
    std::string value_string;
    std::string id_name;
    std::string operation;
    std::string keyword;
    int line, column;
    std::vector<std::vector<int>> value_array;
    std::vector<RPN_Node*> array_elements;
    bool rpn_label, rpn_true_label, rpn_go_label, rpn_empty_label = false;
};

class RPN_TID_Node {
public:
    virtual ~RPN_TID_Node();
    RPN_TID_Node* pred = nullptr, *next = nullptr;
    std::map<std::string, RPN_Node*> ID;
};

class RPN_TID {
public:
    RPN_TID();
    virtual ~RPN_TID();
    void AddTable();
    void RemoveTable();
    void AddID(std::string id_name, RPN_Node* value);
    void setValue(std::string id_name, RPN_Node* value);
    RPN_Node* getValue(std::string id_name);

private:
    RPN_TID_Node* root_;
};

class RPN {
public:
    RPN() { chain_ = {}; }
    ~RPN() { chain_ = {}; }
    void Analyze(const std::vector<Lexem*>& list_of_lexems);
    std::vector<RPN_Node*> GetChain() { return chain_; }
    void AddCell(Lexem* element, std::vector<RPN_Node*> &container) { 
        RPN_Node* new_cell = new RPN_Node(element, table_id);
        if (container.size() == 0) chain_.push_back(new_cell); 
        else {
            container.push_back(new_cell);
        }
    }
    void AddCell(std::vector<RPN_Node*> arg, std::vector<std::vector<int>> value, std::vector<RPN_Node*> &container) { 
        RPN_Node* new_cell = new RPN_Node(value, arg);
        if (container.size() == 0) chain_.push_back(new_cell); 
        else {
            container.push_back(new_cell);
        }
    }
    void AddCell(bool flag, std::vector<RPN_Node*> &container) { 
        RPN_Node* new_cell = new RPN_Node(flag);
        if (container.size() == 0) chain_.push_back(new_cell); 
        else {
            container.push_back(new_cell);
        }
    }
    void AddCell(int number, std::vector<RPN_Node*> &container) { 
        RPN_Node* new_cell = new RPN_Node(number);
        if (container.size() == 0) chain_.push_back(new_cell); 
        else {
            container.push_back(new_cell);
        }
    }
    void AddCell(Lexem* lex, std::string utility, std::vector<RPN_Node*> &container) { 
        RPN_Node* new_cell = new RPN_Node(lex, utility);
        if (container.size() == 0) chain_.push_back(new_cell); 
        else {
            container.push_back(new_cell);
        }
    }
    void AddCell(bool flag, int number, std::vector<RPN_Node*> &container) { 
        RPN_Node* new_cell = new RPN_Node(flag, number);
        if (container.size() == 0) chain_.push_back(new_cell); 
        else {
            container.push_back(new_cell);
        }
    }
    void AddCell(std::vector<RPN_Node*> &container) { 
        RPN_Node* new_cell = new RPN_Node();
        if (container.size() == 0) chain_.push_back(new_cell); 
        else {
            container.push_back(new_cell);
        }
    }
    void AddCell(RPN_Node* new_cell, std::vector<RPN_Node*> &container) {
        if (container.size() == 0) chain_.push_back(new_cell); 
        else {
            container.push_back(new_cell);
        }
    }
    void setTID(TID table) { table_id = table; }

private:
    void GetLex();
    void PROGRAM();
    void VARS_MDEF();
    void FUNC(std::string id_name);
    void BODY(std::string func_name);
    void FUNC_BODY(std::string func_name);
    void STATEMENT(std::string func_name);
    void FUNC_STATEMENT(bool &check, std::string func_name);
    void FUNC_CALL(std::string id_name);
    void IF(std::string func_name);
    void LOOP(std::string func_name);
    void LOOP_WHILE(std::string func_name);
    void LOOP_FOR(std::string func_name);
    void LOOP_FOREACH(std::string func_name);
    void SWITCH(std::string func_name);
    void ID(std::vector<RPN_Node*> &container);
    void TERM(std::vector<RPN_Node*> &container);
    void INPUT();
    void OUTPUT();
    void EXP_ZERO(std::vector<RPN_Node*> &container);
    void EXP_ONE(std::vector<RPN_Node*> &container);
    void EXP_TWO(std::vector<RPN_Node*> &container);
    void EXP_THREE(std::vector<RPN_Node*> &container);
    void EXP_FOUR(std::vector<RPN_Node*> &container);
    void EXP_FIVE(std::vector<RPN_Node*> &container);
    void EXP_SIX(std::vector<RPN_Node*> &container);
    void EXP_SEVEN(std::vector<RPN_Node*> &container);
    void EXP_EIGHT(std::vector<RPN_Node*> &container);
    void EXP_NINE(std::vector<RPN_Node*> &container);
    void EXP_TEN(std::vector<RPN_Node*> &container);
    void EXP_ELEVEN(std::vector<RPN_Node*> &container);
    void EXP_TWELVE(std::vector<RPN_Node*> &container);
    void EXP_THIRTEEN(std::vector<RPN_Node*> &container);
    void EXP_FOURTEEN(std::vector<RPN_Node*> &container);
    void FindFunction(std::string id_name);
    Lexem* lexem_;
    TID table_id;
    RPN_TID value_table;
    int lex_index_ = 0;
    int label_number_ = 0;
    std::vector<Lexem*> list_of_lexems_;
    std::vector<RPN_Node*> chain_;
};
