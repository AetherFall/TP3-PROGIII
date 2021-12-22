#pragma once

/**
    @author William-Benoit Lambert
    @code 1735971
    @page TLNode.hpp
    @note Nodes servant à la création d'un arbre AVL
 */

template <typename T>
class TLNode {
  public:
      T data;
      TLNode<T>*up, * left,* right;

      TLNode(T data, TLNode<T>* up = nullptr, TLNode<T>* left = nullptr, TLNode<T>* right = nullptr) {
        this->data = data;
        this->left = up;
        this->left = left;
        this->right = right;
      }
};
