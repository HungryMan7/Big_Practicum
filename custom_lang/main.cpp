#include "backend/includes/lexical_analyzer.h"

int main() {
    LexicalAnalyzer* la = new LexicalAnalyzer("custom_lang/resources/program.txt");
    la->SetLanguage("custom_lang/resources/lang.txt");
    la->Analyze();
    std::vector<Lexem*> checked_lexems = la->GetLexems();
    for (auto x : checked_lexems) {
        std::cout << x->GetValue() << " ";
        switch (x->GetType()) {
        case LexemType::Identifier:
            std::cout << "identifier" << "\n";
            break;

        case LexemType::Utility:
            std::cout << "utility" << "\n";
            break;

        case LexemType::Operator:
            std::cout << "operator" << "\n";
            break;

        case LexemType::Literal:
            std::cout << "literal" << "\n";
            break;

        case LexemType::Empty:
            std::cout << "empty" << "\n";
            break;
        }
    }
    return 0;
}
