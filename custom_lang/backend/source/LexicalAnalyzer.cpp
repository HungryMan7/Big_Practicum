#include "LexicalAnalyzer.h"
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

void LexicalAnalyzer::Analyze() {
    current_lexem_.clear();
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
    return list_of_lexems_;
}

void LexicalAnalyzer::GetChar() {
    if (current_size_ == file_size_) {
        for (auto x : list_of_lexems_) {
            std::cout << x->GetLine() << ") " << x->GetValue() << " => ";
            switch (x->GetType()) {
            case LexemType::Identifier:
                std::cout << "identifier" << "\n";
                break;

            case LexemType::Utility:
                std::cout << "utility" << "\n";
                break;

            case LexemType::Operator:
                std::cout << "operator" << "\n";
                break;

            case LexemType::Integer:
                std::cout << "integer number" << "\n";
                break;

            case LexemType::Float:
                std::cout << "float number" << "\n";
                break;

            case LexemType::String:
                std::cout << "string" << "\n";
                break;

            case LexemType::Brackets:
                std::cout << "bracket" << "\n";
                break;

            case LexemType::Separator:
                std::cout << "separator" << "\n";
                break;

            case LexemType::Error:
                std::cout << "wrong lexem" << "\n";
                break;

            }
        }
        exit(0);
    }
    symbol_ = *iter_;
    ++iter_;
    ++current_size_;
}

void LexicalAnalyzer::H() {
    if ('a' <= symbol_ && symbol_ <= 'z' || 'A' <= symbol_ && symbol_ <= 'Z' || symbol_ == '_') { /* case letter */
        current_lexem_.push_back(symbol_);
        GetChar();
        ID();
    }
    else if ('0' <= symbol_ && symbol_ <= '9') { /* case digit */
        current_lexem_.push_back(symbol_);
        GetChar();
        INT();
    }
    else if (symbol_ == '"') { /* case " */
        GetChar();
        STR();
    }
    else if (symbol_ == '+' || symbol_ == '-' || symbol_ == '*' || symbol_ == '/' || symbol_ == '%'
        || symbol_ == '=' || symbol_ == '>' || symbol_ == '<' || symbol_ == '!') { /* case sign */
        current_lexem_.push_back(symbol_);
        GetChar();
        SIGN();
    }
    else if (symbol_ == '.' || symbol_ == ',' || symbol_ == ';' || symbol_ == ':') {
        current_lexem_.push_back(symbol_);
        GetChar();
        SPR();
    }
    else if (symbol_ == '{' || symbol_ == '}' || symbol_ == '<' || symbol_ == '>'
        || symbol_ == '(' || symbol_ == ')' || symbol_ == '[' || symbol_ == ']') {
        current_lexem_.push_back(symbol_);
        GetChar();
        BRK();
    }
    else if (symbol_ == '\n') {
        if (symbol_ == '\n') {
            ++current_line_number_;
            ++line_number_;
        }
        GetChar();
        H();
    }
    else if (symbol_ == '#') {
        GetChar();
        SPC();
    }
    else if (current_size_ == file_size_) {
        return;
    }
    else {
        GetChar();
        H();
    }
}

void LexicalAnalyzer::ID() {
    if ('A' <= symbol_ && symbol_ <= 'Z' || 'a' <= symbol_ && symbol_ <= 'z' || symbol_ == '_'
        || '0' <= symbol_ && symbol_ <= '9') {
        current_lexem_.push_back(symbol_);
        GetChar();
        ID();
    }
    else {
        Lexem* new_lexem = new Lexem;
        if (bor_->FindString(current_lexem_)) {
            new_lexem->SetType(LexemType::Utility);
            new_lexem->SetLine(line_number_);
            line_number_ = current_line_number_;
        }
        else {
            new_lexem->SetType(LexemType::Identifier);
            new_lexem->SetLine(line_number_);
            line_number_ = current_line_number_;
        }
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        H();
    }
}

void LexicalAnalyzer::INT() {
    if ('0' <= symbol_ && symbol_ <= '9') { /* case digit */
        current_lexem_.push_back(symbol_);
        GetChar();
        INT();
    }
    else if (symbol_ == '.') {
        GetChar();
        if (symbol_ >= '0' && symbol_ <= '9') {
            current_lexem_.push_back('.');
            current_lexem_.push_back(symbol_);
            GetChar();
            FLOAT();
        }
        else {
            Lexem* new_lexem = new Lexem;
            new_lexem->SetType(LexemType::Integer);
            new_lexem->SetLine(line_number_);
            new_lexem->SetValue(current_lexem_);
            list_of_lexems_.push_back(new_lexem);
            current_lexem_.clear();
            new_lexem = new Lexem;
            new_lexem->SetType(LexemType::Separator);
            new_lexem->SetLine(line_number_);
            line_number_ = current_line_number_;
            new_lexem->SetValue(".");
            list_of_lexems_.push_back(new_lexem);
            H();
        }
    }
    else {
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Integer);
        new_lexem->SetLine(line_number_);
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        H();
    }
}

void LexicalAnalyzer::FLOAT() {
    if ('0' <= symbol_ && symbol_ <= '9') { /* case digit */
        current_lexem_.push_back(symbol_);
        GetChar();
        FLOAT();
    }
    else {
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Float);
        new_lexem->SetValue(current_lexem_);
        new_lexem->SetLine(line_number_);
        line_number_ = current_line_number_;
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        H();
    }
}

void LexicalAnalyzer::STR() {
    if (symbol_ == '\r' || current_size_ == file_size_) {
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Error);
        new_lexem->SetValue(current_lexem_);
        new_lexem->SetLine(line_number_);
        line_number_ = current_line_number_;
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        H();
    }
    else if (symbol_ == '"') {
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::String);
        new_lexem->SetLine(line_number_);
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        GetChar();
        H();
    }
    else {
        current_lexem_.push_back(symbol_);
        GetChar();
        STR();
    }
}

void LexicalAnalyzer::SPR() {
    if (current_lexem_ == ":") {
        if (symbol_ == ':') {
            current_lexem_.push_back(symbol_);
            Lexem* new_lexem = new Lexem;
            new_lexem->SetType(LexemType::Separator);
            new_lexem->SetLine(line_number_);
            line_number_ = current_line_number_;
            new_lexem->SetValue(current_lexem_);
            list_of_lexems_.push_back(new_lexem);
            current_lexem_.clear();
            GetChar();
            H();
        }
        else {
            Lexem* new_lexem = new Lexem;
            new_lexem->SetType(LexemType::Separator);
            new_lexem->SetLine(line_number_);
            line_number_ = current_line_number_;
            new_lexem->SetValue(current_lexem_);
            list_of_lexems_.push_back(new_lexem);
            current_lexem_.clear();
            H();
        }
    }
    else {
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Separator);
        new_lexem->SetLine(line_number_);
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        H();
    }
}

void LexicalAnalyzer::BRK() {
    Lexem* new_lexem = new Lexem;
    new_lexem->SetType(LexemType::Brackets);
    new_lexem->SetLine(line_number_);
    line_number_ = current_line_number_;
    new_lexem->SetValue(current_lexem_);
    list_of_lexems_.push_back(new_lexem);
    current_lexem_.clear();
    H();
}

void LexicalAnalyzer::SIGN() {
    if (symbol_ == '=') {
        current_lexem_.push_back(symbol_);
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Operator);
        new_lexem->SetLine(line_number_);
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        GetChar();
        H();
    }
    else if (symbol_ == '+' && current_lexem_ == "+") {
        current_lexem_.push_back(symbol_);
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Operator);
        new_lexem->SetLine(line_number_);
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        GetChar();
        H();
    }
    else if (symbol_ == '>' && current_lexem_ == ">") {
        current_lexem_.push_back(symbol_);
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Operator);
        new_lexem->SetLine(line_number_);
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        GetChar();
        H();
    }
    else if (symbol_ == '<' && current_lexem_ == "<") {
        current_lexem_.push_back(symbol_);
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Operator);
        new_lexem->SetLine(line_number_);
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        GetChar();
        H();
    }
    else if (symbol_ == '-' && current_lexem_ == "-") {
        current_lexem_.push_back(symbol_);
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Operator);
        new_lexem->SetLine(line_number_);
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        GetChar();
        H();
    }
    else if (symbol_ == '/' && current_lexem_ == "/") {
        current_lexem_.clear();
        GetChar();
        COM_ONE();
    }
    else if (symbol_ == '*' && current_lexem_ == "/") {
        current_lexem_.clear();
        GetChar();
        COM_MANY();
    }
    else {
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Operator);
        new_lexem->SetValue(current_lexem_);
        new_lexem->SetLine(line_number_);
        line_number_ = current_line_number_;
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        H();
    }
}

void LexicalAnalyzer::COM_ONE() {
    if (symbol_ != '\n' && current_size_ != file_size_) {
        current_lexem_.push_back(symbol_);
        GetChar();
        COM_ONE();
    }
    else {
        current_lexem_.clear();
        H();
    }
}

void LexicalAnalyzer::COM_MANY() {
    if (symbol_ == '/' && current_lexem_[current_lexem_.size() - 1] == '*') {
        current_lexem_.clear();
        line_number_ = current_line_number_;
        GetChar();
        H();
    }
    else if (current_size_ == file_size_) {
        current_lexem_.push_back(symbol_);
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Error);
        new_lexem->SetValue(current_lexem_);
        new_lexem->SetLine(line_number_);
        line_number_ = current_line_number_;
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        H();
    }
    else {
        if (symbol_ == '\n') ++current_line_number_;
        current_lexem_.push_back(symbol_);
        GetChar();
        COM_MANY();
    }
}

void LexicalAnalyzer::SPC() {
    if (symbol_ != '\n' && current_size_ != file_size_) {
        current_lexem_.push_back(symbol_);
        GetChar();
        SPC();
    }
    else {
        current_lexem_.clear();
        H();
    }
}
