#pragma once
#include <iostream>

enum class LexemType { Identifier, Utility, Operator, Integer, Float, String, Brackets, Separator, Error };

class Lexem {
public:
    /* setters */
    void SetType(LexemType);
    void SetValue(std::string);
    void SetLine(int);

    /* getters */
    LexemType GetType();
    std::string GetValue();
    int GetLine();

private:
    LexemType type_;
    std::string value_;
    int line_;
};