#include <iostream>
#include <vector>

#include "backend/includes/lexical_analyzer.h"
#include "backend/includes/syntax_analyzer.h"
#include "backend/includes/semantics_analyzer.h"

int main() {
    try {
        std::cout << "LEXICAL ANALYSIS\n" << "______________________________\n";
        LexicalAnalyzer* la = new LexicalAnalyzer("resources/program.txt");
        la->SetLanguage("utilities.txt");
        la->Analyze();
        std::cout << "OK!\n";
        std::cout << "SYNTAX ANALYSIS\n" << "______________________________\n";
        SyntaxAnalyzer* sa = new SyntaxAnalyzer("resources/program.txt");
        sa->Analyze();
        std::cout << "OK!\n";
        std::cout << "SEMANTIC ANALYSIS\n" << "______________________________\n";
        SemanticsAnalyzer* sm = new SemanticsAnalyzer("resources/program.txt");
        sm->Analyze();
        std::cout << "OK!\n";
    }
    catch (const std::string e) {
        std::cout << e;
    }
}