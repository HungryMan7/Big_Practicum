#include <iostream>

enum class LexemType { Identifier, Utility, Operator, Literal };

class Lexem {
public:
    void SetType(LexemType);
    void SetValue(std::string);
    void SetLine(int);
    void SetColumn(int);

    LexemType GetType();
    std::string GetValue();
    int GetLine();
    int GetColumn();

private:
    LexemType type_;
    std::string value_;
    int line_;
    int column_;
};
