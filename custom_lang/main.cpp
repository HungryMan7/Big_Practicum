#include <iostream>
#include <vector>

#include "bor.h"
#include "lexem.h"
#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include "semantic_analyzer.h"

int main() {
    try {
        std::cout << "LEXICAL ANALYSIS\n" << "______________________________\n";
        LexicalAnalyzer* la = new LexicalAnalyzer("program.txt");
        la->Analyze();
        std::cout << "OK!\n";
        std::cout << "SYNTAX ANALYSIS\n" << "______________________________\n";
        SyntaxAnalyzer* sa = new SyntaxAnalyzer("program.txt");
        sa->Analyze();
        std::cout << "OK!\n";
        std::cout << "SEMANTIC ANALYSIS\n" << "______________________________\n";
        SemanticAnalyzer* sm = new SemanticAnalyzer("program.txt");
        sm->Analyze();
        std::cout << "OK!\n";
    }
    catch (const std::string e) {
        std::cout << e;
    }
}