#pragma once
#include "lexical_analyzer.h"

class SyntaxAnalyzer {
    public:
        SyntaxAnalyzer(std::string file_name);
        virtual ~SyntaxAnalyzer();
        void Analyze();

    private:
        void GetLex();
        void PROGRAM();
        void VARS_MDEF();
        void VARS();
        void FUNC();
        void BODY();
        void STATEMENT();
        void FUNC_CALL();
        void IF();
        void LOOP();
        void SWITCH();
        void EX0();
        LexicalAnalyzer* la;
        Lexem* lexem_;
        int lex_index_ = 0;
        std::vector<Lexem*> list_of_lexems_;
};

