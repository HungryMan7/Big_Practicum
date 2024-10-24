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

class Bor {
public:
    Bor();
    Bor(std::string file_name);
    void AddString(std::string &s);
    bool FindString(std::string &s);
    void Delete(Node* vertex);
    ~Bor();
private:
    Node *root = new Node;
};
