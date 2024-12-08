class RPN {
  public:
    RPN() { chain = {}; }
    ~RPN() { chain = {}; }
    GetChain() { return chain }
    AddCell(Cell element) { chain.push_back(element) }
  private:
    std::stack<Cell> chain;
};

RPN program_rpn;

struct Cell {
  public:
    Cell(Lexem lex) {
        if (lex->GetType() == LexemType::Integer) {
      type = "int"
      value_int = std::stoi(lex->value);
        } else if (lex->GetType() == LexemType::Float) {
      value_double = std::stod(lex->value);
        } else if (lex->value == "true" || lex->value == "false") {
      value_bool = (lex->value == "true" ? true : false);
        } else if (lex->GetType() == LexemType::String) {
      value_string = lex->value;
        } else if (lex->GetType() == LexemType::Identificator) {
            id_name = lex->value;
      if (lex[index_of_lexem_] == "(") {
    type = "function";
      } else {
    type = table_id.GetType(id_name);
      }
        } else if (lex->value == "{") {
      std::string elem_type = "";
      int cnt = 0, max_cnt = 0;
      while (lex->value == "{") {
          GetLex();
    cnt++;
    max_cnt = max(cnt, max_cnt);
          if (lex->value == "{") {
        continue;
          } else {
                  do {
            if (lex->value == ",") GetLex();
            elem_type = EXP_ONE() // здесь будем строить арифм. дерево
              } while (lex->value == ",");
        GetLex(); // }
        cnt--;
        if (lex->value == ",") GetLex();
          }
      }
      for (int i = 0; i < max_cnt; ++i) {
    type += "array ";
      }
      type += elem_type;
  } 
    }
  private:
    std::string type;
    int value_int;
    double value_double;
    bool value_bool;
    std::string value_string;
    std::string id_name;
    std::string operation;
    bool RPN_GO;
};