#include <iostream>
#include <vector>

#include "backend/includes/lexical_analyzer.h"

int main() {
    LexicalAnalyzer* la = new LexicalAnalyzer("custom_lang/resources/program.txt");
    la->SetLanguage("custom_lang/resources/lang.txt");
    la->Analyze();
}
