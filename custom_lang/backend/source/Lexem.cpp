#include "Lexem.h"

void Lexem::SetType(LexemType type) {
    type_ = type;
}

void Lexem::SetValue(std::string value) {
    value_ = value;
}

void Lexem::SetLine(int line) {
    line_ = line;
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