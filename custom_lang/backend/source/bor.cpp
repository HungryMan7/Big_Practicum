#include "Bor.h"

#include <iostream>
#include <fstream>
#include <string>

Bor::Bor() {
    root = new Node;
}

Bor::Bor(std::string file_name) {
    root = new Node;
    std::ifstream file;
    file.open(file_name);
    std::string words;
    while (file >> words) {
        AddString(words);
    }
    file.close();
}

void Bor::AddString(std::string &s) {
    Node *vertex = root;
    for (char symbol : s) {
        symbol -= 'a';
        if (!vertex->to[symbol]) {
            vertex->to[symbol] = new Node;
        }
        vertex = vertex->to[symbol];
    }
    vertex->is_terminal = true;
}

bool Bor::FindString(std::string &s) {
    Node *vertex = root;
    for (char symbol : s) {
        symbol -= 'a';
        if (!vertex->to[symbol]) {
            return false;
        }
        vertex = vertex->to[symbol];
    }
    return vertex->is_terminal;
}

void Bor::Delete(Node* vertex) {
    if (!vertex) return;
    for (int i = 0; i < 26; ++i) {
        if (vertex->to[i]) {
            Delete(vertex->to[i]);
        }
    }
    delete vertex;
}

Bor::~Bor() {
    Delete(root);
}
