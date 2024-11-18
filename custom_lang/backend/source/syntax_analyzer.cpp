#include "../includes/syntax_analyzer.h"
#include <fstream>

void SyntaxAnalyzer::GetLex() {
    if (lex_index_ < (int)list_of_lexems_.size()) {
        lexem_ = list_of_lexems_[lex_index_];
        ++lex_index_;
    } else {
        ++lex_index_;
    }
}

void SyntaxAnalyzer::Analyze(std::vector<Lexem*> list_of_lexems) {
    list_of_lexems_ = list_of_lexems;
    GetLex();
    PROGRAM();
}

void SyntaxAnalyzer::PROGRAM() {
    while (true) {
        if (!(lexem_->GetValue() == "int" || lexem_->GetValue() == "double" ||
            lexem_->GetValue() == "bool" || lexem_->GetValue() == "char" ||
            lexem_->GetValue() == "array" || lexem_->GetValue() == "string")) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected type, but found " + lexem_->GetValue();
        }
        GetLex();
        if (lexem_->GetValue() == "main" && list_of_lexems_[lex_index_ - 2]->GetValue() == "int") break;
        else if (lexem_->GetValue() == "main" && list_of_lexems_[lex_index_ - 2]->GetValue() != "int") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected int before main, but found " + list_of_lexems_[lex_index_ - 2]->GetValue();
        }
        if (lexem_->GetType() != LexemType::Identifier) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected identifier, but found " + lexem_->GetValue();
        }
        GetLex();
        if (lexem_->GetValue() == "(") {
            GetLex();
            FUNC();
        } else {
            VARS_MDEF();
            GetLex();
        }
    }
    if (lexem_->GetValue() != "main") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected utility, but found " + lexem_->GetValue();
    }
    GetLex();
    if (lexem_->GetValue() != "(") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected bracket, but found " + lexem_->GetValue();
    }
    GetLex();
    if (lexem_->GetValue() != ")") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected bracket, but found " + lexem_->GetValue();
    }
    GetLex();
    BODY();
    if (lex_index_ <= list_of_lexems_.size()) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected EOF, but found " + lexem_->GetValue();
    }
}

void SyntaxAnalyzer::VARS_MDEF() {
    if (lexem_->GetValue() == ";") {
        return;
    }
    if (lexem_->GetValue() == "=") {
        GetLex();
        if (lexem_->GetValue() != "{") {
            EXP_ZERO();
        } else {
            GetLex();
            EXP_ZERO();
            if (lexem_->GetValue() != "}") {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " expected }, but found " + lexem_->GetValue();
            }
            GetLex();
        }
    }
    while (lexem_->GetValue() == ",") {
        GetLex();
        if (lexem_->GetType() != LexemType::Identifier) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected identifier, but found " + lexem_->GetValue();
        }
        GetLex();
        if (lexem_->GetValue() == "=") {
            GetLex();
            if (lexem_->GetValue() != "{") {
                EXP_ZERO();
            } else {
                GetLex();
                EXP_ZERO();
                if (lexem_->GetValue() != "}") {
                    throw "line: " + std::to_string(lexem_->GetLine()) +
                        " column: " + std::to_string(lexem_->GetColumn() - 1) +
                        " expected }, but found " + lexem_->GetValue();
                }
                GetLex();
            }
        }
    }
    if (lexem_->GetValue() != ";") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected ;, but found " + lexem_->GetValue();
    }
}

/* Daft FUNC XDDD */
void SyntaxAnalyzer::FUNC() {
    if (lexem_->GetValue() == "void") {
        GetLex();
        if (lexem_->GetValue() != ")") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected ), but found " + lexem_->GetValue();
        }
        GetLex();
    } else if (lexem_->GetValue() == "int" || lexem_->GetValue() == "double" ||
        lexem_->GetValue() == "bool" || lexem_->GetValue() == "char" ||
        lexem_->GetValue() == "array" || lexem_->GetValue() == "string") {
        GetLex();
        if (lexem_->GetType() != LexemType::Identifier) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected identifier, but found " + lexem_->GetValue();
        }
        GetLex();
        while (lexem_->GetValue() == ",") {
            GetLex();
            if (!(lexem_->GetValue() == "int" || lexem_->GetValue() == "double" ||
                lexem_->GetValue() == "bool" || lexem_->GetValue() == "char" ||
                lexem_->GetValue() == "array" || lexem_->GetValue() == "string")) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " expected type, but found " + lexem_->GetValue();
            }
            GetLex();
            if (lexem_->GetType() != LexemType::Identifier) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " expected identifier, but found " + lexem_->GetValue();
            }
            GetLex();
        }
        if (lexem_->GetValue() != ")") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected ), but found " + lexem_->GetValue();
        }
        GetLex();
    } else if (lexem_->GetValue() == ")") {
        GetLex();
    } else {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected ), but found " + lexem_->GetValue();
    }
    if (lexem_->GetValue() == ";") {
        GetLex();
    } else if (lexem_->GetValue() == "{") {
        BODY();
    }
}

void SyntaxAnalyzer::BODY() {
    if (lexem_->GetValue() != "{") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected {, but found " + lexem_->GetValue();
    }
    GetLex();
    while (lexem_->GetValue() != "}") {
        STATEMENT();
        if (lex_index_ >= (int)list_of_lexems_.size() && lexem_->GetValue() != "}") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected }, but found " + lexem_->GetValue();
        }
    }
    GetLex();
}

void SyntaxAnalyzer::STATEMENT() {
    if (lexem_->GetValue() == "int" || lexem_->GetValue() == "double" ||
        lexem_->GetValue() == "bool" || lexem_->GetValue() == "char" ||
        lexem_->GetValue() == "array" || lexem_->GetValue() == "string") {
        GetLex();
        if (lexem_->GetType() != LexemType::Identifier) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected identifier, but found " + lexem_->GetValue();
        }
        GetLex();
        VARS_MDEF();
        GetLex();
    } else if (lexem_->GetType() == LexemType::Identifier) {
        if (lex_index_ < (int)list_of_lexems_.size() && list_of_lexems_[lex_index_]->GetValue() == "(") {
            GetLex();
            FUNC_CALL();
            GetLex();
        } else {
            if (lex_index_ < (int)list_of_lexems_.size() && (list_of_lexems_[lex_index_]->GetValue() == "=")) {
                GetLex();
                VARS_MDEF();
            } else {
                EXP_ZERO();
            }
            if (lexem_->GetValue() != ";") {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " expected ;, but found " + lexem_->GetValue();
            }
            GetLex();
        }
    } else if (lexem_->GetValue() == "if") {
        GetLex();
        IF();
    } else if (lexem_->GetValue() == "switch") {
        GetLex();
        SWITCH();
    } else if (lexem_->GetValue() == "loop") {
        GetLex();
        LOOP();
    } else if (lexem_->GetValue() == "break") {
        GetLex();
        if (lexem_->GetValue() != ";") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected ;, but found " + lexem_->GetValue();
        }
        GetLex();
    } else if (lexem_->GetValue() == "continue") {
        GetLex();
        if (lexem_->GetValue() != ";") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected ;, but found " + lexem_->GetValue();
        }
        GetLex();
    } else if (lexem_->GetValue() == "cout") {
        GetLex();
        OUTPUT();
        GetLex();
    } else if (lexem_->GetValue() == "cin") {
        GetLex();
        INPUT();
        GetLex();
    } else if (lexem_->GetValue() == "return") {
        GetLex();
        if (lexem_->GetValue() == ";") {
            GetLex();
        } else {
            EXP_ZERO();
            if (lexem_->GetValue() != ";") {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " expected ;, but found " + lexem_->GetValue();
            }
            GetLex();
        }
    } else {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected statement or return, but found " + lexem_->GetValue();
    }
}

void SyntaxAnalyzer::INPUT() {
    if (lexem_->GetValue() != ">>") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected >>, but found " + lexem_->GetValue();
    }
    GetLex();
    if (lexem_->GetType() != LexemType::Identifier) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected ID, but found " + lexem_->GetValue();
    }
    GetLex();
    while (lexem_->GetValue() == ">>") {
        if (lexem_->GetValue() != ">>") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected >>, but found " + lexem_->GetValue();
        }
        GetLex();
        if (lexem_->GetType() != LexemType::Identifier) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected ID, but found " + lexem_->GetValue();
        }
        GetLex();
    }
    if (lexem_->GetValue() != ";") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected ;, but found " + lexem_->GetValue();
    }
}

void SyntaxAnalyzer::OUTPUT() {
    if (lexem_->GetValue() != "<<") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected >>, but found " + lexem_->GetValue();
    }
    GetLex();
    if (lexem_->GetType() != LexemType::Identifier && lexem_->GetType() != LexemType::String &&
        lexem_->GetType() != LexemType::Integer && lexem_->GetType() != LexemType::Float) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected literal or ID, but found " + lexem_->GetValue();
    } else {
        EXP_ZERO();
    }
    while (lexem_->GetValue() == "<<") {
        if (lexem_->GetValue() != "<<") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected >>, but found " + lexem_->GetValue();
        }
        GetLex();
        if (lexem_->GetType() != LexemType::Identifier && lexem_->GetType() != LexemType::String &&
            lexem_->GetType() != LexemType::Integer && lexem_->GetType() != LexemType::Float) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected literal or ID, but found " + lexem_->GetValue();
        } else {
            EXP_ZERO();
        }
    }
    if (lexem_->GetValue() != ";") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected ;, but found " + lexem_->GetValue();
    }
}

void SyntaxAnalyzer::FUNC_CALL() {
    GetLex();
    while (lexem_->GetType() == LexemType::Identifier || lexem_->GetType() == LexemType::String ||
        lexem_->GetType() == LexemType::Integer || lexem_->GetType() == LexemType::Float) {
        GetLex();
        if (lexem_->GetValue() == ")") {
            break;
        } else if (lexem_->GetValue() == ",") {
            GetLex();
        } else {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected , or ), but found " + lexem_->GetValue();
        }
    }
    if (lexem_->GetValue() != ")") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected ), but found " + lexem_->GetValue();
    }
    GetLex();
    if (lexem_->GetValue() != ";") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected ;, but found " + lexem_->GetValue();
    }
}

void SyntaxAnalyzer::IF() {
    if (lexem_->GetValue() != "(") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected (, but found " + lexem_->GetValue();
    }
    GetLex();
    EXP_ZERO();
    if (lexem_->GetValue() != ")") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected ), but found " + lexem_->GetValue();
    }
    GetLex();
    BODY();
    while (lexem_->GetValue() == "elif") {
        GetLex();
        if (lexem_->GetValue() != "(") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected (, but found " + lexem_->GetValue();
        }
        GetLex();
        EXP_ZERO();
        if (lexem_->GetValue() != ")") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected ), but found " + lexem_->GetValue();
        }
        GetLex();
        BODY();
    }
    if (lexem_->GetValue() == "else") {
        GetLex();
        BODY();
    }
}

void SyntaxAnalyzer::SWITCH() {
    if (lexem_->GetValue() != "(") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected (, but found " + lexem_->GetValue();
    }
    GetLex();
    EXP_ZERO();
    if (lexem_->GetValue() != ")") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected ), but found " + lexem_->GetValue();
    }
    GetLex();
    if (lexem_->GetValue() != "{") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected {, but found " + lexem_->GetValue();
    }
    GetLex();
    while (lexem_->GetValue() == "case") {
        GetLex();
        EXP_ZERO();
        if (lexem_->GetValue() != ":") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected :, but found " + lexem_->GetValue();
        }
        GetLex();
        while (lexem_->GetValue() != "case" && lexem_->GetValue() != "default" && lexem_->GetValue() != "}") {
            STATEMENT();
        }
    }
    if (lexem_->GetValue() == "default") {
        GetLex();
        if (lexem_->GetValue() != ":") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected :, but found " + lexem_->GetValue();
        }
        GetLex();
        while (lexem_->GetValue() != "}") {
            STATEMENT();
        }
    }
    if (lexem_->GetValue() != "}") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected }, but found " + lexem_->GetValue();
    }
    GetLex();
}

void SyntaxAnalyzer::ID() {
    if (lexem_->GetType() != LexemType::Identifier) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected identifier, but found " + lexem_->GetValue();
    }
    GetLex();
}

void SyntaxAnalyzer::TERM() {
    if (lexem_->GetType() != LexemType::Integer && lexem_->GetType() != LexemType::Float &&
        lexem_->GetType() != LexemType::String) {
        if (lexem_->GetValue() != "(") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected (, but found " + lexem_->GetValue();
        }
        GetLex();
        EXP_ZERO();
        if (lexem_->GetValue() != ")") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected ), but found " + lexem_->GetValue();
        }
        GetLex();
    } else GetLex();
}

void SyntaxAnalyzer::EXP_ZERO() {
    EXP_ONE();
    while (lexem_->GetValue() == ",") {
        GetLex();
        EXP_ONE();
    }
}

void SyntaxAnalyzer::EXP_ONE() {
    EXP_TWO();
    while (lexem_->GetValue() == "+=" || lexem_->GetValue() == "-=" || lexem_->GetValue() == "*=" || lexem_->GetValue() == "/=" ||
        lexem_->GetValue() == "%=" || lexem_->GetValue() == "=" || lexem_->GetValue() == ">>=" || lexem_->GetValue() == "<<=" ||
        lexem_->GetValue() == "|=" || lexem_->GetValue() == "&=" || lexem_->GetValue() == "^=") {
        GetLex();
        EXP_TWO();
    }
}

void SyntaxAnalyzer::EXP_TWO() {
    EXP_THREE();
    while (lexem_->GetValue() == "||") {
        GetLex();
        EXP_THREE();
    }
}

void SyntaxAnalyzer::EXP_THREE() {
    EXP_FOUR();
    while (lexem_->GetValue() == "&&") {
        GetLex();
        EXP_FOUR();
    }
}

void SyntaxAnalyzer::EXP_FOUR() {
    EXP_FIVE();
    while (lexem_->GetValue() == "|") {
        GetLex();
        EXP_FIVE();
    }
}

void SyntaxAnalyzer::EXP_FIVE() {
    EXP_SIX();
    while (lexem_->GetValue() == "^") {
        GetLex();
        EXP_SIX();
    }
}

void SyntaxAnalyzer::EXP_SIX() {
    EXP_SEVEN();
    while (lexem_->GetValue() == "&") {
        GetLex();
        EXP_SEVEN();
    }
}

void SyntaxAnalyzer::EXP_SEVEN() {
    EXP_EIGHT();
    while (lexem_->GetValue() == "==" || lexem_->GetValue() == "!=") {
        GetLex();
        EXP_EIGHT();
    }
}

void SyntaxAnalyzer::EXP_EIGHT() {
    EXP_NINE();
    while (lexem_->GetValue() == ">=" || lexem_->GetValue() == "<=" || lexem_->GetValue() == "<" || lexem_->GetValue() == ">") {
        GetLex();
        EXP_NINE();
    }
}

void SyntaxAnalyzer::EXP_NINE() {
    EXP_TEN();
    while (lexem_->GetValue() == ">>" || lexem_->GetValue() == "<<") {
        GetLex();
        EXP_TEN();
    }
}

void SyntaxAnalyzer::EXP_TEN() {
    EXP_ELEVEN();
    while (lexem_->GetValue() == "+" || lexem_->GetValue() == "-") {
        GetLex();
        EXP_ELEVEN();
    }
}

void SyntaxAnalyzer::EXP_ELEVEN() {
    EXP_TWELVE();
    while (lexem_->GetValue() == "*" || lexem_->GetValue() == "/" || lexem_->GetValue() == "%") {
        GetLex();
        EXP_TWELVE();
    }
}

void SyntaxAnalyzer::EXP_TWELVE() {
    while (lexem_->GetValue() == "+" || lexem_->GetValue() == "-" || lexem_->GetValue() == "++"
        || lexem_->GetValue() == "--" || lexem_->GetValue() == "!") {
        GetLex();
    }
    EXP_THIRTEEN();
}

void SyntaxAnalyzer::EXP_THIRTEEN() {
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

void SyntaxAnalyzer::EXP_FOURTEEN() {
    if (lexem_->GetType() == LexemType::Identifier) {
        GetLex();
        while (lexem_->GetValue() == "::") {
            GetLex();
            if (lexem_->GetType() != LexemType::Identifier) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " expected ID, but found " + lexem_->GetValue();
            } else {
                ID();
            }
        }
    } else {
        TERM();
    }
}

void SyntaxAnalyzer::LOOP() {
    if (lexem_->GetValue() == "for") {
        GetLex();
        LOOP_FOR();
    } else if (lexem_->GetValue() == "foreach") {
        GetLex();
        LOOP_FOREACH();
    } else if (lexem_->GetValue() == "while") {
        GetLex();
        LOOP_WHILE();
    } else {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected loop type, but found " + lexem_->GetValue();
    }
}

void SyntaxAnalyzer::LOOP_WHILE() {
    if (lexem_->GetValue() != "(") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected (, but found " + lexem_->GetValue();
    }
    GetLex();
    EXP_ZERO();
    if (lexem_->GetValue() != ")") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected ), but found " + lexem_->GetValue();
    }
    GetLex();
    BODY();
}

void SyntaxAnalyzer::LOOP_FOREACH() {
    if (lexem_->GetValue() != "(") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected (, but found " + lexem_->GetValue();
    }
    GetLex();
    if (lexem_->GetType() != LexemType::Identifier) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected ID, but found " + lexem_->GetValue();
    }
    GetLex();
    if (lexem_->GetValue() != ";") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected ;, but found " + lexem_->GetValue();
    }
    GetLex();
    EXP_ZERO();
    if (lexem_->GetValue() != ")") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected ), but found " + lexem_->GetValue();
    }
    GetLex();
    BODY();
}

void SyntaxAnalyzer::LOOP_FOR() {
    if (lexem_->GetValue() != "(") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected (, but found " + lexem_->GetValue();
    }
    GetLex();
    // first part
    if (lexem_->GetValue() == ";") {
        GetLex();
    } else if (lexem_->GetType() == LexemType::Type) {
        GetLex();
        if (lexem_->GetType() != LexemType::Identifier) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected ID, but found " + lexem_->GetValue();
        }
        GetLex();
        VARS_MDEF();
        if (lexem_->GetValue() != ";") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected ;, but found " + lexem_->GetValue();
        }
        GetLex();
    } else {
        EXP_ZERO();
        GetLex();
        if (lexem_->GetValue() != ";") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected ;, but found " + lexem_->GetValue();
        }
        GetLex();
    }
    // second part
    if (lexem_->GetValue() == ";") {
        GetLex();
    } else {
        EXP_ZERO();
        if (lexem_->GetValue() != ";") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected ;, but found " + lexem_->GetValue();
        }
        GetLex();
    }
    // third part
    if (lexem_->GetValue() == ")") {
        GetLex();
    } else {
        EXP_ZERO();
        if (lexem_->GetValue() != ")") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected ), but found " + lexem_->GetValue();
        }
        GetLex();
    }
    BODY();
}
