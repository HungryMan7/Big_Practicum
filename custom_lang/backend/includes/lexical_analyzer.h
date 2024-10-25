#include <vector>
#include "lexem.h"
#include "bor.h"

class LexicalAnalyzer {
 public:
     LexicalAnalyzer(std::string file_name);
     virtual ~LexicalAnalyzer();
     void ReadFile(std::string file_name);
     void Analyze();

     /* setters */
     void SetLanguage(std::string file_name);

     /* getters */
     Bor* GetLanguage();
     std::vector<Lexem*> GetLexems();

 private:
     void GetChar();
     void H();
     void ID();
     void COM();
     void INT();
     void STR();
     void SIGN();
     void ERR();
     void EMPTY();
     char* program_,* iter_;
     Bor* bor_;
     std::vector<Lexem*> list_;
     char ch_;
     std::string curr_lexem_;
};
