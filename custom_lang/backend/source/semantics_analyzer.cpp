#include "../includes/semantics_analyzer.h"
#include <fstream>

void SemanticsAnalyzer::Analyze(const std::vector<Lexem*>& list_of_lexems) {
    list_of_lexems_ = list_of_lexems;
    GetLex();
    PROGRAM();
}

void SemanticsAnalyzer::GetLex() {
    if (lex_index_ < (int)list_of_lexems_.size()) {
        lexem_ = list_of_lexems_[lex_index_];
        ++lex_index_;
    }
    else {
        ++lex_index_;
    }
}

void SemanticsAnalyzer::PROGRAM() {
    while (3 + 5 == 8) {
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
        GetLex(); // id
        if (lexem_->GetValue() == "(") {
            GetLex();
            if (table_id_.CheckAddingID(current_id_name_)) {
                current_type_.first.insert(current_type_.first.begin(), "function");
                table_id_.AddID(current_type_, current_id_name_);
            }
            else {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " the name is already used";
            }
            table_id_.AddTable();
            FUNC(current_id_name_);
            table_id_.RemoveTable();
        }
        else {
            if (table_id_.CheckAddingID(current_id_name_)) {
                table_id_.AddID( current_type_ , current_id_name_);
            }
            else {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " the name is already used";
            }
            VARS_MDEF(current_type_.first);
            GetLex();
        }
    }
    current_id_name_ = lexem_->GetValue();
    if (table_id_.CheckAddingID(current_id_name_)) {
        current_type_.first.insert(current_type_.first.begin(), "function");
        table_id_.AddID(current_type_, current_id_name_);
    }
    else {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " the name is already used";
    }
    GetLex(); // main
    GetLex(); // (
    GetLex(); // )
    table_id_.AddTable();
    FUNC_BODY("main");
    table_id_.RemoveTable();
}

void SemanticsAnalyzer::VARS_MDEF(std::vector<std::string> id_type) {
    if (lexem_->GetValue() == ";") {
        return;
    }
    if (lexem_->GetValue() == "=") {
        GetLex();
        std::vector<std::string> type;
        type = EXP_ONE();
        if (type != id_type) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " found type mismatch";
        }
    }
    while (lexem_->GetValue() == ",") {
        GetLex();
        current_id_name_ = lexem_->GetValue();
        if (table_id_.CheckAddingID(current_id_name_)) {
            table_id_.AddID({ current_type_ }, current_id_name_);
        }
        else {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " the name is already used";
        }
        GetLex(); // id
        std::vector<std::string> type;
        if (lexem_->GetValue() == "=") {
            GetLex();
            type = EXP_ONE();
            if (type != id_type) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
    }
}

void SemanticsAnalyzer::FUNC(std::string id_name) {
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
        if (table_id_.CheckAddingID(current_id_name_)) {
            std::pair<std::vector<std::string>, std::vector<std::string>> new_type = table_id_.GetType(id_name);
            for (int i = 0; i < (int)current_type_.first.size(); ++i) {
                new_type.second.push_back(current_type_.first[i]);
            }
            table_id_.ChangeType(id_name, new_type);
            table_id_.AddID(current_type_, current_id_name_);
        }
        else {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " the name is already used";
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
            if (table_id_.CheckAddingID(current_id_name_)) {
                std::pair<std::vector<std::string>, std::vector<std::string>> new_type = table_id_.GetType(id_name);
                for (int i = 0; i < (int)current_type_.first.size(); ++i) {
                    new_type.second.push_back(current_type_.first[i]);
                }
                table_id_.ChangeType(id_name, new_type);
                table_id_.AddID(current_type_, current_id_name_);
            }
            else {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " the name is already used";
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

void SemanticsAnalyzer::BODY(std::string func_name) {
    GetLex(); // {
    while (lexem_->GetValue() != "}") {
        STATEMENT(func_name);
    }
    GetLex(); // }
}

void SemanticsAnalyzer::FUNC_BODY(std::string func_name) {
    GetLex(); // {
    bool checker = false;
    while (lexem_->GetValue() != "}") {
        FUNC_STATEMENT(checker, func_name);
    }
    GetLex(); // }
}

void SemanticsAnalyzer::STATEMENT(std::string func_name) {
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
        if (table_id_.CheckAddingID(current_id_name_)) {
            table_id_.AddID({ current_type_ }, current_id_name_);
        }
        else {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " the name is already used";
        }
        GetLex(); // id
        VARS_MDEF(current_type_.first);
        GetLex();
    }
    else if (lexem_->GetType() == LexemType::Identifier) {
        current_id_name_ = lexem_->GetValue();
        if (!table_id_.CheckUsingID(current_id_name_)) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " ID was not declared";
        }
        if (lex_index_ < (int)list_of_lexems_.size() && (list_of_lexems_[lex_index_]->GetValue() == "=")) {
            GetLex();
            VARS_MDEF(table_id_.GetType(current_id_name_).first);
        }
        else {
            do {
                if (lexem_->GetValue() == ",") {
                    GetLex();
                }
                EXP_ONE();
            } while (lexem_->GetValue() == ",");
        }
        GetLex(); // ,
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
        GetLex(); // break
        GetLex(); // ;
    }
    else if (lexem_->GetValue() == "continue") {
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
            std::vector<std::string> type = { "void" };
            type.insert(type.begin(), "function");
            if (type != table_id_.GetType(func_name).first) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " function has a different type";
            }
            GetLex();
        }
        else {
            std::vector<std::string> type;
            do {
                if (lexem_->GetValue() == ",") {
                    GetLex();
                }
                type = EXP_ONE();
            } while (lexem_->GetValue() == ",");
            type.insert(type.begin(), "function");
            if (type != table_id_.GetType(func_name).first) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " function has a different type";
            }
            GetLex(); // ;
        }
    }
}

void SemanticsAnalyzer::FUNC_STATEMENT(bool &check, std::string func_name) {
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
        if (table_id_.CheckAddingID(current_id_name_)) {
            table_id_.AddID({ current_type_ }, current_id_name_);
        }
        else {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " the name is already used";
        }
        GetLex(); // id
        VARS_MDEF(current_type_.first);
        GetLex();
    }
    else if (lexem_->GetType() == LexemType::Identifier) {
        current_id_name_ = lexem_->GetValue();
        if (!table_id_.CheckUsingID(current_id_name_)) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " ID was not declared";
        }
        if (lex_index_ < (int)list_of_lexems_.size() && (list_of_lexems_[lex_index_]->GetValue() == "=")) {
            GetLex();
            VARS_MDEF(table_id_.GetType(current_id_name_).first);
        }
        else {
            do {
                if (lexem_->GetValue() == ",") {
                    GetLex();
                }
                EXP_ONE();
            } while (lexem_->GetValue() == ",");
        }
        GetLex(); // ,
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
        GetLex(); // break
        GetLex(); // ;
    }
    else if (lexem_->GetValue() == "continue") {
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
            std::vector<std::string> type = { "void" };
            type.insert(type.begin(), "function");
            if (type != table_id_.GetType(func_name).first) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " function has a different type";
            }
            GetLex();
        }
        else {
            std::vector<std::string> type;
            do {
                if (lexem_->GetValue() == ",") {
                    GetLex();
                }
                type = EXP_ONE();
            } while (lexem_->GetValue() == ",");
            type.insert(type.begin(), "function");
            if (type != table_id_.GetType(func_name).first) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " function has a different type";
            }
            GetLex(); // ;
        }
    }
}

void SemanticsAnalyzer::INPUT() {
    GetLex(); // >>
    current_id_name_ = lexem_->GetValue();
    if (!table_id_.CheckUsingID(current_id_name_)) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " ID was not declared";
    }
    GetLex(); // id;
    while (lexem_->GetValue() == ">>") {
        GetLex(); // >>
        current_id_name_ = lexem_->GetValue();
        if (!table_id_.CheckUsingID(current_id_name_)) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " ID was not declared";
        }
        GetLex(); // >>
    }
}

void SemanticsAnalyzer::OUTPUT() {
    GetLex(); // <<
    EXP_ZERO();
    while (lexem_->GetValue() == "<<") {
        GetLex(); // <<
        EXP_ZERO();
    }
}

std::vector<std::string> SemanticsAnalyzer::FUNC_CALL(std::string id_name) {
    std::vector<std::string> parameter_types = table_id_.GetType(id_name).second;
    std::vector<std::string> all_types;
    int index = 0;
    GetLex(); // (
    bool checker = false;
    while (lexem_->GetType() == LexemType::Identifier || lexem_->GetType() == LexemType::String ||
        lexem_->GetType() == LexemType::Integer || lexem_->GetType() == LexemType::Float || lexem_->GetValue() == "{" || lexem_->GetValue() == "(") {
        checker = true;
        std::vector<std::string> type;
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            type = EXP_ONE();
            for (int i = 0; i < (int)type.size(); ++i) {
                if (type[i] == "function") continue;
                all_types.push_back(type[i]);
            }
        } while (lexem_->GetValue() == ",");
        GetLex(); // )
    }
    if (!checker) GetLex(); // )
    if (parameter_types != all_types) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " incorrect function call";
    }
    return table_id_.GetType(id_name).first;
}

void SemanticsAnalyzer::IF(std::string func_name) {
    GetLex(); // (
    std::vector<std::string> type;
    do {
        if (lexem_->GetValue() == ",") {
            GetLex();
        }
        type = EXP_ONE();
    } while (lexem_->GetValue() == ",");
    std::vector<std::string> bool_num = { "bool" };
    if (type != bool_num) {
        std::string error = "";
        for (int i = 0; i < (int)type.size(); ++i) {
            error += type[i] + " ";
        }
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected bool, but found " + error;
    }
    GetLex(); // )
    table_id_.AddTable();
    BODY(func_name);
    table_id_.RemoveTable();
    while (lexem_->GetValue() == "elif") {
        GetLex(); // elif
        GetLex(); // (
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            type = EXP_ONE();
        } while (lexem_->GetValue() == ",");
        std::vector<std::string> bool_num = { "bool" };
        if (type != bool_num) {
            std::string error = "";
            for (int i = 0; i < (int)type.size(); ++i) {
                error += type[i] + " ";
            }
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected bool, but found " + error;
        }
        GetLex(); // )
        table_id_.AddTable();
        BODY(func_name);
        table_id_.RemoveTable();
    }
    if (lexem_->GetValue() == "else") {
        GetLex(); // else
        table_id_.AddTable();
        BODY(func_name);
        table_id_.RemoveTable();
    }
}

void SemanticsAnalyzer::SWITCH(std::string func_name) {
    GetLex(); // (
    std::vector<std::string> type;
    do {
        if (lexem_->GetValue() == ",") {
            GetLex();
        }
        type = EXP_ONE();
    } while (lexem_->GetValue() == ",");
    GetLex(); // )
    GetLex(); // {
    std::vector<std::string> case_type;
    while (lexem_->GetValue() == "case") {
        GetLex();
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            case_type = EXP_ONE();
        } while (lexem_->GetValue() == ",");
        if (case_type != type) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " found type mismatch";
        }
        GetLex(); // :
        while (lexem_->GetValue() != "case" && lexem_->GetValue() != "default" && lexem_->GetValue() != "}") {
            table_id_.AddTable();
            STATEMENT(func_name);
            table_id_.RemoveTable();
        }
    }
    if (lexem_->GetValue() == "default") {
        GetLex();
        GetLex(); // :
        while (lexem_->GetValue() != "}") {
            table_id_.AddTable();
            STATEMENT(func_name);
            table_id_.RemoveTable();
        }
    }
    GetLex(); // }
}

std::vector<std::string> SemanticsAnalyzer::ID() {
    current_id_name_ = lexem_->GetValue();
    if (!table_id_.CheckUsingID(current_id_name_)) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " ID was not declared";
    }
    GetLex();
    return table_id_.GetType(current_id_name_).first;
}

std::vector<std::string> SemanticsAnalyzer::TERM() {
    if (lexem_->GetType() != LexemType::Integer && lexem_->GetType() != LexemType::Float &&
        lexem_->GetType() != LexemType::String && lexem_->GetValue() != "true" && lexem_->GetValue() != "false") {
        std::vector<std::string> type;
        if (lexem_->GetValue() == "(") {
            GetLex(); // (
            do {
                if (lexem_->GetValue() == ",") {
                    GetLex();
                }
                type = EXP_ONE();
            } while (lexem_->GetValue() == ",");
            GetLex(); // )
            return type;
        }
        else {
            GetLex(); // {
            type.push_back("array");
            do {
                if (lexem_->GetValue() == ",") {
                    GetLex();
                }
                std::vector<std::string> exp_type = EXP_ONE();
                for (int i = 0; i < (int)exp_type.size(); ++i) {
                    type.push_back(exp_type[i]);
                }
                type.push_back("disarray");
            } while (lexem_->GetValue() == ",");
            GetLex(); // }
            type.push_back("disarray");
            std::vector<std::string> final_type;
            int count_brackets = 0;
            for (int i = 0; i < (int)type.size(); ++i) {
                if (i != (int)type.size() - 1) {
                    if (type[i] != "array") {
                        final_type.push_back(type[i]);
                        break;
                    }
                    else {
                        final_type.push_back(type[i]);
                        count_brackets++;
                    }
                }
            }
            for (int i = 0; i < (int)type.size(); ++i) {
                if (type[i] == "array") {
                    count_brackets--;
                }
                else if (type[i] == "disarray") {
                    count_brackets++;
                }
                else if (type[i] != final_type[(int)final_type.size() - 1]) {
                    throw "line: " + std::to_string(lexem_->GetLine()) +
                        " column: " + std::to_string(lexem_->GetColumn() - 1) +
                        " too many types in one array";
                }
                if (count_brackets < 0) {
                    throw "line: " + std::to_string(lexem_->GetLine()) +
                        " column: " + std::to_string(lexem_->GetColumn() - 1) +
                        " incorrect array identification";
                }
            }
            return final_type;
        }
    }
    else {
        if (lexem_->GetType() == LexemType::Integer) {
            GetLex();
            return { "int" };
        } else if (lexem_->GetType() == LexemType::Float) {
            GetLex();
            return { "double" };
        }
        else if (lexem_->GetValue() == "true" && lexem_->GetValue() == "false") {
            GetLex();
            return { "bool" };
        }
        else {
            GetLex();
            return { "string" };
        }
    }
}

std::vector<std::string> SemanticsAnalyzer::EXP_ZERO() {
    std::vector<std::string> type;
    type = EXP_ONE();
    while (lexem_->GetValue() == ",") {
        GetLex();
        type = EXP_ONE();
    }
    return type;
}

std::vector<std::string> SemanticsAnalyzer::EXP_ONE() {
    std::vector<std::string> integer = { "int" };
    std::vector<std::string> float_num = { "double" };
    std::vector<std::string> string_num = { "string" };
    std::vector<std::string> type_first = EXP_TWO();
    bool checker = false;
    while (lexem_->GetValue() == "+=" || lexem_->GetValue() == "-=" || lexem_->GetValue() == "*=" || lexem_->GetValue() == "/=" || lexem_->GetValue() == "%=") {
        checker = true;
        if (lexem_->GetValue() == "%=") {
            GetLex();
            std::vector<std::string> type_second = EXP_TWO();
            if (!(type_first == type_second && type_first == integer)) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
        else if (lexem_->GetValue() == "/=" || lexem_->GetValue() == "-=" || lexem_->GetValue() == "*=") {
            GetLex();
            std::vector<std::string> type_second = EXP_TWO();
            if (!(type_first == type_second && (type_first == integer || type_first == float_num))) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
        else {
            GetLex();
            std::vector<std::string> type_second = EXP_TWO();
            if (!(type_first == type_second && (type_first == integer || type_first == float_num || type_first == string_num))) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
    }
    return type_first;
}

std::vector<std::string> SemanticsAnalyzer::EXP_TWO() {
    std::vector<std::string> bool_num = { "bool" };
    std::vector<std::string> type_first = EXP_THREE();
    bool checker = false;
    while (lexem_->GetValue() == "||") {
        checker = true;
        GetLex();
        std::vector<std::string> type_second = EXP_THREE();
        if (!(type_first == type_second && type_first == bool_num)) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " found type mismatch";
        }
    }
    if (!checker) return type_first;
    else return { "bool" };
}

std::vector<std::string> SemanticsAnalyzer::EXP_THREE() {
    std::vector<std::string> bool_num = { "bool" };
    std::vector<std::string> type_first = EXP_FOUR();
    bool checker = false;
    while (lexem_->GetValue() == "&&") {
        checker = true;
        GetLex();
        std::vector<std::string> type_second = EXP_FOUR();
        if (!(type_first == type_second && type_first == bool_num)) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " found type mismatch";
        }
    }
    if (!checker) return type_first;
    else return { "bool" };
}

std::vector<std::string> SemanticsAnalyzer::EXP_FOUR() {
    std::vector<std::string> integer = { "int" };
    std::vector<std::string> float_num = { "double" };
    std::vector<std::string> string_num = { "string" };
    std::vector<std::string> type_first = EXP_FIVE();
    bool checker = false;
    while (lexem_->GetValue() == "|") {
        checker = true;
        if (lexem_->GetValue() != "|") {
            GetLex();
            std::vector<std::string> type_second = EXP_SIX();
            if (!(type_first == type_second && (type_first == integer || type_first == float_num || type_first == string_num))) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
        else {
            GetLex();
            std::vector<std::string> type_second = EXP_SIX();
            if (!(type_first == type_second && type_first == integer || type_first == float_num)) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
    }
    return type_first;
}

std::vector<std::string> SemanticsAnalyzer::EXP_FIVE() {
    std::vector<std::string> integer = { "int" };
    std::vector<std::string> float_num = { "double" };
    std::vector<std::string> string_num = { "string" };
    std::vector<std::string> type_first = EXP_SIX();
    bool checker = false;
    while (lexem_->GetValue() == "^") {
        checker = true;
        if (lexem_->GetValue() != "^") {
            GetLex();
            std::vector<std::string> type_second = EXP_SEVEN();
            if (!(type_first == type_second && (type_first == integer || type_first == float_num || type_first == string_num))) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
        else {
            GetLex();
            std::vector<std::string> type_second = EXP_SEVEN();
            if (!(type_first == type_second && type_first == integer || type_first == float_num)) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
    }
    return type_first;
}

std::vector<std::string> SemanticsAnalyzer::EXP_SIX() {
    std::vector<std::string> integer = { "int" };
    std::vector<std::string> float_num = { "double" };
    std::vector<std::string> string_num = { "string" };
    std::vector<std::string> type_first = EXP_SEVEN();
    bool checker = false;
    while (lexem_->GetValue() == "&") {
        checker = true;
        if (lexem_->GetValue() != "&") {
            GetLex();
            std::vector<std::string> type_second = EXP_EIGHT();
            if (!(type_first == type_second && (type_first == integer || type_first == float_num || type_first == string_num))) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
        else {
            GetLex();
            std::vector<std::string> type_second = EXP_EIGHT();
            if (!(type_first == type_second && type_first == integer || type_first == float_num)) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
    }
    return type_first;
}

std::vector<std::string> SemanticsAnalyzer::EXP_SEVEN() {
    std::vector<std::string> integer = { "int" };
    std::vector<std::string> float_num = { "double" };
    std::vector<std::string> string_num = { "string" };
    std::vector<std::string> bool_num = { "bool" };
    std::vector<std::string> type_first = EXP_EIGHT();
    bool checker = false;
    while (lexem_->GetValue() == "==" || lexem_->GetValue() == "!=") {
        checker = true;
        GetLex();
        std::vector<std::string> type_second = EXP_EIGHT();
        if (!(type_first == type_second && (type_first == integer || type_first == float_num || type_first == string_num || type_first == bool_num))) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " found type mismatch";
        }
    }
    if (!checker) return type_first;
    else return { "bool" };
}

std::vector<std::string> SemanticsAnalyzer::EXP_EIGHT() {
    std::vector<std::string> integer = { "int" };
    std::vector<std::string> float_num = { "double" };
    std::vector<std::string> string_num = { "string" };
    std::vector<std::string> type_first = EXP_NINE();
    bool checker = false;
    while (lexem_->GetValue() == ">=" || lexem_->GetValue() == "<=" || lexem_->GetValue() == "<" || lexem_->GetValue() == ">") {
        checker = true;
        GetLex();
        std::vector<std::string> type_second = EXP_NINE();
        if (!(type_first == type_second && (type_first == integer || type_first == float_num || type_first == string_num))) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " found type mismatch";
        }
    }
    if (!checker) return type_first;
    else return { "bool" };
}

std::vector<std::string> SemanticsAnalyzer::EXP_NINE() {
    std::vector<std::string> integer = { "int" };
    std::vector<std::string> float_num = { "double" };
    std::vector<std::string> string_num = { "string" };
    std::vector<std::string> type_first = EXP_TEN();
    while (lexem_->GetValue() == ">>" || lexem_->GetValue() == "<<") {
        GetLex();
        std::vector<std::string> type_second = EXP_TEN();
        if (!(type_first == type_second && (type_first == integer || type_first == float_num || type_first == string_num))) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " found type mismatch";
        }
    }
    return type_first;
}

std::vector<std::string> SemanticsAnalyzer::EXP_TEN() {
    std::vector<std::string> integer = { "int" };
    std::vector<std::string> float_num = { "double" };
    std::vector<std::string> string_num = { "string" };
    std::vector<std::string> type_first = EXP_ELEVEN();
    bool checker = false;
    while (lexem_->GetValue() == "+" || lexem_->GetValue() == "-") {
        checker = true;
        if (lexem_->GetValue() != "+") {
            GetLex();
            std::vector<std::string> type_second = EXP_TWELVE();
            if (!(type_first == type_second && (type_first == integer || type_first == float_num || type_first == string_num))) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
        else {
            GetLex();
            std::vector<std::string> type_second = EXP_TWELVE();
            if (!(type_first == type_second && type_first == integer || type_first == float_num)) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
    }
    return type_first;
}

std::vector<std::string> SemanticsAnalyzer::EXP_ELEVEN() {
    std::vector<std::string> integer = { "int" };
    std::vector<std::string> float_num = { "double" };
    std::vector<std::string> type_first = EXP_TWELVE();
    bool checker = false;
    while (lexem_->GetValue() == "*" || lexem_->GetValue() == "/" || lexem_->GetValue() == "%") {
        checker = true;
        if (lexem_->GetValue() != "%") {
            GetLex();
            std::vector<std::string> type_second = EXP_TWELVE();
            if (!(type_first == type_second && (type_first == integer || type_first == float_num))) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
        else {
            GetLex();
            std::vector<std::string> type_second = EXP_TWELVE();
            if (!(type_first == type_second && type_first == integer)) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " found type mismatch";
            }
        }
    }
    return type_first;
}

std::vector<std::string> SemanticsAnalyzer::EXP_TWELVE() {
    bool checker = false;
    std::string sign;
    while (lexem_->GetValue() == "+" || lexem_->GetValue() == "-" || lexem_->GetValue() == "++"
        || lexem_->GetValue() == "--" || lexem_->GetValue() == "!") {
        checker = true;
        sign = lexem_->GetValue();
        GetLex();
    }
    std::vector<std::string> type = EXP_THIRTEEN();
    std::vector<std::string> integer = { "int" };
    std::vector<std::string> bool_num = { "bool" };
    std::vector<std::string> float_num = { "double" };
    if (checker && (sign == "--" || sign == "++")) {
        if (type != integer) {
            std::string error = "";
            for (int i = 0; i < (int)type.size(); ++i) {
                error += type[i] + " ";
            }
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected integer, but found " + error;
        }
        else return { "int" };
    } else if (checker && (sign == "-" || sign == "+")) {
        if (type != integer && type != float_num) {
            std::string error = "";
            for (int i = 0; i < (int)type.size(); ++i) {
                error += type[i] + " ";
            }
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected integer, but found " + error;
        }
        else return type;
    } else if (checker) {
        if (type != bool_num) {
            std::string error = "";
            for (int i = 0; i < (int)type.size(); ++i) {
                error += type[i] + " ";
            }
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected integer, but found " + error;
        }
        else return { "bool" };
    }
    else return type;
}

std::vector<std::string> SemanticsAnalyzer::EXP_THIRTEEN() {
    std::vector<std::string> type = EXP_FOURTEEN();
    std::vector<std::string> integer = { "int" };
    int count_indexation = 0;
    if (lexem_->GetValue() == "[") {
        while (lexem_->GetValue() == "[") {
            count_indexation++;
            GetLex(); // [
            std::vector<std::string> index_type = EXP_ZERO();
            if (index_type != integer) {
                std::string error = "";
                for (int i = 0; i < (int)type.size(); ++i) {
                    error += type[i] + " ";
                }
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " expected integer, but found " + error;
            }
            GetLex(); // ]
        }
        for (int i = 0; i < (int)type.size(); ++i) {
            if (type[i] == "array") {
                count_indexation--;
            }
        }
        if (count_indexation > 0) {
            std::string error = "";
            for (int i = 0; i < (int)type.size(); ++i) {
                error += type[i] + " ";
            }
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected more multidimensional array, but found " + error;
        }
        std::vector<std::string> new_type;
        for (int i = 0; i < count_indexation; ++i) {
            new_type.push_back("array");
        }
        new_type.push_back(type[(int)type.size() - 1]);
        type = new_type;
    }
    while (lexem_->GetValue() == "++" || lexem_->GetValue() == "--") {
        if (type != integer) {
            std::string error = "";
            for (int i = 0; i < (int)type.size(); ++i) {
                error += type[i] + " ";
            }
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected integer, but found " + error;
        }
        GetLex();
    }
    return type;
}

std::vector<std::string> SemanticsAnalyzer::EXP_FOURTEEN() {
    if (lexem_->GetType() == LexemType::Identifier) {
        current_id_name_ = lexem_->GetValue();
        if (!table_id_.CheckUsingID(current_id_name_)) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " ID was not declared";
        }
        else if (lex_index_ < (int)list_of_lexems_.size() && list_of_lexems_[lex_index_]->GetValue() == "(") {
            GetLex(); // id
            return FUNC_CALL(current_id_name_);
        }
        else {
            GetLex();
            return table_id_.GetType(current_id_name_).first;
        }
        while (lexem_->GetValue() == "::") {
            GetLex();
            return ID();
        }
    } else {
        return TERM();
    }
}

void SemanticsAnalyzer::LOOP(std::string func_name) {
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

void SemanticsAnalyzer::LOOP_WHILE(std::string func_name) {
    GetLex(); // (
    std::vector<std::string> type;
    do {
        if (lexem_->GetValue() == ",") {
            GetLex();
        }
        type = EXP_ONE();
    } while (lexem_->GetValue() == ",");
    std::vector<std::string> bool_num = { "bool" };
    if (type != bool_num) {
        std::string error = "";
        for (int i = 0; i < (int)type.size(); ++i) {
            error += type[i] + " ";
        }
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected bool, but found " + error;
    }
    GetLex(); // )
    table_id_.AddTable();
    BODY(func_name);
    table_id_.RemoveTable();
}

void SemanticsAnalyzer::LOOP_FOREACH(std::string func_name) {
    GetLex(); // (
    current_id_name_ = lexem_->GetValue();
    if (!table_id_.CheckUsingID(current_id_name_)) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " ID was not declared";
    }
    GetLex(); // id
    std::vector<std::string> id_type = table_id_.GetType(current_id_name_).first;
    std::vector<std::string> exp_type;
    GetLex(); // ;
    do {
        if (lexem_->GetValue() == ",") {
            GetLex();
        }
        exp_type = EXP_ONE();
    } while (lexem_->GetValue() == ",");
    if ((int)exp_type.size() <= (int)id_type.size() || (int)exp_type.size() - (int)id_type.size() > 1) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " found type mismatch";
    }
    while ((int)exp_type.size() > (int)id_type.size()) {
        exp_type.erase(exp_type.begin());
    }
    if (exp_type != id_type) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " found type mismatch";
    }
    GetLex(); // )
    table_id_.AddTable();
    BODY(func_name);
    table_id_.RemoveTable();
}

void SemanticsAnalyzer::LOOP_FOR(std::string func_name) {
    table_id_.AddTable();
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
        if (table_id_.CheckAddingID(current_id_name_)) {
            table_id_.AddID({ current_type_ }, current_id_name_);
        }
        else {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " the name is already used";
        }
        GetLex();
        VARS_MDEF(current_type_.first);
        GetLex(); // ;
    }
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
            type = EXP_ONE();
        } while (lexem_->GetValue() == ",");
        if (type != bool_num) {
            std::string error = "";
            for (int i = 0; i < (int)type.size(); ++i) {
                error += type[i] + " ";
            }
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected bool, but found " + error;
        }
        GetLex(); // ;
    }
    //third part
    if (lexem_->GetValue() == ")") {
        GetLex();
    }
    else {
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            EXP_ONE();
        } while (lexem_->GetValue() == ",");
        GetLex(); // )
    }
    BODY(func_name);
    table_id_.RemoveTable();
}
