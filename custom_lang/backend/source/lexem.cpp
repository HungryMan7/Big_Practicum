#include "../includes/lexem.h"

void Lexem::SetType(LexemType type) {
    type_ = type;
}

void Lexem::SetValue(std::string value) {
    value_ = value;
}

void Lexem::SetLine(int line) {
    line_ = line;
}

void Lexem::SetColumn(int column) {
    column_ = column;
}

LexemType Lexem::GetType() {
    return type_;
}
std::string Lexem::GetValue() {
    return value_;
}

int Lexem::GetLine() {
    return line_;
}

int Lexem::GetColumn() {
    return column_;
}
