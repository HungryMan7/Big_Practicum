#pragma once
#include "lexical_analyzer.h"
#include "lexem.h"
#include "tid.h"

#include <iostream>
#include <string>
#include <vector>

class SemanticsAnalyzer {
public:
    SemanticsAnalyzer(std::string file_name);
    virtual ~SemanticsAnalyzer();
    void Analyze();

private:
    void GetLex();
    void PROGRAM();
    void VARS_MDEF(std::vector<std::string> id_type);
    void FUNC(std::string id_name);
    void BODY(std::string func_name);
    void FUNC_BODY(std::string func_name);
    void STATEMENT(std::string func_name);
    void FUNC_STATEMENT(bool &check, std::string func_name);
    std::vector<std::string> FUNC_CALL(std::string id_name);
    void IF(std::string func_name);
    void LOOP(std::string func_name);
    void LOOP_WHILE(std::string func_name);
    void LOOP_FOR(std::string func_name);
    void LOOP_FOREACH(std::string func_name);
    void SWITCH(std::string func_name);
    std::vector<std::string> ID();
    std::vector<std::string> TERM();
    void INPUT();
    void OUTPUT();
    std::vector<std::string> EXP_ZERO();
    std::vector<std::string> EXP_ONE();
    std::vector<std::string> EXP_TWO();
    std::vector<std::string> EXP_THREE();
    std::vector<std::string> EXP_FOUR();
    std::vector<std::string> EXP_FIVE();
    std::vector<std::string> EXP_SIX();
    std::vector<std::string> EXP_SEVEN();
    std::vector<std::string> EXP_EIGHT();
    std::vector<std::string> EXP_NINE();
    std::vector<std::string> EXP_TEN();
    std::vector<std::string> EXP_ELEVEN();
    std::vector<std::string> EXP_TWELVE();
    std::vector<std::string> EXP_THIRTEEN();
    std::vector<std::string> EXP_FOURTEEN();
    LexicalAnalyzer* la;
    Lexem* lexem_;
    int lex_index_ = 0;
    std::pair<std::vector<std::string>, std::vector<std::string>> current_type_;
    std::string current_id_name_;
    std::vector<Lexem*> list_of_lexems_;
    TID table_id_;
};
