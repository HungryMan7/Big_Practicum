#include "../includes/lexical_analyzer.h"
#include <fstream>
#include <string>

LexicalAnalyzer::~LexicalAnalyzer() {
    delete bor_;
    delete program_;
    for (auto lexem : list_of_lexems_) {
        delete lexem;
    }
}

void LexicalAnalyzer::ReadFile(const std::string& filename) {
    std::ifstream file;
    try {
        file.open(filename, std::ifstream::binary);
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

void LexicalAnalyzer::Analyze(const std::string& filename) {
    ReadFile(filename);
    current_lexem_.clear();
    GetChar();
    H();
}

void LexicalAnalyzer::SetLanguage(const std::string& filename) {
    if (bor_) delete bor_;
    bor_ = new Bor(filename);
}

std::vector<Lexem*> LexicalAnalyzer::GetLexems() {
    return list_of_lexems_;
}

void LexicalAnalyzer::GetChar() {
    if (current_size_ == file_size_ + 1) {
        for (auto x : list_of_lexems_) {
            if (x->GetType() == LexemType::Error) {
                throw "line: " + std::to_string(x->GetLine()) +
                    " column: " + std::to_string(x->GetColumn() - 1) +
                    " wrong lexem";
            }
        }
        return;
    }
    symbol_ = *iter_;
    ++iter_;
    ++current_size_;
    ++current_column_number_;
}

void LexicalAnalyzer::H() {
    if ('a' <= symbol_ && symbol_ <= 'z' || 'A' <= symbol_ && symbol_ <= 'Z' || symbol_ == '_') {
        current_lexem_.push_back(symbol_);
        GetChar();
        ID();
    }
    else if ('0' <= symbol_ && symbol_ <= '9') {
        current_lexem_.push_back(symbol_);
        GetChar();
        INT();
    }
    else if (symbol_ == '"') {
        GetChar();
        STR();
    }
    else if (symbol_ == '+' || symbol_ == '-' || symbol_ == '*' || symbol_ == '/' || symbol_ == '%'
        || symbol_ == '=' || symbol_ == '>' || symbol_ == '<' || symbol_ == '!' || symbol_ == '^'
        || symbol_ == '&' || symbol_ == '|') {
        current_lexem_.push_back(symbol_);
        GetChar();
        SIGN();
    }
    else if (symbol_ == '.' || symbol_ == ',' || symbol_ == ';' || symbol_ == ':') {
        current_lexem_.push_back(symbol_);
        GetChar();
        SPR();
    }
    else if (symbol_ == '{' || symbol_ == '}' || symbol_ == '(' || symbol_ == ')' || symbol_ == '[' || symbol_ == ']') {
        current_lexem_.push_back(symbol_);
        GetChar();
        BRK();
    }
    else if (symbol_ == '\n') {
        if (symbol_ == '\n') {
            ++current_line_number_;
            current_column_number_ = 1;
            ++line_number_;
        }
        GetChar();
        H();
    }
    else if (symbol_ == '#') {
        GetChar();
        SPC();
    }
    else if (current_size_ == file_size_ + 1) {
        GetChar();
        return;
    }
    else if ((int)symbol_ == 39) {
        throw "line: " + std::to_string(current_line_number_) +
            " column: " + std::to_string(current_column_number_) +
            " wrong lexem (can't use char)";
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
            if (current_lexem_ == "int" || current_lexem_ == "double" || current_lexem_ == "bool" ||
                current_lexem_ == "char" || current_lexem_ == "array" || current_lexem_ == "string" || current_lexem_ == "void") {
                new_lexem->SetType(LexemType::Type);
            }
            else {
                new_lexem->SetType(LexemType::Utility);
            }
            new_lexem->SetLine(line_number_);
            line_number_ = current_line_number_;
        }
        else {
            new_lexem->SetType(LexemType::Identifier);
            new_lexem->SetLine(line_number_);
            line_number_ = current_line_number_;
        }
        new_lexem->SetValue(current_lexem_);
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size());
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        H();
    }
}

void LexicalAnalyzer::INT() {
    if ('0' <= symbol_ && symbol_ <= '9') {
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
            new_lexem->SetColumn(current_column_number_ - current_lexem_.size() - 1);
            current_lexem_.clear();
            new_lexem = new Lexem;
            new_lexem->SetType(LexemType::Separator);
            new_lexem->SetLine(line_number_);
            new_lexem->SetColumn(1);
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
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size());
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        H();
    }
}

void LexicalAnalyzer::FLOAT() {
    if ('0' <= symbol_ && symbol_ <= '9') {
        current_lexem_.push_back(symbol_);
        GetChar();
        FLOAT();
    }
    else {
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Float);
        new_lexem->SetValue(current_lexem_);
        new_lexem->SetLine(line_number_);
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size());
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
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size());
        line_number_ = current_line_number_;
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        H();
    }
    else if (symbol_ == '"') {
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::String);
        new_lexem->SetLine(line_number_);
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size());
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
            new_lexem->SetColumn(current_column_number_ - current_lexem_.size() + 1);
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
            new_lexem->SetColumn(current_column_number_ - current_lexem_.size());
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
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size());
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
    new_lexem->SetColumn(current_column_number_ - current_lexem_.size());
    line_number_ = current_line_number_;
    new_lexem->SetValue(current_lexem_);
    list_of_lexems_.push_back(new_lexem);
    current_lexem_.clear();
    H();
}

void LexicalAnalyzer::SIGN() {
    if (symbol_ == '=') {
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Operator);
        new_lexem->SetLine(line_number_);
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size());
        current_lexem_ += "=";
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        GetChar();
        H();
    }
    else if (current_lexem_ == "&" && symbol_ == '&') {
        current_lexem_.push_back(symbol_);
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Logic);
        new_lexem->SetLine(line_number_);
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size() + 1);
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        GetChar();
        H();
    }
    else if (current_lexem_ == "|" && symbol_ == '|') {
        current_lexem_.push_back(symbol_);
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Logic);
        new_lexem->SetLine(line_number_);
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size() + 1);
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        GetChar();
        H();
    }
    else if (current_lexem_ == "+" && symbol_ == '+') {
        current_lexem_.push_back(symbol_);
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Operator);
        new_lexem->SetLine(line_number_);
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size() + 1);
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        GetChar();
        H();
    } else if (current_lexem_ == ">" || current_lexem_ == "<") {
        if (symbol_ == '=') {
            current_lexem_.push_back(symbol_);
            GetChar();
        } else if (current_lexem_ == ">" && symbol_ == '>' ||
                   current_lexem_ == "<" && symbol_ == '<') {
            current_lexem_.push_back(symbol_);
            GetChar();
            if (symbol_ == '=') {
                current_lexem_.push_back(symbol_);
                GetChar();
            }
        }
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Operator);
        new_lexem->SetLine(line_number_);
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size() + 1);
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        H();   
    } else if (current_lexem_ == "-" && symbol_ == '-') {
        current_lexem_.push_back(symbol_);
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Operator);
        new_lexem->SetLine(line_number_);
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size() + 1);
        line_number_ = current_line_number_;
        new_lexem->SetValue(current_lexem_);
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        GetChar();
        H();
    } else if (current_lexem_ == "/" && symbol_ == '/') {
        current_lexem_.clear();
        GetChar();
        COM_ONE();
    } else if (current_lexem_ == "/" && symbol_ == '*') {
        current_lexem_.clear();
        GetChar();
        COM_MANY();
    } else {
        Lexem* new_lexem = new Lexem;
        new_lexem->SetType(LexemType::Operator);
        new_lexem->SetValue(current_lexem_);
        new_lexem->SetLine(line_number_);
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size());
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
        new_lexem->SetColumn(current_column_number_ - current_lexem_.size());
        line_number_ = current_line_number_;
        list_of_lexems_.push_back(new_lexem);
        current_lexem_.clear();
        H();
    }
    else {
        if (symbol_ == '\n') {
            ++current_line_number_;
        }
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
