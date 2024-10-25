#include "../includes/lexical_analyzer.h"
#include <fstream>

void LexicalAnalyzer::AnalyzeFile(std::string file_name) {
    std::ifstream file;
    try {
        file.open(file_name);
    } catch (const std::exception& e) {
        std::cout << "ERROR! " << e.what() << "\n";
    }
    std::vector<std::string> buffer;
    std::string word;
    while (file >> word) {
        buffer.push_back(word);
    }
    for (auto word : buffer) {
        std::cout << bor_->FindString(word) << "\n";
    }
}

void LexicalAnalyzer::SetLanguage(std::string file_name) {
    if (bor_) delete bor_;
    bor_ = new Bor(file_name);
}

Bor*& LexicalAnalyzer::GetLanguage() {
    return bor_;
}
