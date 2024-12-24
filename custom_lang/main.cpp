#include "backend/includes/lexical_analyzer.h"
#include "backend/includes/syntax_analyzer.h"
#include "backend/includes/semantics_analyzer.h"
#include "backend/includes/rpn.h"
#include "backend/includes/interpreter.h"

int main() {
    LexicalAnalyzer* la = new LexicalAnalyzer;
    std::cout << "LEXICAL ANALYSIS\n" << "______________________________\n";
    try {
        la->SetLanguage("resources/lang.txt");
        la->Analyze("resources/program.txt");
    } catch (const std::string e) {
        std::cout << e;
    } catch (char const* e) {
        std::cout << e;
    }
    SyntaxAnalyzer* sa = new SyntaxAnalyzer;
    SemanticsAnalyzer* sm = new SemanticsAnalyzer;
    try {
        std::cout << "SYNTAX ANALYSIS\n" << "______________________________\n";
        sa->Analyze(la->GetLexems());
        std::cout << "OK!\n";
        std::cout << "SEMANTIC ANALYSIS\n" << "______________________________\n";
        sm->Analyze(la->GetLexems());
        std::cout << "OK!\n";
    } catch (const std::string e) {
        std::cout << e;
    } catch (char const* e) {
        std::cout << e;
    }
    RPN* rpn_chain = new RPN;
    rpn_chain->Analyze(la->GetLexems());
    for (auto elem : rpn_chain->GetChain()) {
        elem->Print();
        std::cout << "\n";
    }
    std::cout << "tvoya mat' shluxa\n";
    Interpreter* interpreter = new Interpreter;
    interpreter->Run(rpn_chain);
    return 0;
}