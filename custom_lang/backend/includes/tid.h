#include <vector>
#include <string>

enum class Type { int, float, bool, char, string, array };

class Variable {
 public:
     Variable(std::string name, Type type, std::string value);
     Variable(const Variable& variable);

 private:
     std::string name_;
     Type type_;
     std::string value;
};

class Function {
 public:
     Function(std::string name, Type return_type, TID* variables);
     Function(const Function& function);

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

 private:
     std::vector<Variable*> variables_;
     std::vector<Function*> functions_;
};