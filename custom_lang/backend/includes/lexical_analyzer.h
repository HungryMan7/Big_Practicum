#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "lexem.h"
#include "bor.h"

class LexicalAnalyzer {
public:
    LexicalAnalyzer(std::string file_name);
    virtual ~LexicalAnalyzer();
    void ReadFile(std::string file_name);
    void Analyze();
    void SetLanguage(const std::string& file_name);
    std::vector<Lexem*> GetLexems();

private:
    void GetChar();
    void H();
    void ID();
    void COM_ONE();
    void COM_MANY();
    void INT();
    void STR();
    void SIGN();
    void SPR();
    void BRK();
    void SPC();
    void FLOAT();
    char* program_, *iter_;
    int file_size_, current_size_ = 0;
    int line_number_ = 1, current_line_number_ = 1;
    int current_column_number_ = 1;
    Bor* bor_;
    std::vector<Lexem*> list_of_lexems_;
    char symbol_;
    std::string current_lexem_;
};