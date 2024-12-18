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
    Bor(std::string file_name); /*! \brief Create trie by file */
    void AddString(std::string &s); /*! \brief Add string to trie */
    bool FindString(std::string &s); /*! \brief Find string in trie */
    void Delete(Node* vertex); /*! \brief Delete trie */
    ~Bor();
private:
    Node *root = new Node;
};