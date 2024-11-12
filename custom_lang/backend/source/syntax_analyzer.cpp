#include "../includes/syntax_analyzer.h"

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
    while (lexem_->GetType() == LexemType::Utility) {
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
            GetLex();
            VARS_MDEF();
            if (lexem_->GetValue() != ";") {
                throw "line: " + std::to_string(lexem_->GetLine()) +
                    " column: " + std::to_string(lexem_->GetColumn()) +
                    " expected ;, but found " + lexem_->GetValue();
            }
            GetLex();
        }
    }
}

void SyntaxAnalyzer::VARS_MDEF() {
    
}

void SyntaxAnalyzer::VARS() {

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
        GetLex();
        FUNC_CALL();
        if (lexem_->GetValue() != ";") {
            throw "line: " + std::to_string(lexem_->GetLine()) +
                " column: " + std::to_string(lexem_->GetColumn()) +
                " expected ;, but found " + lexem_->GetValue();
        }
        GetLex();
    } else if (lexem_->GetValue() == "if") {
        IF();
    } else if (lexem_->GetValue() == "switch") {
        SWITCH();
    } else if (lexem_->GetValue() == "loop") {
        LOOP();
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
    } else {
        EX0();
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

void SyntaxAnalyzer::EX0() {

}

void SyntaxAnalyzer::Analyze() {
    GetLex();
    PROGRAM();
}