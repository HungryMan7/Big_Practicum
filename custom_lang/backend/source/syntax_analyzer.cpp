/*#include "SyntaxAnalyzer.h"
#include "LexicalAnalyzer.h"
#include <fstream>

SyntaxAnalyzer::SyntaxAnalyzer(std::string file_name) {
    ReadFile(file_name);
    LexicalAnalyzer* la = new LexicalAnalyzer("Program.txt");
    la->SetLanguage("Utilities.txt");
    la->Analyze();
    list_of_lexems_ = la->GetLexems();
}

SyntaxAnalyzer::~SyntaxAnalyzer() {
    delete program_;
}

void SyntaxAnalyzer::ReadFile(std::string file_name) {
    std::ifstream file;
    try {
        file.open(file_name, std::ifstream::binary);
    }
    catch (const std::exception& e) {
        std::cout << "ERROR! " << e.what() << "\n";
    }
    int text_size;
    file.seekg(0, file.end);
    text_size = (int)file.tellg();
    file.seekg(0, file.beg);
    program_ = new char[text_size];
    file.read(program_, text_size);
    file.close();
    iter_ = program_;
    file_size_ = text_size;
}

void SyntaxAnalyzer::GetLex() {
    lexem_ = *lex_iter_;
    ++lex_iter_;
    ++current_size_;
}

void SyntaxAnalyzer::START() {
      nothing 
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
            if (lexem_.GetType() != LexemType::Identifier) throw "expected ID, found " + ;
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
}*/