#include "../includes/rpn.h"
#include <fstream>

std::vector<RPN_Node*> empty = {};

void RPN::FindFunction(std::string id_name) {
    int counter = 0;
    for (auto elem : chain_) {
        if (elem->GetType() == "function") {
            if (elem->GetName() == id_name) {
                for (auto label_elem : chain_) {
                    if (label_elem->IsEmptyLabel()) {
                        chain_.insert(chain_.begin() + counter, new RPN_Node(true, label_number_));
                        label_number_++;
                        break;
                    }
                }
                break;
            } else counter++;
        } else counter++;
    }
}
//ОК
void RPN::Analyze(const std::vector<Lexem*>& list_of_lexems) {
    list_of_lexems_ = list_of_lexems;
    GetLex();
    PROGRAM();
}
//ОК
void RPN::GetLex() {
    if (lex_index_ < (int)list_of_lexems_.size()) {
        lexem_ = list_of_lexems_[lex_index_];
        ++lex_index_;
    }
    else {
        ++lex_index_;
    }
}
//ОК
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
            AddCell(new RPN_Node("empty_label"), empty);
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
    AddCell(new RPN_Node("empty_label"), empty);
}
//OK
void RPN::VARS_MDEF() {
    int id_line_, id_column_;
    std::pair<std::vector<std::string>, std::pair<std::vector<std::string>, std::vector<std::string>>> current_type_;
    std::string current_id_name_;
    if (lexem_->GetValue() == ";") {
        return;
    }
    if (lexem_->GetValue() == "=") {
        GetLex();
        EXP_ONE(empty);
        RPN_Node* equality = new RPN_Node(lexem_->GetValue());
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
            RPN_Node* equality = new RPN_Node(lexem_->GetValue());
            AddCell(equality, empty);
        }
    }
}
//OK
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
//OK
void RPN::BODY(std::string func_name) {
    GetLex(); // {
    while (lexem_->GetValue() != "}") {
        STATEMENT(func_name);
    }
    GetLex(); // }
}
//OK
void RPN::FUNC_BODY(std::string func_name) {
    GetLex(); // {
    bool checker = false;
    while (lexem_->GetValue() != "}") {
        FUNC_STATEMENT(checker, func_name);
    }
    GetLex(); // }
}
//OK
void RPN::STATEMENT(std::string func_name) {
    AddCell(new RPN_Node(label_number_), empty);
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
        GetLex(); // id
        VARS_MDEF();
        GetLex();
    }
    else if (lexem_->GetType() == LexemType::Identifier) {
        current_id_name_ = lexem_->GetValue();
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
        AddCell(new RPN_Node(true, label_number_ + 1), empty);
        GetLex(); // break
        GetLex(); // ;
    }
    else if (lexem_->GetValue() == "continue") {
        AddCell(new RPN_Node(true, label_number_), empty);
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
    AddCell(new RPN_Node(label_number_ + 1), empty);
    label_number_ += 2;
}
//OK
void RPN::FUNC_STATEMENT(bool &check, std::string func_name) {
    AddCell(new RPN_Node(label_number_), empty);
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
        GetLex(); // id
        VARS_MDEF();
        GetLex();
    }
    else if (lexem_->GetType() == LexemType::Identifier) {
        current_id_name_ = lexem_->GetValue();
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
        AddCell(new RPN_Node(true, label_number_ + 1), empty);
        GetLex(); // break
        GetLex(); // ;
    }
    else if (lexem_->GetValue() == "continue") {
        AddCell(new RPN_Node(true, label_number_), empty);
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
    AddCell(new RPN_Node(label_number_ + 1), empty);
    label_number_ += 2;
}
//OK
void RPN::INPUT() {
    GetLex(); // >>
    RPN_Node* id = new RPN_Node(lexem_);
    AddCell(id, empty);
    GetLex(); // ID;
    while (lexem_->GetValue() == ">>") {
        GetLex(); // >>
        id = new RPN_Node(lexem_);
        AddCell(id, empty);
        GetLex(); // ID
    }
    RPN_Node* input = new RPN_Node("cin", "utility");
    AddCell(input, empty);
}
//ОК
void RPN::OUTPUT() {
    GetLex(); // <<
    EXP_ZERO(empty);
    while (lexem_->GetValue() == "<<") {
        GetLex(); // <<
        EXP_ZERO(empty);
    }
    RPN_Node* output = new RPN_Node("cout", "utility");
    AddCell(output, empty);
}
//ОК
void RPN::FUNC_CALL(std::string id_name) {
    int index = 0;
    GetLex(); // (
    bool checker = false;
    while (lexem_->GetType() == LexemType::Identifier || lexem_->GetType() == LexemType::String ||
        lexem_->GetType() == LexemType::Integer || lexem_->GetType() == LexemType::Float || lexem_->GetValue() == "{" || lexem_->GetValue() == "(") {
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            RPN_Node* param = new RPN_Node(table_id.getType(id_name).second.second[index], "identifier");
            AddCell(param, empty);
            EXP_ONE(empty);
            RPN_Node* sign = new RPN_Node("=", "other");
            AddCell(sign, empty);
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
//ОК
void RPN::IF(std::string func_name) {
    int counter = 0;
    GetLex(); // (
    EXP_ZERO(empty);
    GetLex(); // )
    AddCell(false, empty);
    table_id.AddTable();
    BODY(func_name);
    table_id.RemoveTable();
    AddCell(true, label_number_, empty);
    ++label_number_;
    ++counter;
    while (lexem_->GetValue() == "elif") {
        GetLex(); // elif
        GetLex(); // (
        EXP_ZERO(empty);
        GetLex(); // )
        AddCell(false, empty);
        table_id.AddTable();
        BODY(func_name);
        table_id.RemoveTable();
        AddCell(true, label_number_, empty);
        ++label_number_;
        ++counter;
    }
    if (lexem_->GetValue() == "else") {
        GetLex(); // else
        table_id.AddTable();
        BODY(func_name);
        table_id.RemoveTable();
    }
    while (counter) {
        AddCell(label_number_ - counter, empty);
        --counter;
    }
}
//ОК
void RPN::SWITCH(std::string func_name) {
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
        AddCell(false, empty);
        while (lexem_->GetValue() != "case" && lexem_->GetValue() != "default" && lexem_->GetValue() != "}") {
            table_id.AddTable();
            STATEMENT(func_name);
            table_id.RemoveTable();
        }
        AddCell(true, label_number_, empty);
        ++label_number_;
        ++counter;
    }
    if (lexem_->GetValue() == "default") {
        GetLex();
        GetLex(); // :
        AddCell(false, empty);
        while (lexem_->GetValue() != "}") {
            table_id.AddTable();
            STATEMENT(func_name);
            table_id.RemoveTable();
        }
    }
    GetLex(); // }
    while (counter) {
        AddCell(label_number_ - counter, empty);
        counter--;
    }
}
//OK
void RPN::ID() {
    AddCell(lexem_, empty);
    GetLex();
}
//ОК
void RPN::TERM() {
    if (lexem_->GetType() != LexemType::Integer && lexem_->GetType() != LexemType::Float &&
        lexem_->GetType() != LexemType::String && lexem_->GetValue() != "true" && lexem_->GetValue() != "false") {
        if (lexem_->GetValue() == "(") {
            GetLex(); // (
            do {
                if (lexem_->GetValue() == ",") {
                    GetLex();
                }
                EXP_ONE(empty);
            } while (lexem_->GetValue() == ",");
            GetLex(); // )
        }
        else {
            GetLex(); // {
            int counter = 1;
            std::vector<RPN_Node*> elements;
            while (lexem_->GetValue() == "{") {
                counter++;
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
                            sizes[counter - 1].push_back(1);
                        } else sizes[counter - 1][sizes[counter - 1].size() - 1]++;
                    }
                    counter--;
                    GetLex();
                }
                if (lexem_->GetValue() == ",") {
                    sizes[counter].push_back(0);
                    GetLex();
                }
            }
            AddCell(elements, sizes, empty);
        }
    }
    else {
        AddCell(lexem_, empty);
        GetLex();
    }
}
//ОК
void RPN::EXP_ZERO(std::vector<RPN_Node*> &container) {
    EXP_ONE(container);
    while (lexem_->GetValue() == ",") {
        GetLex();
        EXP_ONE(container);
    }
}
//ОК
void RPN::EXP_ONE(std::vector<RPN_Node*> &container) {
    EXP_TWO(container);
    while (lexem_->GetValue() == "+=" || lexem_->GetValue() == "-=" || lexem_->GetValue() == "*=" || lexem_->GetValue() == "/=" || lexem_->GetValue() == "%=") {
        GetLex();
        EXP_TWO(container);
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        AddCell(operation, empty);
    }
}
//ОК
void RPN::EXP_TWO(std::vector<RPN_Node*> &container) {
    EXP_THREE(container);
    while (lexem_->GetValue() == "||") {
        GetLex();
        EXP_THREE(container);
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        AddCell(operation, empty);
    }
}
//ОК
void RPN::EXP_THREE(std::vector<RPN_Node*> &container) {
    EXP_FOUR(container);
    while (lexem_->GetValue() == "&&") {
        GetLex();
        EXP_FOUR(container);
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        AddCell(operation, empty);
    }
}
//ОК
void RPN::EXP_FOUR(std::vector<RPN_Node*> &container) {
    EXP_FIVE(container);
    while (lexem_->GetValue() == "|") {
        GetLex();
        EXP_FIVE(container);
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        AddCell(operation, empty);
    }
}
//ОК
void RPN::EXP_FIVE(std::vector<RPN_Node*> &container) {
    EXP_SIX(container);
    while (lexem_->GetValue() == "^") {
        GetLex();
        EXP_SIX(container);
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        AddCell(operation, empty);
    }
}
//ОК
void RPN::EXP_SIX(std::vector<RPN_Node*> &container) {
    EXP_SEVEN(container);
    while (lexem_->GetValue() == "&") {
        GetLex();
        EXP_SEVEN(container);
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        AddCell(operation, empty);
    }
}
//ОК
void RPN::EXP_SEVEN(std::vector<RPN_Node*> &container) {
    EXP_EIGHT(container);
    bool checker = false;
    while (lexem_->GetValue() == "==" || lexem_->GetValue() == "!=") {
        GetLex();
        EXP_EIGHT(container);
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        AddCell(operation, empty);
    }
}
//ОК
void RPN::EXP_EIGHT(std::vector<RPN_Node*> &container) {
    EXP_NINE(container);
    while (lexem_->GetValue() == ">=" || lexem_->GetValue() == "<=" || lexem_->GetValue() == "<" || lexem_->GetValue() == ">") {
        GetLex();
        EXP_NINE(container);
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        AddCell(operation, empty);
    }
}
//ОК
void RPN::EXP_NINE(std::vector<RPN_Node*> &container) {
    EXP_TEN(container);
    while (lexem_->GetValue() == ">>" || lexem_->GetValue() == "<<") {
        GetLex();
        EXP_TEN(container);
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        AddCell(operation, empty);
    }
}
//ОК
void RPN::EXP_TEN(std::vector<RPN_Node*> &container) {
    EXP_ELEVEN(container);
    while (lexem_->GetValue() == "+" || lexem_->GetValue() == "-") {
        GetLex();
        EXP_ELEVEN(container);
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        AddCell(operation, empty);
    }
}
//ОК
void RPN::EXP_ELEVEN(std::vector<RPN_Node*> &container) {
    EXP_TWELVE(container);
    while (lexem_->GetValue() == "*" || lexem_->GetValue() == "/" || lexem_->GetValue() == "%") {
        GetLex();
        EXP_TWELVE(container);
        RPN_Node* operation = new RPN_Node(lexem_->GetValue(), "other");
        AddCell(operation, empty);
    }
}
//ОК
void RPN::EXP_TWELVE(std::vector<RPN_Node*> &container) {
    std::vector<std::string> oper;
    while (lexem_->GetValue() == "+" || lexem_->GetValue() == "-" || lexem_->GetValue() == "++"
        || lexem_->GetValue() == "--" || lexem_->GetValue() == "!") {
        oper.push_back(lexem_->GetValue());
        GetLex();
    }
    EXP_THIRTEEN(container);
    for (int i = 0; i < oper.size(); ++i) {
        RPN_Node* operation = new RPN_Node(oper[i], "other");
        AddCell(operation, empty);
    }
}
//ОК
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
            AddCell(sign, empty);
        }
    }
    while (lexem_->GetValue() == "++" || lexem_->GetValue() == "--") {
        RPN_Node* sign = new RPN_Node(lexem_->GetValue(), "other");
        GetLex();
    }
}
//ОК
void RPN::EXP_FOURTEEN(std::vector<RPN_Node*> &container) {
    if (lexem_->GetType() == LexemType::Identifier) {
        if (lex_index_ < (int)list_of_lexems_.size() && list_of_lexems_[lex_index_]->GetValue() == "(") {
            std::string id_name = lexem_->GetValue();
            GetLex(); // id
            FUNC_CALL(id_name);
        }
        else {
            ID();
        }
        while (lexem_->GetValue() == "::") {
            GetLex();
            ID();
        }
    } else {
        TERM();
    }
}
//ОК
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
//ОК
void RPN::LOOP_WHILE(std::string func_name) {
    GetLex(); // (
    AddCell(label_number_, empty);
    EXP_ZERO(empty);
    GetLex(); // )
    AddCell(true, label_number_ + 1, empty);
    AddCell(true, empty);
    table_id.AddTable();
    BODY(func_name);
    table_id.RemoveTable();
    AddCell(true, label_number_, empty);
    AddCell(label_number_ + 1, empty);
    label_number_ += 2;
}
//OK
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
//OK
void RPN::LOOP_FOR(std::string func_name) {
    GetLex(); // (
    table_id.AddTable();
    // first part
    if (lexem_->GetValue() == ";") {
        GetLex();
    }
    else { // type
        if (lexem_->GetValue() == "array") {
            GetLex();
            int counter = 0;
            while (lexem_->GetValue() == "<") {
                counter++;
                GetLex(); // <
                if (lexem_->GetValue() == "array") {
                    GetLex(); // array
                }
                else {
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
            GetLex(); // type
        }
        GetLex();
        VARS_MDEF();
        GetLex(); // ;
    }
    // second part
    AddCell(label_number_, empty);
    if (lexem_->GetValue() == ";") {
        GetLex();
    }
    else {
        EXP_ZERO(empty);
        GetLex(); // ;
    }
    AddCell(true, label_number_ + 1, empty);
    AddCell(true, empty);
    //third part
    std::vector<RPN_Node*> third_cycle_part;
    if (lexem_->GetValue() == ")") {
        GetLex();
    }
    else {
        EXP_ZERO(third_cycle_part);
        GetLex(); // )
    }
    BODY(func_name);
    table_id.RemoveTable();
    for (int i = 0; i < third_cycle_part.size(); ++i) {
        chain_.push_back(third_cycle_part[i]);
    }
    AddCell(true, label_number_, empty);
    AddCell(label_number_ + 1, empty);
    label_number_ += 2;
}
//OK

RPN_TID_Node::~RPN_TID_Node() {
    for (auto id : ID) {
        delete id.second;
    }
}
//OK
RPN_TID::RPN_TID() {
    root_ = new RPN_TID_Node;
}
//OK
RPN_TID::~RPN_TID() {
    while (root_) {
        RPN_TID_Node* pred_ = root_->pred;
        delete root_;
        root_ = pred_;
    }
}
//OK
void RPN_TID::AddID(std::string id_name, RPN_Node* value) {
    root_->ID[id_name] = value;
}
//OK
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
//OK
RPN_Node* RPN_TID::getValue(std::string id_name) {
    RPN_TID_Node* temp = root_;
    do {
        for (auto id : temp->ID) {
            if (id.first == id_name) {
                return id.second;
            }
        }
        temp = temp->pred;
    } while(temp);
}
//OK
void RPN_TID::AddTable() {
    RPN_TID_Node *add_table = new RPN_TID_Node;
    RPN_TID_Node* temp = root_;
    root_->next = add_table;
    root_ = root_->next;
    root_->pred = temp;
}
//OK
void RPN_TID::RemoveTable() {
    RPN_TID_Node* temp = root_->pred;
    delete root_;
    root_ = temp;
}
//OK