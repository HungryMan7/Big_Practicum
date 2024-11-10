#include "../includes/syntax_analyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(std::string file_name) {
    LexicalAnalyzer* la = new LexicalAnalyzer(file_name);
    la->SetLanguage("custom_lang/resources/lang.txt");
    la->Analyze();
    list_of_lexems_ = la->GetLexems();
}

SyntaxAnalyzer::~SyntaxAnalyzer() {

}

void SyntaxAnalyzer::GetLex() {
    lexem_ = *lex_iter_;
    ++lex_iter_;
    ++current_size_;
}

void SyntaxAnalyzer::START() {

}

void SyntaxAnalyzer::EXP_ZERO() {
    EXP_ONE();
    while (lexem_.GetValue() == ",") {
        GetLex();
        EXP_ONE();
    }
}

void SyntaxAnalyzer::EXP_ONE() {
    if (lexem_.GetType() == LexemType::Identifier) {
        GetLex();
        while (lexem_.GetValue() == "+=" || lexem_.GetValue() == "-=" || lexem_.GetValue() == "*=" || lexem_.GetValue() == "/=" ||
            lexem_.GetValue() == "%=" || lexem_.GetValue() == "=") {
            GetLex();
            if (lexem_.GetType() != LexemType::Identifier) throw "expected ID, found " + lexem_.GetValue();
        }
    }
    else {

    }
}

void SyntaxAnalyzer::EXP_TWO() {

}

void SyntaxAnalyzer::EXP_THREE() {

}

void SyntaxAnalyzer::EXP_FOUR() {

}

void SyntaxAnalyzer::EXP_FIVE() {

}

void SyntaxAnalyzer::EXP_SIX() {

}

void SyntaxAnalyzer::EXP_SEVEN() {

}

void SyntaxAnalyzer::EXP_EIGHT() {

}

void SyntaxAnalyzer::EXP_NINE() {

}

void SyntaxAnalyzer::EXP_TEN() {

}

void SyntaxAnalyzer::EXP_ELEVEN() {

}

void SyntaxAnalyzer::EXP_TWELVE() {

}

void SyntaxAnalyzer::EXP_THIRTEEN() {

}

void SyntaxAnalyzer::EXP_FOURTEEN() {

}

void SyntaxAnalyzer::Analyze() {
    GetLex();
    START();
}