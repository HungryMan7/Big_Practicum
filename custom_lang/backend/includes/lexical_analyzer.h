#include <vector>
#include "lexem.h"
#include "bor.h"

class LexicalAnalyzer {
 public:
     void AnalyzeFile(std::string file_name);

     /* setters */
     void SetLanguage(std::string file_name);

     /* getters */
     Bor*& GetLanguage();

 private:
     Bor* bor_;
     std::vector<Lexem*> list_;
     char ch;
};
