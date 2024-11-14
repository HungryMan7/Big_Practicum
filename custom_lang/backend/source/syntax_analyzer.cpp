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
    if (lex_index_ < (int)list_of_lexems_.size()) {
        lexem_ = list_of_lexems_[lex_index_];
        ++lex_index_;
    }
}

void SyntaxAnalyzer::PROGRAM() {
    while (3 + 5 == 8) {
        if (!(lexem_->GetType() == LexemType::Type)) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected type, but found " + lexem_->GetValue();
        }
        GetLex();
        if (lexem_->GetValue() == "main") break;
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
        }
    }
    if (lexem_->GetValue() != "main") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected main, but found " + lexem_->GetValue();
    }
    GetLex();
    if (lexem_->GetValue() != "(") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected (, but found " + lexem_->GetValue();
    }
    GetLex();
    if (lexem_->GetValue() != ")") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn() - 1) +
            " expected ), but found " + lexem_->GetValue();
    }
    GetLex();
    BODY();
}

void SyntaxAnalyzer::VARS_MDEF() {
    if (lexem_->GetValue() == ";") {
        GetLex();
        return;
    }
    if (lexem_->GetValue() == "=") {
        GetLex();
        EXP_ZERO();
        GetLex();
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
            EXP_ZERO();
            if (lexem_->GetValue() != ";" && lexem_->GetValue() != ",") {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " expected ;, but found " + lexem_->GetValue();
            }
            GetLex();
        }
    }
    GetLex();
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
    } else if (lexem_->GetType() == LexemType::Type) {
        GetLex();
        if (lexem_->GetType() != LexemType::Identifier) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected identifier, but found " + lexem_->GetValue();
        }
        GetLex();
        while (lexem_->GetValue() == ",") {
            GetLex();
            if (!(lexem_->GetType() == LexemType::Type)) {
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
        if (lex_index_ >= list_of_lexems_.size() && lexem_->GetValue() != "}") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected }, but found " + lexem_->GetValue();
        }
    }
    GetLex();
}

// <statement> ::= <vars-mdef> ';' | <ex0> ';' | <if> | <switch> | <loop> | <func-call> ';' | "break" ';' | "continue" ';' | eps
void SyntaxAnalyzer::STATEMENT() {
    if (lexem_->GetType() == LexemType::Type) {
        GetLex();
        if (lexem_->GetType() != LexemType::Identifier) {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected identifier, but found " + lexem_->GetValue();
        }
        GetLex();
        VARS_MDEF();
    } else if (lexem_->GetType() == LexemType::Identifier) {
        EXP_ZERO();
        if (lexem_->GetValue() != ";") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn() - 1) +
                " expected ;, but found " + lexem_->GetValue();
        }
        GetLex();
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
    } else {
        GetLex();
    }
}

void SyntaxAnalyzer::FUNC_CALL() {

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

/*
loops
| <loop>         ::= "loop" [ <loop-while> | <loop-foreach> | <loop-for> ]
| <loop-while>   ::= '(' <ex0> ')' <body>
| <loop-foreach> ::= '(' <id> ';' <ex0> ')' <body>
| <loop-for>     ::= '(' [ <ex0> | <vars-mdef> | eps ] ';' [ <ex0> | eps ] ';' [ <ex0> | eps ] ')' <body>
*/
void SyntaxAnalyzer::LOOP() {
    if (lexem_->GetValue() != "(") {
        throw "line: " + std::to_string(lexem_->GetLine()) +
            " column: " + std::to_string(lexem_->GetColumn()) +
            " expected (, but found " + lexem_->GetValue();
    }
    GetLex();
    if (lexem_->GetType() == LexemType::Type || lexem_->GetValue() == ";") {
        GetLex();
        LOOP_FOR();
    } else if (lexem_->GetType() == LexemType::Identifier) {
        GetLex();
        LOOP_FOREACH();
    } else {
        GetLex();
        LOOP_WHILE();
    }
    if (lexem_->GetValue() == ")") {
        GetLex();
        return;
    }
}

void SyntaxAnalyzer::LOOP_WHILE() {
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
    if (lexem_->GetType() == LexemType::Identifier) {
        GetLex();
        if (lexem_->GetValue() == "(") {
            GetLex();
            if (lexem_->GetType() != LexemType::Identifier && lexem_->GetType() != LexemType::Integer &&
                lexem_->GetType() != LexemType::Float && lexem_->GetType() != LexemType::String &&
                lexem_->GetValue() != ")") {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " expected values, but found " + lexem_->GetValue();
            }
            while (lexem_->GetValue() == ",") {
                GetLex();
                if (lexem_->GetType() != LexemType::Identifier && lexem_->GetType() != LexemType::Integer &&
                    lexem_->GetType() != LexemType::Float && lexem_->GetType() != LexemType::String) {
                    throw "line: " + std::to_string(lexem_->GetLine()) +
                        " column: " + std::to_string(lexem_->GetColumn() - 1) +
                        " expected values, but found " + lexem_->GetValue();
                }
                GetLex();
            }
            if (lexem_->GetValue() != ")") {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn() - 1) +
                    " expected ), but found " + lexem_->GetValue();
            }
            GetLex();
        }
    } else {
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
                    " expected (, but found " + lexem_->GetValue();
            }
        } else GetLex();
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

void SyntaxAnalyzer::Analyze() {
    GetLex();
    PROGRAM();
}