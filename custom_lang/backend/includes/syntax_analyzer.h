#pragma once
#include "lexical_analyzer.h"
#include "lexem.h"

class SyntaxAnalyzer {
 public:
     void Analyze(std::vector<Lexem*> list_of_lexems);

 private:
     void GetLex();
     void PROGRAM();
     void VARS_MDEF();
     void FUNC();
     void BODY();
     void STATEMENT();
     void FUNC_CALL();
     void IF();
     void LOOP();
     void LOOP_WHILE();
     void LOOP_FOR();
     void LOOP_FOREACH();
     void SWITCH();
     void ID();
     void TERM();
     void INPUT();
     void OUTPUT();
     void EXP_ZERO();
     void EXP_ONE();
     void EXP_TWO();
     void EXP_THREE();
     void EXP_FOUR();
     void EXP_FIVE();
     void EXP_SIX();
     void EXP_SEVEN();
     void EXP_EIGHT();
     void EXP_NINE();
     void EXP_TEN();
     void EXP_ELEVEN();
     void EXP_TWELVE();
     void EXP_THIRTEEN();
     void EXP_FOURTEEN();
     Lexem* lexem_;
     int lex_index_ = 0;
     std::vector<Lexem*> list_of_lexems_;
};
