#pragma once
#include <iostream>

enum class LexemType { Identifier, Utility, Operator, Integer, Float, String, Brackets, Separator, Logic, Type, Error };

class Lexem {
public:
    /* setters */
    void SetType(LexemType);
    void SetValue(std::string);
    void SetLine(int);
    void SetColumn(int);

    /* getters */
    LexemType GetType();
    std::string GetLexemType();
    std::string GetValue();
    int GetLine();
    int GetColumn();

private:
    LexemType type_;
    std::string value_;
    int line_ = 0;
    int column_ = 0;
};