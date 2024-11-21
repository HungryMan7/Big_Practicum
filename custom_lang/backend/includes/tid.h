#include <vector>
#include <string>

enum class Type { 
    Int,
    Float,
    Bool,
    Char,
    String,
    Array
};

class TID;

class Variable {
 public:
     Variable(std::string name, Type type, std::string value);
     Variable(const Variable& variable);
     std::string GetName();

 private:
     std::string name_;
     Type type_;     
     std::string value_;
};

class Function {
 public:
     Function(std::string name, Type return_type, TID* variables);
     Function(const Function& function);
     std::string GetName();

 private:
     std::string name_;
     Type return_type_;
     TID* variables_;
};

class TID {
 public:
     TID(const TID& other);
     void AddVariable(std::string name, Type type, std::string value);
     void AddVariable(const Variable*& variable);
     void AddVariable(const Variable& variable);
     void AddFunction(std::string name, Type return_type, TID* variables);
     void AddFunction(const Function*& function);
     void AddFunction(const Function& function);
     bool Contains(Variable* variable);
     bool Contains(Function* function);

 private:
     std::vector<Variable*> variables_;
     std::vector<Function*> functions_;
};