#include <iostream>
#include <vector>

#include "Bor.h"
#include "Lexem.h"
#include "LexicalAnalyzer.h"

int main() {
    LexicalAnalyzer* la = new LexicalAnalyzer("program.txt");
    la->SetLanguage("lang.txt");
    la->Analyze();
}
