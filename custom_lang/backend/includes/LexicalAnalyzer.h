#pragma once
#include <iostream>
#include <vector>

#include "Lexem.h"
#include "Bor.h"

class LexicalAnalyzer {
public:
    LexicalAnalyzer(std::string file_name);
    virtual ~LexicalAnalyzer();
    void ReadFile(std::string file_name);
    void Analyze();

    /* setters */
    void SetLanguage(std::string file_name);

    /* getters */
    Bor* GetLanguage();
    std::vector<Lexem*> GetLexems();

private:
    void GetChar();
    void H(); /* start state */
    void ID(); /* identificator's state */
    void COM_ONE(); /* commetaries */
    void COM_MANY(); /* commetaries */
    void INT(); /* integer numbers' state */
    void STR(); /* string's state */
    void SIGN(); /* sign's state */
    void SPR(); /* separation state */
    void BRK(); /* brackets' state */
    void SPC(); /* includes' / defines' / pragma's state */
    void FLOAT(); /* float numbers's state */
    char* program_, *iter_;
    int file_size_, current_size_ = 0;
    int line_number_ = 1, current_line_number_ = 1;
    Bor* bor_;
    std::vector<Lexem*> list_of_lexems_;
    char symbol_;
    std::string current_lexem_;
};