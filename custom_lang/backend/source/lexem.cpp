#include "../includes/lexem.h"
#include <string>

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

std::string Lexem::GetLexemType() {
    switch (GetType()) {
    case LexemType::Identifier:
        return "identifier";
        break;

    case LexemType::Utility:
        return "utility";
        break;

    case LexemType::Operator:
        return "operator";
        break;

    case LexemType::Integer:
        return "integer number";
        break;

    case LexemType::Float:
        return "float number";
        break;

    case LexemType::String:
        return "string";
        break;

    case LexemType::Brackets:
        return "bracket";
        break;

    case LexemType::Separator:
        return "separator";
        break;

    case LexemType::Logic:
        return "logical operator";
        break;

    case LexemType::Error:
        return "wrong lexem";
        break;
    default:
        return "nothing";
        break;
    }
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