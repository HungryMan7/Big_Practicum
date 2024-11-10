#pragma once
#include "lexical_analyzer.h"

class SyntaxAnalyzer {
    public:
        SyntaxAnalyzer(std::string file_name);
        virtual ~SyntaxAnalyzer();
        void Analyze();

        /* setters */

        /* getters */

    private:
        void GetLex();
        void START();
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
        char* program_, *iter_;
        Lexem* lex_iter_;
        Lexem lexem_;
        std::vector<Lexem*> list_of_lexems_;
        int file_size_, current_size_ = 0;
};

