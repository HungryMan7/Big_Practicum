#include "semantic_analyzer.h"
#include <fstream>

SemanticAnalyzer::SemanticAnalyzer(std::string file_name) {
    la = new LexicalAnalyzer(file_name);
    la->SetLanguage("utilities.txt");
    la->Analyze();
    list_of_lexems_ = la->GetLexems();
}

SemanticAnalyzer::~SemanticAnalyzer() {
    delete la;
}

void SemanticAnalyzer::GetLex() {
    if (lex_index_ < (int)list_of_lexems_.size()) {
        lexem_ = list_of_lexems_[lex_index_];
        ++lex_index_;
    }
    else {
        ++lex_index_;
    }
}

void SemanticAnalyzer::PROGRAM() {
    while (3 + 5 == 8) {
        current_type_ = lexem_->GetValue();
        GetLex(); // type
        if (lexem_->GetValue() == "main" && list_of_lexems_[lex_index_ - 2]->GetValue() == "int") break;
        current_id_name_ = lexem_->GetValue();
        GetLex(); // id
        if (lexem_->GetValue() == "(") {
            GetLex();
            if (table_id_.CheckAddingID(current_id_name_)) {
                table_id_.AddID({ "function" , current_type_ }, current_id_name_);
            }
            else {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " the name is already used";
            }
            table_id_.AddTable();
            FUNC();
            table_id_.RemoveTable();
        }
        else {
            if (table_id_.CheckAddingID(current_id_name_)) {
                table_id_.AddID({ current_type_ }, current_id_name_);
            }
            else {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " the name is already used";
            }
            VARS_MDEF();
            GetLex();
        }
    }
    current_id_name_ = lexem_->GetValue();
    if (table_id_.CheckAddingID(current_id_name_)) {
        table_id_.AddID({ "function" , current_type_ }, current_id_name_);
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
    BODY();
    table_id_.RemoveTable();
}

void SemanticAnalyzer::VARS_MDEF() {
    if (lexem_->GetValue() == ";") {
        return;
    }
    if (lexem_->GetValue() == "=") {
        GetLex();
        if (lexem_->GetValue() != "{") {
            EXP_ONE();
        }
        else {
            GetLex();
            do {
                EXP_ONE();
            } while (lexem_->GetValue() == ",");
            GetLex(); // }
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
        if (lexem_->GetValue() == "=") {
            GetLex();
            if (lexem_->GetValue() != "{") {
                EXP_ONE();
            }
            else {
                GetLex();
                do {
                    EXP_ONE();
                } while (lexem_->GetValue() == ",");
                GetLex(); // }
            }
        }
    }
}

void SemanticAnalyzer::FUNC() {
    if (lexem_->GetValue() == "void") {
        GetLex(); // void
        GetLex(); // )
    }
    else if (lexem_->GetType() == LexemType::Type) {
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
        while (lexem_->GetValue() == ",") {
            GetLex();
            GetLex(); // type
            current_id_name_ = lexem_->GetValue();
            if (table_id_.CheckAddingID(current_id_name_)) {
                table_id_.AddID({ current_type_ }, current_id_name_);
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
        BODY();
    }
}

void SemanticAnalyzer::BODY() {
    GetLex(); // {
    while (lexem_->GetValue() != "}") {
        STATEMENT();
    }
    GetLex(); // }
}

void SemanticAnalyzer::STATEMENT() {
    if (lexem_->GetType() == LexemType::Type) {
        current_type_ = lexem_->GetValue();
        GetLex(); // type
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
        VARS_MDEF();
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
            VARS_MDEF();
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
        IF();
    }
    else if (lexem_->GetValue() == "switch") {
        GetLex();
        SWITCH();
    }
    else if (lexem_->GetValue() == "loop") {
        GetLex();
        LOOP();
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
            GetLex();
        }
        else {
            do {
                if (lexem_->GetValue() == ",") {
                    GetLex();
                }
                EXP_ONE();
            } while (lexem_->GetValue() == ",");
            GetLex(); // ;
        }
    }
}

void SemanticAnalyzer::INPUT() {
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

void SemanticAnalyzer::OUTPUT() {
    GetLex(); // <<
    EXP_ZERO();
    while (lexem_->GetValue() == "<<") {
        GetLex(); // <<
        EXP_ZERO();
    }
}

void SemanticAnalyzer::FUNC_CALL() {
    GetLex(); // (
    while (lexem_->GetType() == LexemType::Identifier || lexem_->GetType() == LexemType::String ||
        lexem_->GetType() == LexemType::Integer || lexem_->GetType() == LexemType::Float) {
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            EXP_ONE();
        } while (lexem_->GetValue() == ",");
        GetLex(); // )
    }
}

void SemanticAnalyzer::IF() {
    GetLex(); // (
    do {
        if (lexem_->GetValue() == ",") {
            GetLex();
        }
        EXP_ONE();
    } while (lexem_->GetValue() == ",");
    GetLex(); // )
    table_id_.AddTable();
    BODY();
    table_id_.RemoveTable();
    while (lexem_->GetValue() == "elif") {
        GetLex(); // elif
        GetLex(); // (
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            EXP_ONE();
        } while (lexem_->GetValue() == ",");
        GetLex(); // )
        table_id_.AddTable();
        BODY();
        table_id_.RemoveTable();
    }
    if (lexem_->GetValue() == "else") {
        GetLex(); // else
        table_id_.AddTable();
        BODY();
        table_id_.RemoveTable();
    }
}

void SemanticAnalyzer::SWITCH() {
    GetLex(); // (
    do {
        if (lexem_->GetValue() == ",") {
            GetLex();
        }
        EXP_ONE();
    } while (lexem_->GetValue() == ",");
    GetLex(); // )
    GetLex(); // {
    while (lexem_->GetValue() == "case") {
        GetLex();
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            EXP_ONE();
        } while (lexem_->GetValue() == ",");
        GetLex(); // :
        while (lexem_->GetValue() != "case" && lexem_->GetValue() != "default" && lexem_->GetValue() != "}") {
            table_id_.AddTable();
            STATEMENT();
            table_id_.RemoveTable();
        }
    }
    if (lexem_->GetValue() == "default") {
        GetLex();
        GetLex(); // :
        while (lexem_->GetValue() != "}") {
            table_id_.AddTable();
            STATEMENT();
            table_id_.RemoveTable();
        }
    }
    GetLex(); // }
}

void SemanticAnalyzer::ID() {
    current_id_name_ = lexem_->GetValue();
    if (!table_id_.CheckUsingID(current_id_name_)) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " ID was not declared";
    }
    GetLex();
}

void SemanticAnalyzer::TERM() {
    if (lexem_->GetType() != LexemType::Integer && lexem_->GetType() != LexemType::Float &&
        lexem_->GetType() != LexemType::String) {
        GetLex(); // (
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            EXP_ONE();
        } while (lexem_->GetValue() == ",");
        GetLex(); // )
    }
    else GetLex();
}

void SemanticAnalyzer::EXP_ZERO() {
    EXP_ONE();
    while (lexem_->GetValue() == ",") {
        GetLex();
        EXP_ONE();
    }
}

void SemanticAnalyzer::EXP_ONE() {
    EXP_TWO();
    while (lexem_->GetValue() == "+=" || lexem_->GetValue() == "-=" || lexem_->GetValue() == "*=" || lexem_->GetValue() == "/=" ||
        lexem_->GetValue() == "%=" || lexem_->GetValue() == "=" || lexem_->GetValue() == ">>=" || lexem_->GetValue() == "<<=" ||
        lexem_->GetValue() == "|=" || lexem_->GetValue() == "&=" || lexem_->GetValue() == "^=") {
        GetLex();
        EXP_TWO();
    }
}

void SemanticAnalyzer::EXP_TWO() {
    EXP_THREE();
    while (lexem_->GetValue() == "||") {
        GetLex();
        EXP_THREE();
    }
}

void SemanticAnalyzer::EXP_THREE() {
    EXP_FOUR();
    while (lexem_->GetValue() == "&&") {
        GetLex();
        EXP_FOUR();
    }
}

void SemanticAnalyzer::EXP_FOUR() {
    EXP_FIVE();
    while (lexem_->GetValue() == "|") {
        GetLex();
        EXP_FIVE();
    }
}

void SemanticAnalyzer::EXP_FIVE() {
    EXP_SIX();
    while (lexem_->GetValue() == "^") {
        GetLex();
        EXP_SIX();
    }
}

void SemanticAnalyzer::EXP_SIX() {
    EXP_SEVEN();
    while (lexem_->GetValue() == "&") {
        GetLex();
        EXP_SEVEN();
    }
}

void SemanticAnalyzer::EXP_SEVEN() {
    EXP_EIGHT();
    while (lexem_->GetValue() == "==" || lexem_->GetValue() == "!=") {
        GetLex();
        EXP_EIGHT();
    }
}

void SemanticAnalyzer::EXP_EIGHT() {
    EXP_NINE();
    while (lexem_->GetValue() == ">=" || lexem_->GetValue() == "<=" || lexem_->GetValue() == "<" || lexem_->GetValue() == ">") {
        GetLex();
        EXP_NINE();
    }
}

void SemanticAnalyzer::EXP_NINE() {
    EXP_TEN();
    /*while (lexem_->GetValue() == ">>" || lexem_->GetValue() == "<<") {
        GetLex();
        EXP_TEN();
    }*/
}

void SemanticAnalyzer::EXP_TEN() {
    EXP_ELEVEN();
    while (lexem_->GetValue() == "+" || lexem_->GetValue() == "-") {
        GetLex();
        EXP_ELEVEN();
    }
}

void SemanticAnalyzer::EXP_ELEVEN() {
    EXP_TWELVE();
    while (lexem_->GetValue() == "*" || lexem_->GetValue() == "/" || lexem_->GetValue() == "%") {
        GetLex();
        EXP_TWELVE();
    }
}

void SemanticAnalyzer::EXP_TWELVE() {
    while (lexem_->GetValue() == "+" || lexem_->GetValue() == "-" || lexem_->GetValue() == "++"
        || lexem_->GetValue() == "--" || lexem_->GetValue() == "!") {
        GetLex();
    }
    EXP_THIRTEEN();
}

void SemanticAnalyzer::EXP_THIRTEEN() {
    EXP_FOURTEEN();
    while (lexem_->GetValue() == "++" || lexem_->GetValue() == "--") {
        GetLex();
    }
    while (lexem_->GetValue() == "[") {
        GetLex();
        EXP_ZERO();
        if (lexem_->GetValue() != "]") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected ], but found " + lexem_->GetValue();
        }
        GetLex();
    }
}

void SemanticAnalyzer::EXP_FOURTEEN() {
    if (lexem_->GetType() == LexemType::Identifier) {
        current_id_name_ = lexem_->GetValue();
        if (!table_id_.CheckUsingID(current_id_name_)) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " ID was not declared";
        }
        if (lex_index_ < (int)list_of_lexems_.size() && list_of_lexems_[lex_index_]->GetValue() == "(") {
            GetLex();
            FUNC_CALL();
        }
        else {
            GetLex();
        }
        while (lexem_->GetValue() == "::") {
            GetLex();
            ID();
        }
    }
    else {
        TERM();
    }
}

void SemanticAnalyzer::LOOP() {
    if (lexem_->GetValue() == "for") {
        GetLex();
        LOOP_FOR();
    }
    else if (lexem_->GetValue() == "foreach") {
        GetLex();
        LOOP_FOREACH();
    }
    else if (lexem_->GetValue() == "while") {
        GetLex();
        LOOP_WHILE();
    }
}

void SemanticAnalyzer::LOOP_WHILE() {
    GetLex(); // (
    do {
        if (lexem_->GetValue() == ",") {
            GetLex();
        }
        EXP_ONE();
    } while (lexem_->GetValue() == ",");
    GetLex(); // )
    table_id_.AddTable();
    BODY();
    table_id_.RemoveTable();
}

void SemanticAnalyzer::LOOP_FOREACH() {
    GetLex(); // (
    current_id_name_ = lexem_->GetValue();
    if (!table_id_.CheckUsingID(current_id_name_)) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " ID was not declared";
    }
    GetLex();
    GetLex(); // ;
    do {
        if (lexem_->GetValue() == ",") {
            GetLex();
        }
        EXP_ONE();
    } while (lexem_->GetValue() == ",");
    GetLex(); // )
    table_id_.AddTable();
    BODY();
    table_id_.RemoveTable();
}

void SemanticAnalyzer::LOOP_FOR() {
    table_id_.AddTable();
    GetLex(); // (
    // first part
    if (lexem_->GetValue() == ";") {
        GetLex();
    }
    else if (lexem_->GetType() == LexemType::Type) {
        current_type_ = lexem_->GetValue();
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
        GetLex();
        VARS_MDEF();
        GetLex(); // ;
    }
    else {
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            EXP_ONE();
        } while (lexem_->GetValue() == ",");
        GetLex(); // ;
    }
    // second part
    if (lexem_->GetValue() == ";") {
        GetLex();
    }
    else {
        do {
            if (lexem_->GetValue() == ",") {
                GetLex();
            }
            EXP_ONE();
        } while (lexem_->GetValue() == ",");
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
    BODY();
    table_id_.RemoveTable();
}

void SemanticAnalyzer::DeclarationCheck() {
    GetLex();
    PROGRAM();
}

void SemanticAnalyzer::Analyze() {
    DeclarationCheck();
    //TypeCheck();
}