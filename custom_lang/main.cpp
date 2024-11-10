#include <iostream>
#include <vector>

#include "backend/includes/syntax_analyzer.h"

int main() {
    LexicalAnalyzer* la = new LexicalAnalyzer("custom_lang/resources/program.txt");
    la->SetLanguage("custom_lang/resources/lang.txt");
    la->Analyze();
    SyntaxAnalyzer* sa = new SyntaxAnalyzer("custom_lang/resources/program.txt");
}
