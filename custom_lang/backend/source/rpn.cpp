#include "../includes/rpn.h"
#include <fstream>

std::vector<RPN_Node*> empty = {};
std::vector<RPN_Node*> while_loop = {new RPN_Node("while", "utility")};
std::vector<RPN_Node*> for_loop = {new RPN_Node("for", "utility")};

std::vector<int> first_array, last_array;
int first_common_label_number;
int last_common_label_number;

RPN_Node::RPN_Node(const RPN_Node& other) {
    type_ = other.type_;
    label_number = other.label_number;
    value_int = other.value_int;
    value_double = other.value_double;
    value_bool = other.value_bool;
    value_string = other.value_string;
    id_name = other.id_name;
    operation = other.operation;
    keyword = other.keyword;
    line = other.line; 
    column = other.column;
    value_array = other.value_array;
    for (auto node : other.array_elements) {
        array_elements.push_back(node);
    }
    rpn_label = other.rpn_label;
    rpn_false_label = other.rpn_false_label;
    rpn_go_label = other.rpn_go_label;
    rpn_empty_label = other.rpn_empty_label;
}

void RPN::FindFunction(std::string id_name) {
    int counter = 0;
    for (auto elem : chain_) {
        if (elem->GetType() == "function") {
            if (elem->GetName() == id_name) {
                bool flag = false;
                for (auto label_elem : chain_) {
                    if (label_elem != elem && !flag) {
                        continue;
                    } else flag = true;
                    if (label_elem->IsEmptyLabel()) {
                        chain_.insert(chain_.begin() + counter, new RPN_Node(true, label_number_));
                        label_number_++;
                        break;
                    } else counter++;
                }
                break;
            } else counter++;
        } else counter++;
    }
}

void RPN::Analyze(const std::vector<Lexem*>& list_of_lexems) {
    list_of_lexems_ = list_of_lexems;
    GetLex();
    PROGRAM();
}

void RPN::GetLex() {
    if (lex_index_ < (int)list_of_lexems_.size()) {
        lexem_ = list_of_lexems_[lex_index_];
        ++lex_index_;
    }
    else {
        ++lex_index_;
    }
}

void RPN::PROGRAM() {
    int id_line_, id_column_;
    std::pair<std::vector<std::string>, std::pair<std::vector<std::string>, std::vector<std::string>>> current_type_;
    std::string current_id_name_;
    while (true) {
        if (lexem_->GetValue() == "array") {
            current_type_ = { { "array" }, {} };
            GetLex();
            int counter = 0;
            while (lexem_->GetValue() == "<") {
                counter++;
                GetLex(); // <
                if (lexem_->GetValue() == "array") {
                    GetLex(); // array
                    current_type_.first.push_back("array");
                }
                else {
                    current_type_.first.push_back(lexem_->GetValue());
                    GetLex(); // type
                    break;
                }
            }
            while (counter) {
                if (lexem_->GetValue() == ">>") counter -= 2;
                else counter--;
                GetLex();
            }
        }
        else {
            current_type_ = { { lexem_->GetValue() }, {} };
            GetLex(); // type
        }
        if (lexem_->GetValue() == "main" && list_of_lexems_[lex_index_ - 2]->GetValue() == "int") break;
        current_id_name_ = { lexem_->GetValue() };
        id_line_ = lexem_->GetLine();
        id_column_ = lexem_->GetColumn();
        GetLex(); // id
        if (lexem_->GetValue() == "(") {
            GetLex();
            if (!table_id.IsUsed(current_id_name_)) {
                current_type_.first.insert(current_type_.first.begin(), "function");
                table_id.AddID(current_type_, current_id_name_, id_line_, id_column_);
            }
            AddCell(new RPN_Node(current_id_name_, "function"), empty);
            table_id.AddTable();
            FUNC(current_id_name_);
            table_id.RemoveTable();
            AddCell(empty);
        }
        else {
            if (!table_id.IsUsed(current_id_name_)) {
                table_id.AddID( current_type_ , current_id_name_, id_line_, id_column_);
            }
            AddCell(new RPN_Node(current_id_name_, "identifier"), empty);
            VARS_MDEF();
            GetLex();
        }
    }
    current_id_name_ = lexem_->GetValue();
    id_line_ = lexem_->GetLine();
    id_column_ = lexem_->GetColumn();
    if (!table_id.IsUsed(current_id_name_)) {
        current_type_.first.insert(current_type_.first.begin(), "function");
        table_id.AddID(current_type_, current_id_name_, id_line_, id_column_);
    }
    GetLex(); // main
    GetLex(); // (
    GetLex(); // )
    AddCell(new RPN_Node(current_id_name_, "function"), empty);
    table_id.AddTable();
    FUNC_BODY("main");
    table_id.RemoveTable();
    AddCell(empty);
}

void RPN::VARS_MDEF() {
    int id_line_, id_column_;
    std::pair<std::vector<std::string>, std::pair<std::vector<std::string>, std::vector<std::string>>> current_type_;
    std::string current_id_name_;
    if (lexem_->GetValue() == ";") {
        current_id_name_ = list_of_lexems_[lex_index_ - 2]->GetValue();
        if (table_id.getType(current_id_name_).first[0] == "int") {
            AddCell(new RPN_Node("0", "integer"), empty);
        } else if (table_id.getType(current_id_name_).first[0] == "double") {
            AddCell(new RPN_Node("0.0", "double"), empty);
        } else if (table_id.getType(current_id_name_).first[0] == "string") {
            AddCell(new RPN_Node("", "string"), empty);
        } else if (table_id.getType(current_id_name_).first[0] == "array") {
            AddCell({new RPN_Node("0", "int")}, {{1}}, empty);
        } else {
            AddCell(new RPN_Node("true", "bool"), empty);
        }
        AddCell(new RPN_Node("=", "other"), empty);
        return;
    }
    if (lexem_->GetValue() == "=") {
        GetLex();
        EXP_ONE(empty);
        RPN_Node* equality = new RPN_Node("=", "other");
        AddCell(equality, empty);
    }
    while (lexem_->GetValue() == ",") {
        GetLex();
        current_id_name_ = lexem_->GetValue();
        id_line_ = lexem_->GetLine();
        id_column_ = lexem_->GetColumn();
        if (!table_id.IsUsed(current_id_name_)) {
            table_id.AddID({ current_type_ }, current_id_name_, id_line_, id_column_);
        }
        AddCell(lexem_, empty);
        GetLex(); // id
        std::vector<std::string> type;
        if (lexem_->GetValue() == "=") {
            GetLex();
            EXP_ONE(empty);
            RPN_Node* equality = new RPN_Node("=", "other");
            AddCell(equality, empty);
        }
    }
}

void RPN::FUNC(std::string id_name) {
    int id_line_, id_column_;
    std::pair<std::vector<std::string>, std::pair<std::vector<std::string>, std::vector<std::string>>> current_type_;
    std::string current_id_name_;
    if (lexem_->GetValue() == ")") {
        GetLex(); // )
    }
    else if (lexem_->GetType() == LexemType::Type) {
        if (lexem_->GetValue() == "array") {
            current_type_ = { { "array" }, {} };
            GetLex();
            int counter = 0;
            while (lexem_->GetValue() == "<") {
                counter++;
                GetLex(); // <
                if (lexem_->GetValue() == "array") {
                    GetLex(); // array
                    current_type_.first.push_back("array");
                }
                else {
                    current_type_.first.push_back(lexem_->GetValue());
                    GetLex(); // type
                    break;
                }
            }
            while (counter) {
                if (lexem_->GetValue() == ">>") counter -= 2;
                else counter--;
                GetLex();
            }
        }
        else {
            current_type_ = { { lexem_->GetValue() }, {} };
            GetLex(); // type
        }
        current_id_name_ = lexem_->GetValue();
        id_line_ = lexem_->GetLine();
        id_column_ = lexem_->GetColumn();
        if (!table_id.IsUsed(current_id_name_)) {
            std::pair<std::vector<std::string>, std::pair<std::vector<std::string>, std::vector<std::string>>> new_type = table_id.getType(id_name);
            for (int i = 0; i < (int)current_type_.first.size(); ++i) {
                new_type.second.first.push_back(current_type_.first[i]);
            }
            new_type.second.second.push_back(current_id_name_);
            table_id.setType(id_name, new_type);
            table_id.AddID(current_type_, current_id_name_, id_line_, id_column_);
        }
        GetLex(); // id
        while (lexem_->GetValue() == ",") {
            GetLex();
            if (lexem_->GetValue() == "array") {
                current_type_ = { { "array" }, {} };
                GetLex();
                int counter = 0;
                while (lexem_->GetValue() == "<") {
                    counter++;
                    GetLex(); // <
                    if (lexem_->GetValue() == "array") {
                        GetLex(); // array
                        current_type_.first.push_back("array");
                    }
                    else {
                        current_type_.first.push_back(lexem_->GetValue());
                        GetLex(); // type
                        break;
                    }
                }
                while (counter) {
                    if (lexem_->GetValue() == ">>") counter -= 2;
                    else counter--;
                    GetLex();
                }
            }
            else {
                current_type_ = { { lexem_->GetValue() }, {} };
                GetLex(); // type
            }
            current_id_name_ = lexem_->GetValue();
            id_line_ = lexem_->GetLine();
            id_column_ = lexem_->GetColumn();
            if (!table_id.IsUsed(current_id_name_)) {
                std::pair<std::vector<std::string>, std::pair<std::vector<std::string>, std::vector<std::string>>> new_type = table_id.getType(id_name);
                for (int i = 0; i < (int)current_type_.first.size(); ++i) {
                    new_type.second.first.push_back(current_type_.first[i]);
                }
                new_type.second.second.push_back(current_id_name_);
                table_id.setType(id_name, new_type);
                table_id.AddID(current_type_, current_id_name_, id_line_, id_column_);
            }
            GetLex(); // id;
        }
        GetLex(); // )
    }
    else if (lexem_->GetValue() == ")") {
        GetLex();
    }
    if (lexem_->GetValue() == ";") {
        GetLex();
    }
    else if (lexem_->GetValue() == "{") {
        FUNC_BODY(id_name);
    }
}

void RPN::BODY(std::string func_name) {
    GetLex(); // {
    bool flag = false;
    RPN_Node* last_elem;
    RPN_Node* pred_last_elem;
    if (while_loop.size() > 1) {
        first_common_label_number = label_number_;
        last_common_label_number = label_number_ + 1;
        first_array.push_back(first_common_label_number);
        last_array.push_back(last_common_label_number);
        label_number_ += 2;
        for (int i = 1; i < while_loop.size() - 2; ++i) {
            chain_.push_back(while_loop[i]);
        }
        flag = true;
        last_elem = while_loop[while_loop.size() - 1];
        pred_last_elem = while_loop[while_loop.size() - 2];
        while_loop = {new RPN_Node("while", "utility")};
    }
    while (lexem_->GetValue() != "}") {
        STATEMENT(func_name);
    }
    if (flag) chain_.push_back(last_elem);
    first_array.pop_back();
    last_array.pop_back();
    GetLex(); // }
}

void RPN::FUNC_BODY(std::string func_name) {
    GetLex(); // {
    bool flag = false;
    RPN_Node* pred_last_elem;
    RPN_Node* last_elem;
    if (while_loop.size() > 1) {
        first_common_label_number = label_number_;
        last_common_label_number = label_number_ + 1;
        label_number_ += 2;
        for (int i = 1; i < while_loop.size() - 2; ++i) {
            chain_.push_back(while_loop[i]);
        }
        flag = true;
        last_elem = while_loop[while_loop.size() - 1];
        pred_last_elem = while_loop[while_loop.size() - 2];
        while_loop = {new RPN_Node("while", "utility")};
    }
    bool checker = false;
    while (lexem_->GetValue() != "}") {
        FUNC_STATEMENT(checker, func_name);
    }
    if (flag) chain_.push_back(last_elem);
    GetLex(); // }
}

void RPN::STATEMENT(std::string func_name) {
    int first_label_number = label_number_;
    int last_label_number = label_number_ + 1;
    label_number_ += 2;
    AddCell(new RPN_Node(first_label_number), empty);
    int id_line_, id_column_;
    std::pair<std::vector<std::string>, std::pair<std::vector<std::string>, std::vector<std::string>>> current_type_;
    std::string current_id_name_;
    if (lexem_->GetType() == LexemType::Type) {
        if (lexem_->GetValue() == "array") {
            current_type_ = { { "array" }, {} };
            GetLex();
            int counter = 0;
            while (lexem_->GetValue() == "<") {
                counter++;
                GetLex(); // <
                if (lexem_->GetValue() == "array") {
                    GetLex(); // array
                    current_type_.first.push_back("array");
                }
                else {
                    current_type_.first.push_back(lexem_->GetValue());
                    GetLex(); // type
                    break;
                }
            }
            while (counter) {
                if (lexem_->GetValue() == ">>") counter -= 2;
                else counter--;
                GetLex();
            }
        }
        else {
            current_type_ = { { lexem_->GetValue() }, {} };
            GetLex(); // type
        }
        current_id_name_ = lexem_->GetValue();
        id_line_ = lexem_->GetLine();
        id_column_ = lexem_->GetColumn();
        if (!table_id.IsUsed(current_id_name_)) {
            table_id.AddID({ current_type_ }, current_id_name_, id_line_, id_column_);
        }
        AddCell(new RPN_Node(current_id_name_, "identifier"), empty);
        GetLex(); // id
        VARS_MDEF();
        GetLex();
    }
    else if (lexem_->GetType() == LexemType::Identifier) {
        current_id_name_ = lexem_->GetValue();
        if (!table_id.IsUsed(current_id_name_)) {
            table_id.AddID({ current_type_ }, current_id_name_, id_line_, id_column_);
        }
        if (lex_index_ < (int)list_of_lexems_.size() && (list_of_lexems_[lex_index_]->GetValue() == "=")) {
            AddCell(new RPN_Node(current_id_name_, "identifier"), empty);
            GetLex();
            VARS_MDEF();
        }
        else {
            do {
                if (lexem_->GetValue() == ",") {
                    GetLex();
                }
                EXP_ONE(empty);
            } while (lexem_->GetValue() == ",");
        }
        GetLex(); // ;
    }
    else if (lexem_->GetValue() == "if") {
        GetLex();
        IF(func_name);
    }
    else if (lexem_->GetValue() == "switch") {
        GetLex();
        SWITCH(func_name);
    }
    else if (lexem_->GetValue() == "loop") {
        GetLex();
        LOOP(func_name);
    }
    else if (lexem_->GetValue() == "break") {
        AddCell(new RPN_Node(true, last_array[last_array.size() - 1]), empty);
        GetLex(); // break
        GetLex(); // ;
    }
    else if (lexem_->GetValue() == "continue") {
        AddCell(new RPN_Node(true, first_array[first_array.size() - 1]), empty);
        GetLex(); // continue
        GetLex(); // ;
    }
    else if (lexem_->GetValue() == "cout") {
        GetLex();
        OUTPUT();
        GetLex();
    }
    else if (lexem_->GetValue() == "cin") {
        GetLex();
        INPUT();
        GetLex();
    }
    else if (lexem_->GetValue() == "return") {
        GetLex();
        if (lexem_->GetValue() == ";") {
            GetLex();
        }
        else {
            do {
                if (lexem_->GetValue() == ",") {
                    GetLex();
                }
                EXP_ONE(empty);
            } while (lexem_->GetValue() == ",");
            GetLex(); // ;
        }
        AddCell(new RPN_Node("return", "utility"), empty);
    }
    AddCell(new RPN_Node(last_label_number), empty);
}

void RPN::FUNC_STATEMENT(bool &check, std::string func_name) {
    int first_label_number = label_number_;
    int last_label_number = label_number_ + 1;
    label_number_ += 2;
    AddCell(new RPN_Node(first_label_number), empty);
    int id_line_, id_column_;
    std::pair<std::vector<std::string>, std::pair<std::vector<std::string>, std::vector<std::string>>> current_type_;
    std::string current_id_name_;
    if (lexem_->GetType() == LexemType::Type) {
        if (lexem_->GetValue() == "array") {
            current_type_ = { { "array" }, {} };
            GetLex();
            int counter = 0;
            while (lexem_->GetValue() == "<") {
                counter++;
                GetLex(); // <
                if (lexem_->GetValue() == "array") {
                    GetLex(); // array
                    current_type_.first.push_back("array");
                }
                else {
                    current_type_.first.push_back(lexem_->GetValue());
                    GetLex(); // type
                    break;
                }
            }
            while (counter) {
                if (lexem_->GetValue() == ">>") counter -= 2;
                else counter--;
                GetLex();
            }
        }
        else {
            current_type_ = { { lexem_->GetValue() }, {} };
            GetLex(); // type
        }
        current_id_name_ = lexem_->GetValue();
        id_line_ = lexem_->GetLine();
        id_column_ = lexem_->GetColumn();
        if (!table_id.IsUsed(current_id_name_)) {
            table_id.AddID({ current_type_ }, current_id_name_, id_line_, id_column_);
        }
        AddCell(new RPN_Node(current_id_name_, "identifier"), empty);
        GetLex(); // id
        VARS_MDEF();
        GetLex();
    }
    else if (lexem_->GetType() == LexemType::Identifier) {
        current_id_name_ = lexem_->GetValue();
        if (!table_id.IsUsed(current_id_name_)) {
            table_id.AddID({ current_type_ }, current_id_name_, id_line_, id_column_);
        }
        if (lex_index_ < (int)list_of_lexems_.size() && (list_of_lexems_[lex_index_]->GetValue() == "=")) {
            AddCell(new RPN_Node(current_id_name_, "identifier"), empty);
            GetLex();
            VARS_MDEF();
        }
        else {
            do {
                if (lexem_->GetValue() == ",") {
                    GetLex();
                }
                EXP_ONE(empty);
            } while (lexem_->GetValue() == ",");
        }
        GetLex(); // ;
    }
    else if (lexem_->GetValue() == "if") {
        GetLex();
        IF(func_name);
    }
    else if (lexem_->GetValue() == "switch") {
        GetLex();
        SWITCH(func_name);
    }
    else if (lexem_->GetValue() == "loop") {
        GetLex();
        LOOP(func_name);
    }
    else if (lexem_->GetValue() == "break") {
        AddCell(new RPN_Node(true, last_array[last_array.size() - 1]), empty);
        GetLex(); // break
        GetLex(); // ;
    }
    else if (lexem_->GetValue() == "continue") {
        AddCell(new RPN_Node(true, first_array[first_array.size() - 1]), empty);
        GetLex(); // continue
        GetLex(); // ;
    }
    else if (lexem_->GetValue() == "cout") {
        GetLex();
        OUTPUT();
        GetLex();
    }
    else if (lexem_->GetValue() == "cin") {
        GetLex();
        INPUT();
        GetLex();
    }
    else if (lexem_->GetValue() == "return") {
        check = true;
        GetLex();
        if (lexem_->GetValue() == ";") {
            GetLex();
        }
        else {
            do {
                if (lexem_->GetValue() == ",") {
                    GetLex();
                }
                EXP_ONE(empty);
            } while (lexem_->GetValue() == ",");
            GetLex(); // ;
        }
        AddCell(new RPN_Node("return", "utility"), empty);
    }
    AddCell(new RPN_Node(last_label_number), empty);
}

void RPN::INPUT() {
    GetLex(); // >>
    RPN_Node* id = new RPN_Node(lexem_, table_id);
    AddCell(id, empty);
    GetLex(); // ID;
    while (lexem_->GetValue() == ">>") {
        GetLex(); // >>
        id = new RPN_Node(lexem_, table_id);
        AddCell(id, empty);
        GetLex(); // ID
    }
    RPN_Node* input = new RPN_Node("cin", "utility");
    AddCell(input, empty);
}

void RPN::OUTPUT() {
    GetLex(); // <<
    EXP_FOURTEEN(empty);
    while (lexem_->GetValue() == "<<") {
        GetLex(); // <<
        EXP_FOURTEEN(empty);
    }
    RPN_Node* output = new RPN_Node("cout", "utility");
    AddCell(output, empty);
}

void RPN::FUNC_CALL(std::string id_name) {
    int index = 0;
    GetLex(); // (
    bool checker = false;
    while (lexem_->GetType() == LexemType::Identifier || lexem_->GetType() == LexemType::String ||
        lexem_->GetType() == LexemType::Integer || lexem_->GetType() == LexemType::Float || lexem_->GetValue() == "{" || lexem_->GetValue() == "(") {
        checker = true;
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            RPN_Node* param = new RPN_Node(table_id.getType(id_name).second.second[index], "identifier");
            AddCell(param, empty);
            EXP_ONE(empty);
            RPN_Node* sign = new RPN_Node("=", "other");
            AddCell(sign, empty);
            index++;
        } while (lexem_->GetValue() == ",");
        GetLex(); // )
    }
    if (!checker) GetLex(); // )
    RPN_Node* param = new RPN_Node(id_name, "function");
    AddCell(param, empty);
    RPN_Node* label = new RPN_Node(label_number_);
    AddCell(label, empty);
    FindFunction(id_name);
}

void RPN::IF(std::string func_name) {
    GetLex(); // (
    EXP_ZERO(empty);
    GetLex(); // )
    int final_label = label_number_;
    label_number_++;
    int false_label_number = false_number;
    AddCell(new RPN_Node(false, true, false_label_number), empty);
    false_number++;
    table_id.AddTable();
    BODY(func_name);
    table_id.RemoveTable();
    AddCell(new RPN_Node(true, final_label), empty);
    AddCell(new RPN_Node(false_label_number, false), empty);
    while (lexem_->GetValue() == "elif") {
        GetLex(); // elif
        GetLex(); // (
        EXP_ZERO(empty);
        GetLex(); // )
        false_label_number = false_number;
        AddCell(new RPN_Node(false, true, false_label_number), empty);
        false_number++;
        table_id.AddTable();
        BODY(func_name);
        table_id.RemoveTable();
        AddCell(new RPN_Node(true, final_label), empty);
        AddCell(new RPN_Node(false_label_number, false), empty);
    }
    if (lexem_->GetValue() == "else") {
        GetLex(); // else
        table_id.AddTable();
        BODY(func_name);
        table_id.RemoveTable();
    }
    AddCell(new RPN_Node(final_label), empty);
}

void RPN::SWITCH(std::string func_name) {
    std::vector<int> false_markers = {};
    int counter = 0;
    GetLex(); // (
    EXP_ZERO(empty);
    GetLex(); // )
    GetLex(); // {
    while (lexem_->GetValue() == "case") {
        GetLex();
        EXP_ZERO(empty);
        GetLex(); // :
        RPN_Node* equality = new RPN_Node("==", "other");
        AddCell(equality, empty);
        int false_label_number = label_number_;
        AddCell(true, false_label_number, empty);
        label_number_++;
        AddCell(false, empty);
        while (lexem_->GetValue() != "case" && lexem_->GetValue() != "default" && lexem_->GetValue() != "}") {
            table_id.AddTable();
            STATEMENT(func_name);
            table_id.RemoveTable();
        }
        AddCell(false_label_number, empty);
        false_markers.push_back(false_label_number);
    }
    if (lexem_->GetValue() == "default") {
        GetLex();
        GetLex(); // :
        while (lexem_->GetValue() != "}") {
            table_id.AddTable();
            STATEMENT(func_name);
            table_id.RemoveTable();
        }
    }
    GetLex(); // }
}

void RPN::ID(std::vector<RPN_Node*> &container) {
    AddCell(lexem_, container);
    GetLex();
}

void RPN::TERM(std::vector<RPN_Node*> &container) {
    if (lexem_->GetType() != LexemType::Integer && lexem_->GetType() != LexemType::Float &&
        lexem_->GetType() != LexemType::String && lexem_->GetValue() != "true" && lexem_->GetValue() != "false") {
        if (lexem_->GetValue() == "(") {
            GetLex(); // (
            do {
                if (lexem_->GetValue() == ",") {
                    GetLex();
                }
                EXP_ONE(container);
            } while (lexem_->GetValue() == ",");
            GetLex(); // )
        }
        else {
            GetLex(); // {
            int counter = 1;
            std::vector<RPN_Node*> elements = {new RPN_Node("elem", "string")};
            while (lexem_->GetValue() == "{") {
                counter++;
                GetLex();
            }
            std::vector<std::vector<int>> sizes(counter);
            int elem_counter = 0;
            while (counter != 0) {
                while (lexem_->GetValue() == "{") {
                    counter++;
                    GetLex();
                }
                elem_counter = 0;
                do {
                    if (lexem_->GetValue() == ",") {
                        GetLex();
                    }
                    EXP_ONE(elements);
                    elem_counter++;
                } while (lexem_->GetValue() != "}");
                if (sizes[sizes.size() - 1].size() == 0) sizes[sizes.size() - 1].push_back(elem_counter);
                else sizes[sizes.size() - 1][sizes[sizes.size() - 1].size() - 1] = elem_counter;
                while (lexem_->GetValue() == "}") {
                    if (counter - 1 >= 0) {
                        if (sizes[counter - 1].size() == 0) {
                            sizes[counter - 1].push_back(sizes[counter].size());
                        } else if (counter != sizes.size()) {
                            sizes[counter - 1][sizes[counter - 1].size() - 1] = sizes[counter].size();
                            for (int i = 0; i < sizes[counter - 1].size() - 1; ++i) {
                                sizes[counter - 1][sizes[counter - 1].size() - 1] -= sizes[counter - 1][i];
                            }
                        }
                    }
                    counter--;
                    GetLex();
                }
                if (lexem_->GetValue() == ",") {
                    for (int i = counter; i < sizes.size(); ++i) {
                        sizes[i].push_back(0);
                    }
                    GetLex();
                }
            }
            elements.erase(elements.begin());
            AddCell(elements, sizes, container);
        }
    }
    else {
        AddCell(lexem_, container);
        GetLex();
    }
}

void RPN::EXP_ZERO(std::vector<RPN_Node*> &container) {
    EXP_ONE(container);
    while (lexem_->GetValue() == ",") {
        GetLex();
        EXP_ONE(container);
    }
}

void RPN::EXP_ONE(std::vector<RPN_Node*> &container) {
    EXP_TWO(container);
    while (lexem_->GetValue() == "+=" || lexem_->GetValue() == "-=" || lexem_->GetValue() == "*=" || lexem_->GetValue() == "/=" || lexem_->GetValue() == "%=") {
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        GetLex();
        EXP_TWO(container);
        AddCell(operation, container);
    }
}

void RPN::EXP_TWO(std::vector<RPN_Node*> &container) {
    EXP_THREE(container);
    while (lexem_->GetValue() == "||") {
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        GetLex();
        EXP_THREE(container);
        AddCell(operation, container);
    }
}

void RPN::EXP_THREE(std::vector<RPN_Node*> &container) {
    EXP_FOUR(container);
    while (lexem_->GetValue() == "&&") {
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        GetLex();
        EXP_FOUR(container);
        AddCell(operation, container);
    }
}

void RPN::EXP_FOUR(std::vector<RPN_Node*> &container) {
    EXP_FIVE(container);
    while (lexem_->GetValue() == "|") {
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        GetLex();
        EXP_FIVE(container);
        AddCell(operation, container);
    }
}

void RPN::EXP_FIVE(std::vector<RPN_Node*> &container) {
    EXP_SIX(container);
    while (lexem_->GetValue() == "^") {
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        GetLex();
        EXP_SIX(container);
        AddCell(operation, container);
    }
}

void RPN::EXP_SIX(std::vector<RPN_Node*> &container) {
    EXP_SEVEN(container);
    while (lexem_->GetValue() == "&") {
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        GetLex();
        EXP_SEVEN(container);
        AddCell(operation, container);
    }
}

void RPN::EXP_SEVEN(std::vector<RPN_Node*> &container) {
    EXP_EIGHT(container);
    bool checker = false;
    while (lexem_->GetValue() == "==" || lexem_->GetValue() == "!=") {
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        GetLex();
        EXP_EIGHT(container);
        AddCell(operation, container);
    }
}

void RPN::EXP_EIGHT(std::vector<RPN_Node*> &container) {
    EXP_NINE(container);
    while (lexem_->GetValue() == ">=" || lexem_->GetValue() == "<=" || lexem_->GetValue() == "<" || lexem_->GetValue() == ">") {
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        GetLex();
        EXP_NINE(container);
        AddCell(operation, container);
    }
}

void RPN::EXP_NINE(std::vector<RPN_Node*> &container) {
    EXP_TEN(container);
    while (lexem_->GetValue() == ">>" || lexem_->GetValue() == "<<") {
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        GetLex();
        EXP_TEN(container);
        AddCell(operation, container);
    }
}

void RPN::EXP_TEN(std::vector<RPN_Node*> &container) {
    EXP_ELEVEN(container);
    while (lexem_->GetValue() == "+" || lexem_->GetValue() == "-") {
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        GetLex();
        EXP_ELEVEN(container);
        AddCell(operation, container);
    }
}

void RPN::EXP_ELEVEN(std::vector<RPN_Node*> &container) {
    EXP_TWELVE(container);
    while (lexem_->GetValue() == "*" || lexem_->GetValue() == "/" || lexem_->GetValue() == "%") {
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        GetLex();
        EXP_TWELVE(container);
        AddCell(operation, container);
    }
}

void RPN::EXP_TWELVE(std::vector<RPN_Node*> &container) {
    std::vector<std::string> oper;
    while (lexem_->GetValue() == "+" || lexem_->GetValue() == "-" || lexem_->GetValue() == "++"
        || lexem_->GetValue() == "--" || lexem_->GetValue() == "!") {
        if (lexem_->GetValue() == "+" || lexem_->GetValue() == "-") {
            oper.push_back("0");
        }
        oper.push_back(lexem_->GetValue());
        GetLex();
    }
    std::vector<std::string> new_oper;
    std::vector<std::string> signs;
    for (int i = 0; i < oper.size(); ++i) {
        if (oper[i] == "0") {
            RPN_Node* elem = new RPN_Node(oper[i], "integer");
            AddCell(elem, container);
        } else {
            if (oper[i] == "+" || oper[i] == "-") {
                signs.push_back(oper[i]);
            } else {
                new_oper.push_back(oper[i]);
            }
        }
    }
    EXP_THIRTEEN(container);
    for (int i = 0; i < signs.size(); ++i) {
        RPN_Node* operation = new RPN_Node(signs[i], "other");
        AddCell(operation, container);
    }
    for (int i = 0; i < new_oper.size(); ++i) {
        RPN_Node* operation = new RPN_Node(new_oper[i], "other");
        AddCell(operation, container);
    }
}

void RPN::EXP_THIRTEEN(std::vector<RPN_Node*> &container) {
    EXP_FOURTEEN(container);
    if (lexem_->GetValue() == "[") {
        while (lexem_->GetValue() == "[") {
            GetLex(); // [
            EXP_ZERO(container);
            GetLex(); // ]
            RPN_Node* sign = new RPN_Node("[", "other");
            AddCell(sign, empty);
            sign = new RPN_Node("]", "other");
            AddCell(sign, container);
        }
    }
    while (lexem_->GetValue() == "++" || lexem_->GetValue() == "--") {
        RPN_Node* sign = new RPN_Node(lexem_->GetValue(), "other");
        AddCell(sign, container);
        GetLex();
    }
}

void RPN::EXP_FOURTEEN(std::vector<RPN_Node*> &container) {
    if (lexem_->GetType() == LexemType::Identifier) {
        if (lex_index_ < (int)list_of_lexems_.size() && list_of_lexems_[lex_index_]->GetValue() == "(") {
            std::string id_name = lexem_->GetValue();
            GetLex(); // id
            FUNC_CALL(id_name);
        }
        else {
            ID(container);
        }
        while (lexem_->GetValue() == "::") {
            GetLex();
            ID(container);
        }
    } else {
        TERM(container);
    }
}

void RPN::LOOP(std::string func_name) {
    if (lexem_->GetValue() == "for") {
        GetLex();
        LOOP_FOR(func_name);
    }
    else if (lexem_->GetValue() == "foreach") {
        GetLex();
        LOOP_FOREACH(func_name);
    }
    else if (lexem_->GetValue() == "while") {
        GetLex();
        LOOP_WHILE(func_name);
    }
}

void RPN::LOOP_WHILE(std::string func_name) {
    GetLex(); // (
    AddCell(label_number_, while_loop);
    EXP_ZERO(while_loop);
    GetLex(); // )
    AddCell(true, label_number_ + 1, while_loop);
    AddCell(true, while_loop);
    AddCell(true, label_number_, while_loop);
    AddCell(label_number_ + 1, while_loop);
    table_id.AddTable();
    BODY(func_name);
    table_id.RemoveTable();
}

void RPN::LOOP_FOREACH(std::string func_name) {
    GetLex(); // (
    AddCell(lexem_, empty);
    GetLex(); // id
    GetLex(); // ;
    EXP_ZERO(empty); //Комментарий: здесь уже должна быть проверка с массивом и значениями, что делается, кажется, в реализации кода
    GetLex(); // )
    table_id.AddTable();
    BODY(func_name);
    table_id.RemoveTable();
}

void RPN::LOOP_FOR(std::string func_name) {
    int id_line_, id_column_;
    std::pair<std::vector<std::string>, std::pair<std::vector<std::string>, std::vector<std::string>>> current_type_;
    std::string current_id_name_;
    table_id.AddTable();
    GetLex(); // (
    // first part
    if (lexem_->GetValue() == ";") {
        GetLex();
    }
    else { // type
        if (lexem_->GetValue() == "array") {
            current_type_ = { { "array" }, {} };
            GetLex();
            int counter = 0;
            while (lexem_->GetValue() == "<") {
                counter++;
                GetLex(); // <
                if (lexem_->GetValue() == "array") {
                    GetLex(); // array
                    current_type_.first.push_back("array");
                }
                else {
                    current_type_.first.push_back(lexem_->GetValue());
                    GetLex(); // type
                    break;
                }
            }
            while (counter) {
                if (lexem_->GetValue() == ">>") counter -= 2;
                else counter--;
                GetLex();
            }
        }
        else {
            current_type_ = { { lexem_->GetValue() }, {} };
            GetLex(); // type
        }
        current_id_name_ = lexem_->GetValue();
        id_line_ = lexem_->GetLine();
        id_column_ = lexem_->GetColumn();
        if (!table_id.IsUsed(current_id_name_)) {
            table_id.AddID({ current_type_ }, current_id_name_, id_line_, id_column_);
        }
        AddCell(new RPN_Node(lexem_->GetValue(), "identifier"), empty);
        GetLex();
        VARS_MDEF();
        GetLex(); // ;
    }
    first_common_label_number = label_number_;
    last_common_label_number = label_number_ + 1;
    first_array.push_back(first_common_label_number);
    last_array.push_back(last_common_label_number);
    label_number_ += 2;
    AddCell(first_common_label_number, empty);
    // second part
    if (lexem_->GetValue() == ";") {
        GetLex();
    }
    else {
        std::vector<std::string> type;
        std::vector<std::string> bool_num = { "bool" };
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            EXP_ONE(empty);
        } while (lexem_->GetValue() == ",");
        GetLex(); // ;
    }
    AddCell(true, last_common_label_number, empty);
    AddCell(true, empty);
    //third part
    if (lexem_->GetValue() == ")") {
        GetLex();
    }
    else {
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            EXP_ONE(for_loop);
        } while (lexem_->GetValue() == ",");
        GetLex(); // )
    }
    BODY(func_name);
    table_id.RemoveTable();
    for (int i = 1; i < for_loop.size(); ++i) {
        chain_.push_back(for_loop[i]);
    }
    for_loop = {new RPN_Node("for", "utility")};
    AddCell(true, first_common_label_number, empty);
    AddCell(last_common_label_number, empty);
    first_array.pop_back();
    last_array.pop_back();
}


RPN_TID_Node::~RPN_TID_Node() {
    for (auto id : ID) {
        delete id.second;
    }
}

RPN_TID::RPN_TID() {
    root_ = new RPN_TID_Node;
}

RPN_TID::~RPN_TID() {
    while (root_) {
        RPN_TID_Node* pred_ = root_->pred;
        delete root_;
        root_ = pred_;
    }
}

void RPN_TID::AddID(std::string id_name, RPN_Node* value) {
    root_->ID[id_name] = value;
}

bool RPN_TID::FindID(std::string id_name) {
    RPN_TID_Node* temp = root_;
    do {
        for (auto id : temp->ID) {
            if (id.first == id_name) {
                return true;
            }
        }
        temp = temp->pred;
    } while(temp);
    return false;
}

void RPN_TID::setValue(std::string id_name, RPN_Node* value) {
    RPN_TID_Node* temp = root_;
    do {
        for (auto id : temp->ID) {
            if (id.first == id_name) {
                id.second = value;
                break;
            }
        }
        temp = temp->pred;
    } while(temp);
}

RPN_Node* RPN_TID::getValue(std::string id_name) {
    RPN_Node* value = new RPN_Node();
    RPN_TID_Node* temp = root_;
    do {
        for (auto id : temp->ID) {
            if (id.first == id_name) {
                value = id.second;
            }
        }
        temp = temp->pred;
    } while(temp);
    return value;
}

void RPN_TID::AddTable() {
    RPN_TID_Node *add_table = new RPN_TID_Node;
    RPN_TID_Node* temp = root_;
    root_->next = add_table;
    root_ = root_->next;
    root_->pred = temp;
}

void RPN_TID::RemoveTable() {
    RPN_TID_Node* temp = root_->pred;
    delete root_;
    root_ = temp;
}
