#include "backend/includes/syntax_analyzer.h"

int main() {
    SyntaxAnalyzer* sa = new SyntaxAnalyzer("custom_lang/resources/program.txt");
    try {
        sa->Analyze();
    } catch (const std::string e) {
        std::cout << e;
    }
}
