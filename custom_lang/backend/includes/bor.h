#pragma once
#include <iostream>
#include <string>

struct Node {
    bool is_terminal = false;
    Node *to[26];
    Node() {
        for (int i = 0; i < 26; ++i) {
            to[i] = nullptr;
        }
    }
};
  
/*! \brief A trie class */
class Bor {
public:
    Bor();
    Bor(std::string file_name); /*! Create trie by file */
    void AddString(std::string &s); /*! Add string to trie */
    bool FindString(std::string &s); /*! Find string in trie */
    void Delete(Node* vertex); /*! Delete trie */
    ~Bor();
private:
    Node *root = new Node;
};