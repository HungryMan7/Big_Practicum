#include "../includes/lexical_analyzer.h"
#include <fstream>

LexicalAnalyzer::LexicalAnalyzer(std::string file_name) {
    bor_ = new Bor;
    ReadFile(file_name);
}

LexicalAnalyzer::~LexicalAnalyzer() {
    delete bor_;
    delete program_;
}

void LexicalAnalyzer::ReadFile(std::string file_name) {
    std::ifstream file;
    try {
        file.open(file_name);
    } catch (const std::exception& e) {
        std::cout << "ERROR! " << e.what() << "\n";
    }
    size_t text_size;
    file.seekg(0, std::ios::end);
    text_size = file.tellg();
    file.seekg(0, std::ios::beg);
    program_ = new char[text_size];
    file.read(program_, text_size);
    file.close();
    iter_ = program_;
}

void LexicalAnalyzer::Analyze() {
    curr_lexem_.clear();
    GetChar();
    H();
}

void LexicalAnalyzer::SetLanguage(std::string file_name) {
    if (bor_) delete bor_;
    bor_ = new Bor(file_name);
}

Bor* LexicalAnalyzer::GetLanguage() {
    return bor_;
}

std::vector<Lexem*> LexicalAnalyzer::GetLexems() {
    return list_;
}

void LexicalAnalyzer::GetChar() {
    if (!iter_) {
        std::cout << "EOF reached!\n";
        return;
    }
    ch_ = *iter_;
    ++iter_;
}

void LexicalAnalyzer::H() {
    if (65 <= (int)ch_ && (int)ch_ <= 90 || 97 <= (int)ch_ && (int)ch_ <= 122 || (int)ch_ == 95) { /* case letter */
        curr_lexem_.push_back(ch_);
        GetChar();
        ID();
    } else if (48 <= (int)ch_ && (int)ch_ <= 57) { /* case digit */
        curr_lexem_.push_back(ch_);
        GetChar();
        INT();
    } else if ((int)ch_ == 34) { /* case " */
        // curr_lexem_.push_back(ch_);
        GetChar();
        STR();
    } else if ((int)ch_ == 35) { /* case # */
        curr_lexem_.push_back(ch_);
        GetChar();
        COM();
    } else if ((int)ch_ == 43 || (int)ch_ == 45 || (int)ch_ == 42 || (int)ch_ == 47 || (int)ch_ == 37 
        || (int)ch_ == 61 || (int)ch_ == 62 || (int)ch_ == 60 || (int)ch_ == 33) { /* case sign */
        curr_lexem_.push_back(ch_);
        GetChar();
        SIGN();
    } else {
        ERR();
    }
}

void LexicalAnalyzer::ID() {
    if (65 <= (int)ch_ && (int)ch_ <= 90 || 97 <= (int)ch_ && (int)ch_ <= 122 || (int)ch_ == 95
        || 48 <= (int)ch_ && (int)ch_ <= 57) {
        curr_lexem_.push_back(ch_);
        GetChar();
        ID();
    } else if (ch_ == ' ' || ch_ == '\0' || ch_ == '\n') {
        Lexem* new_lexem = new Lexem;
        if (bor_->FindString(curr_lexem_)) {
            new_lexem->SetType(LexemType::Utility);
        } else {
            new_lexem->SetType(LexemType::Identifier);
        }
        new_lexem->SetValue(curr_lexem_);
        list_.push_back(new_lexem);
        curr_lexem_.clear();
        GetChar();
        H();
    } else {
        ERR();
    }
}

void LexicalAnalyzer::COM() {
    if (ch_ != '\n' && ch_ != '\0') {
        curr_lexem_.push_back(ch_);
        GetChar();
        COM();
    } else {
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Comment);
        new_lexem->SetValue(curr_lexem_);
        list_.push_back(new_lexem);
        curr_lexem_.clear();
        GetChar();
        H();
    }
}

void LexicalAnalyzer::INT() {
    if (48 <= (int)ch_ && (int)ch_ <= 57) { /* case digit */
        curr_lexem_.push_back(ch_);
        GetChar();
        INT();
    } else if (ch_ == ' ' || ch_ == '\0' || ch_ == '\n') {
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Literal);
        new_lexem->SetValue(curr_lexem_);
        list_.push_back(new_lexem);
        curr_lexem_.clear();
        GetChar();
        H();
    } else {
        ERR();
    }
}

void LexicalAnalyzer::STR() {
    if (ch_ == '\n' || ch_ == '\0') {
        ERR();
    } else if ((int)ch_ == 34) {
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Literal);
        new_lexem->SetValue(curr_lexem_);
        list_.push_back(new_lexem);
        curr_lexem_.clear();
        GetChar();
        GetChar();
        H();
    } else {
        curr_lexem_.push_back(ch_);
        GetChar();
        STR();
    }
}

void LexicalAnalyzer::SIGN() {

}

void LexicalAnalyzer::ERR() {
    std::cout << "Wrong lexem detected!\n";
}


void LexicalAnalyzer::EMPTY() {

}
