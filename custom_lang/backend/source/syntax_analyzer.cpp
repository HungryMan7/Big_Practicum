#include "../includes/syntax_analyzer.h"
#include <fstream>

SyntaxAnalyzer::SyntaxAnalyzer(std::string file_name) {
    la = new LexicalAnalyzer(file_name);
    la->SetLanguage("custom_lang/resources/lang.txt");
    la->Analyze();
    list_of_lexems_ = la->GetLexems();
}

SyntaxAnalyzer::~SyntaxAnalyzer() {
    delete la;
}

void SyntaxAnalyzer::GetLex() {
    if (lex_index_ >= (int)list_of_lexems_.size()) exit(0);
    lexem_ = list_of_lexems_[lex_index_];
    ++lex_index_;
}

void SyntaxAnalyzer::PROGRAM() {
    if (!(lexem_->GetValue() == "int" || lexem_->GetValue() == "double" ||
        lexem_->GetValue() == "bool" || lexem_->GetValue() == "char" ||
        lexem_->GetValue() == "array" || lexem_->GetValue() == "string")) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected type, but found " + lexem_->GetValue();
    }
    GetLex();
    if (lexem_->GetType() != LexemType::Identifier) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected identifier, but found " + lexem_->GetValue();
    }
    GetLex();
    if (lexem_->GetValue() == "(") {
        GetLex();
        FUNC();
    } else {
        VARS_MDEF();
    }
    while (2 + 2 == 4) {
        if (!(lexem_->GetValue() == "int" || lexem_->GetValue() == "double" ||
            lexem_->GetValue() == "bool" || lexem_->GetValue() == "char" ||
            lexem_->GetValue() == "array" || lexem_->GetValue() == "string")) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected type, but found " + lexem_->GetValue();
        }
        GetLex();
        if (lexem_->GetType() != LexemType::Identifier) {
            throw "line: " + std::to_string(lexem_->GetLine()) + 
                  " column: " + std::to_string(lexem_->GetColumn()) +
                  " expected identifier, but found " + lexem_->GetValue();
        }
        GetLex();
        if (lexem_->GetValue() == "(") {
            GetLex();
            FUNC();
        } else {
            VARS_MDEF();
        }
    }
}

void SyntaxAnalyzer::VARS_MDEF() {
    if (lexem_->GetValue() == "=") {
        GetLex();
        EXP_ZERO();
        GetLex();
    }
    while (lexem_->GetValue() == ",") {
        GetLex();
        if (lexem_->GetType() != LexemType::Identifier) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected identifier, but found " + lexem_->GetValue();
        }
        GetLex();
        if (lexem_->GetValue() == "=") {
            GetLex();
            EXP_ZERO();
        }
    }
    if (lexem_->GetValue() != ";") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected ;, but found " + lexem_->GetValue();
    }
    GetLex();
}

/* Daft FUNC XDDD */
void SyntaxAnalyzer::FUNC() {
    if (lexem_->GetValue() == "void") {
        GetLex();
        if (lexem_->GetValue() != ")") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected ), but found " + lexem_->GetValue();
        }
        GetLex();
    } else if (lexem_->GetValue() == "int" || lexem_->GetValue() == "double" ||
        lexem_->GetValue() == "bool" || lexem_->GetValue() == "char" ||
        lexem_->GetValue() == "array" || lexem_->GetValue() == "string") {
        GetLex();
        if (lexem_->GetType() != LexemType::Identifier) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected identifier, but found " + lexem_->GetValue();
        }
        GetLex();
        while (lexem_->GetValue() == ",") {
            GetLex();
            if (!(lexem_->GetValue() == "int" || lexem_->GetValue() == "double" ||
                lexem_->GetValue() == "bool" || lexem_->GetValue() == "char" ||
                lexem_->GetValue() == "array" || lexem_->GetValue() == "string")) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn()) +
                    " expected type, but found " + lexem_->GetValue();
            }
            GetLex();
            if (lexem_->GetType() != LexemType::Identifier) {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn()) +
                    " expected identifier, but found " + lexem_->GetValue();
            }
            GetLex();
        }
        if (lexem_->GetValue() != ")") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected ), but found " + lexem_->GetValue();
        }
        GetLex();
    } else if (lexem_->GetValue() == ")") {
        GetLex();
    } else {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected ), but found " + lexem_->GetValue();
    }
    if (lexem_->GetValue() == ";") {
        GetLex();
    } else if (lexem_->GetValue() == "{") {
        GetLex();
        BODY();
    }
}

void SyntaxAnalyzer::BODY() {    
    while (lexem_->GetValue() != "}") {
        STATEMENT();
    }
}

// <statement> ::= <vars-mdef> ';' | <ex0> ';' | <if> | <switch> | <loop> | <func-call> ';' | "break" ';' | "continue" ';' | eps
void SyntaxAnalyzer::STATEMENT() {
    // 1.
    if (lexem_->GetValue() == "int" || lexem_->GetValue() == "double" ||
        lexem_->GetValue() == "bool" || lexem_->GetValue() == "char" ||
        lexem_->GetValue() == "array" || lexem_->GetValue() == "string") {
        GetLex();
        VARS_MDEF();
        if (lexem_->GetValue() != ";") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected ;, but found " + lexem_->GetValue();
        }
        GetLex();
    } else if (lexem_->GetType() == LexemType::Identifier) {
        EXP_ZERO();
        if (lexem_->GetValue() != ";") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected ;, but found " + lexem_->GetValue();
        }
        GetLex();
    } else if (lexem_->GetValue() == "if") {
        IF();
        GetLex();
    } else if (lexem_->GetValue() == "switch") {
        SWITCH();
        GetLex();
    } else if (lexem_->GetValue() == "loop") {
        LOOP();
        GetLex();
    } else if (lexem_->GetValue() == "break") {
        GetLex();
        if (lexem_->GetValue() != ";") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected ;, but found " + lexem_->GetValue();
        }
        GetLex();
    } else if (lexem_->GetValue() == "continue") {
        GetLex();
        if (lexem_->GetValue() != ";") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected ;, but found " + lexem_->GetValue();
        }
        GetLex();
    }
}

void SyntaxAnalyzer::FUNC_CALL() {

}

void SyntaxAnalyzer::IF() {

}

void SyntaxAnalyzer::LOOP() {

}

void SyntaxAnalyzer::SWITCH() {

}

void SyntaxAnalyzer::ID() {
    if (lexem_->GetType() != LexemType::Identifier) {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected identifier, but found " + lexem_->GetValue();
    }
    GetLex();
}

void SyntaxAnalyzer::TERM() {
    if (lexem_->GetType() == LexemType::Identifier) {
        GetLex();
        if (lexem_->GetValue() == "(") {
            GetLex();
            if (lexem_->GetType() != LexemType::Identifier && lexem_->GetType() != LexemType::Integer &&
                lexem_->GetType() != LexemType::Float && lexem_->GetType() != LexemType::String && 
                lexem_->GetValue() != ")") {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn()) +
                    " expected values, but found " + lexem_->GetValue();
            }
            while (lexem_->GetValue() == ",") {
                GetLex();
                if (lexem_->GetType() != LexemType::Identifier && lexem_->GetType() != LexemType::Integer && 
                    lexem_->GetType() != LexemType::Float && lexem_->GetType() != LexemType::String) {
                    throw "line: " + std::to_string(lexem_->GetLine()) +
                        " column: " + std::to_string(lexem_->GetColumn()) +
                        " expected values, but found " + lexem_->GetValue();
                }
                GetLex();
            }
            if (lexem_->GetValue() != ")") {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn()) +
                    " expected ), but found " + lexem_->GetValue();
            }
            GetLex();
        }
    } else {
        if (lexem_->GetType() != LexemType::Integer && lexem_->GetType() != LexemType::Float &&
            lexem_->GetType() != LexemType::String) {
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
                    " expected (, but found " + lexem_->GetValue();
            }
        }
    }
}

void SyntaxAnalyzer::EXP_ZERO() {
    EXP_ONE();
    while (lexem_->GetValue() == ",") {
        GetLex();
        EXP_ONE();
    }
}

void SyntaxAnalyzer::EXP_ONE() {
    if (lexem_->GetType() == LexemType::Identifier) {
        GetLex();
        while (lexem_->GetValue() == "+=" || lexem_->GetValue() == "-=" || lexem_->GetValue() == "*=" || lexem_->GetValue() == "/=" ||
            lexem_->GetValue() == "%=" || lexem_->GetValue() == "=") {
            GetLex();
            if (lexem_->GetType() != LexemType::Identifier)  {
                EXP_TWO();
                break;
            }
            else {
                ID();
            }
        }
    }
    else {
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
}

void SyntaxAnalyzer::EXP_FOURTEEN() {
    if (lexem_->GetType() == LexemType::Identifier) {
        GetLex();
        while (lexem_->GetValue() == "::") {
            GetLex();
            if (lexem_->GetType() != LexemType::Identifier) {
                throw "In line" + std::to_string(lexem_->GetLine()) + ": expected ID, found " + lexem_->GetLexemType();
            }
            else {
                ID();
            }
        }
    }
    else {
        TERM();
    }
}

void SyntaxAnalyzer::Analyze() {
    GetLex();
    PROGRAM();
}