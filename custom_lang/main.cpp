#include "backend/includes/lexical_analyzer.h"
#include "backend/includes/syntax_analyzer.h"
#include "backend/includes/semantics_analyzer.h"
#include "backend/includes/rpn.h"

int main() {
    LexicalAnalyzer* la = new LexicalAnalyzer;
    SyntaxAnalyzer* sa = new SyntaxAnalyzer;
    SemanticsAnalyzer* sm = new SemanticsAnalyzer;
    try {
        std::cout << "LEXICAL ANALYSIS\n" << "______________________________\n";
        la->SetLanguage("resources/lang.txt");
        la->Analyze("resources/program.txt");
        std::cout << "OK!\n";
        std::cout << "SYNTAX ANALYSIS\n" << "______________________________\n";
        sa->Analyze(la->GetLexems());
        std::cout << "OK!\n";
        std::cout << "SEMANTIC ANALYSIS\n" << "______________________________\n";
        sm->Analyze(la->GetLexems());
        std::cout << "OK!\n";
    }
    catch (const std::string e) {
        std::cout << e;
    }

    RPN* rpn_chain = new RPN;
    rpn_chain->Analyze(la->GetLexems());
    std::vector<RPN_Node*> answer = rpn_chain->GetChain();
}