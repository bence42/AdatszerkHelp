//
// Created by Roli on 2019.10.06..
//

#ifndef KHF02_TREE_PARSER_HPP
#define KHF02_TREE_PARSER_HPP

#include <iostream>
#include <string>
#include "exception.hpp"

class TernaryTree {
    class Node {
    public:
        int value;
        Node *left, *middle, *right, *parent;
        Node(int V) : value(V), left(nullptr), middle(nullptr), right(nullptr), parent(nullptr) {}
    };

public:
    Node *root;
    TernaryTree(const std::string &input) {
        root = new Node(-1);
        Node *act = root;
        for (unsigned int i = 1; i < input.length() - 1; ++i) {
            if (input[i] == '(' && act->left == nullptr) {
                Node *x = new Node(-1);
                act->left = x;
                x->parent = act;
                act = x;
            } else if (input[i] == '(' && act->middle == nullptr) {
                Node *x = new Node(-1);
                act->middle = x;
                x->parent = act;
                act = x;
            } else if (input[i] == '(' && act->right == nullptr) {
                Node *x = new Node(-1);
                act->right = x;
                x->parent = act;
                act = x;
            } else if (input[i] == ')') {
                act = act->parent;
            } else if (input[i] > 47 && input[i] < 58 && act->left == nullptr) {
                int val = input.at(i) - 48;
                Node *x = new Node(val);
                act->left = x;
                x->parent = act;
            } else if (input[i] > 47 && input[i] < 58 && act->middle == nullptr) {
                int val = input.at(i) - 48;
                Node *x = new Node(val);
                act->middle = x;
                x->parent = act;
            } else if (input[i] > 47 && input[i] < 58 && act->right == nullptr) {
                int val = input.at(i) - 48;
                Node *x = new Node(val);
                act->right = x;
                x->parent = act;
            }
        }
    }

    ~TernaryTree() {
        _destroy(root);
    }

    static void _destroy(Node *x);
    int get_value(const std::vector<short> &path) const;
};


int TernaryTree::get_value(const std::vector<short> &path) const {
    Node *act = root;
    for (short i : path) {
        switch (i) {
            case (0):
                act = act->left;
                if (act == nullptr) {
                    throw TreeException();
                }
                break;
            case (1):
                act = act->middle;
                if (act == nullptr) {
                    throw TreeException();
                }
                break;
            case (2):
                act = act->right;
                if (act == nullptr) {
                    throw TreeException();
                }
                break;
            default:
                break;
        }
    }
    if(act->value == -1){
        throw TreeException();
    }
    return act->value;
}

void TernaryTree::_destroy(Node *x) {
    if (x != nullptr) {
        _destroy(x->left);
        _destroy(x->middle);
        _destroy(x->right);
        delete x;
    }
}

#endif //KHF02_TREE_PARSER_HPP
