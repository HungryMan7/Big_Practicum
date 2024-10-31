#include <iostream>
#include <vector>

#include "Bor.h"
#include "Lexem.h"
#include "LexicalAnalyzer.h"

int main() {
    LexicalAnalyzer* la = new LexicalAnalyzer("Program.txt");
    la->SetLanguage("Utilities.txt");
    la->Analyze();
}