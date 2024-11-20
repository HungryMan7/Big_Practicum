#include "backend/includes/syntax_analyzer.h"
#include "backend/includes/semantics_analyzer.h"

int main() {
    try {
        LexicalAnalyzer* la = new LexicalAnalyzer;
        la->SetLanguage("custom_lang/resources/lang.txt");
        la->Analyze("custom_lang/resources/program.txt");
        SyntaxAnalyzer* sya = new SyntaxAnalyzer;
        sya->Analyze(la->GetLexems());
        /*
        SemanticsAnalyzer* sea = new SemanticsAnalyzer;
        sea->Analyze(la->GetLexems()); 
        */
        std::cout << "OK!\n";
    } catch (const std::string e) {
        std::cout << e;
    }
}
