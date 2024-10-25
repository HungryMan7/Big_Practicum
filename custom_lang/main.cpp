#include "backend/includes/lexical_analyzer.h"

int main() {
    LexicalAnalyzer* la = new LexicalAnalyzer;
    la->SetLanguage("custom_lang/resources/lang.txt");
    la->AnalyzeFile("custom_lang/resources/program.txt");
    return 0;
}
