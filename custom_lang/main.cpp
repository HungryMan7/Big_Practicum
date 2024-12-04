#include "backend/includes/lexical_analyzer.h"
#include "backend/includes/syntax_analyzer.h"
#include "backend/includes/semantics_analyzer.h"

int main() {
    try {
        std::cout << "LEXICAL ANALYSIS\n" << "______________________________\n";
        LexicalAnalyzer* la = new LexicalAnalyzer;
        la->SetLanguage("resources/lang.txt");
        la->Analyze("resources/program.txt");
        std::cout << "OK!\n";
        std::cout << "SYNTAX ANALYSIS\n" << "______________________________\n";
        SyntaxAnalyzer* sa = new SyntaxAnalyzer;
        sa->Analyze(la->GetLexems());
        std::cout << "OK!\n";
        std::cout << "SEMANTIC ANALYSIS\n" << "______________________________\n";
        SemanticsAnalyzer* sm = new SemanticsAnalyzer;
        sm->Analyze(la->GetLexems());
        std::cout << "OK!\n";
    }
    catch (const std::string e) {
        std::cout << e;
    }
}