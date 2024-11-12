#include "syntax_analyzer.h"
#include <fstream>

SyntaxAnalyzer::SyntaxAnalyzer(std::string file_name) {
    la = new LexicalAnalyzer("Program.txt");
    la->SetLanguage("Utilities.txt");
    la->Analyze();
    list_of_lexems_ = la->GetLexems();
}

SyntaxAnalyzer::~SyntaxAnalyzer() {
    delete la;
}

void SyntaxAnalyzer::GetLex() {
    if (lex_index_ >= (int)list_of_lexems_.size()) return;
    lexem_ = list_of_lexems_[lex_index_];
    ++lex_index_;
}

void SyntaxAnalyzer::START() {
    /* nothing */
}

void SyntaxAnalyzer::ID() {
    /* nothing */
}

void SyntaxAnalyzer::TERM() {
    /* nothing */
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
                throw "In line" + std::to_string(lexem_->GetLine()) + ": expected ID, found" + lexem_->GetLexemType();
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
    START();
}