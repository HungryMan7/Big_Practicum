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
    /*! Create trie by file */
    Bor(std::string file_name);
    /*! Add string to trie */
    void AddString(std::string &s);
    /*! Find string in trie */
    bool FindString(std::string &s);
    /*! Delete trie */
    void Delete(Node* vertex);
    ~Bor();
private:
    Node *root = new Node;
};