#pragma once

#include <queue>
#include "DLNode.hpp"

using namespace std;

/**
    @author William-Benoit Lambert
    @code 1735971
    @page BSTree.hpp
    @note Structure de données --> Arbre binaire de recherche
 */

enum Traversal { Prefix, Infix, Postfix, BreathFirst };

template <typename T>
class BSTree {
private:
    DLNode<T>* root;
    size_t count;

    void prefixTraversal(DLNode<T>* node, queue<T>* traversalQueue) {
        traversalQueue->push(node->data);
        if (node->left)
            prefixTraversal(node->left, traversalQueue);
        if(node->right)
            prefixTraversal(node->right, traversalQueue);
    }

    void infixTraversal(DLNode<T>* node, queue<T>* traversalQueue) {
        if (node->left)
            infixTraversal(node->left, traversalQueue);
        traversalQueue->push(node->data);
        if(node->right)
            infixTraversal(node->right, traversalQueue);
    }

    void postfixTraversal(DLNode<T>* node, queue<T>* traversalQueue) {
        if (node->left)
            postfixTraversal(node->left, traversalQueue);
        if(node->right)
            postfixTraversal(node->right, traversalQueue);
        traversalQueue->push(node->data);
    }

    void breathfirstTraversal(queue<DLNode<T>*>* where, queue<T>* traversalQueue) {
        if(where->front()->left) where->push(where->front()->left);
        if(where->front()->right) where->push(where->front()->right);

        traversalQueue->push(where->front()->data);
        where->pop();

        if(where->size()) breathfirstTraversal(where, traversalQueue);
    }

    DLNode<T>* getParent(DLNode<T>* feuille){
        if(root) {
            if(feuille != root){
                DLNode<T> *node = root, *temp = root;

                while (node) {
                    if (node->left && feuille->data < node->data) {
                        temp = node;
                        node = node->left;
                    }
                    else if (node->right && feuille->data > node->data) {
                        temp = node;
                        node = node->right;
                    }
                    else
                        return temp;
                }
            }
        }
        return nullptr;
    }

    DLNode<T>* searchNode(DLNode<T>* node, T data) {
        if(data < node->data)
            return searchNode(node->left, data);
        else if(data > node->data)
            return searchNode(node->right,data);
        else if(data == node->data)
            return node;
        else
            return nullptr;
    }

    bool search(DLNode<T>* node, T data) {
        if(node) {
            if (data < node->data && node->left)
                return search(node->left, data);
            else if (data > node->data && node->right)
                return search(node->right, data);
            else if(data == node->data)
                return true;
            else
                return false;
        }
        else
            return false;
    }

public:
    BSTree() {
        root = nullptr;
        count = 0;
    }

    ~BSTree() { removeAll(); }

    void removeAll() {
        while(root) { remove(root->data); }
    }

    void add(T data) {
        if(root) {
            DLNode<T>* runner = root;

            while(runner) {
                if(data < runner->data) {
                    if(runner->left)
                        runner = runner->left;
                    else {
                        runner->left = new DLNode<T>(data);
                        runner = nullptr;
                        count++;
                    }
                }
                else if(data > runner->data) {
                    if(runner->right)
                        runner = runner->right;
                    else {
                        runner->right = new DLNode<T>(data);
                        runner = nullptr;
                        count++;
                    }
                }
                else
                    runner = nullptr;
            }
        }
        else {
            root = new DLNode<T>(data);
            count++;
        }
    }

    void remove(T data) {
        if(root && search(data)) {
            DLNode<T>* node = searchNode(data);

            //Feuille unique
            if (!node->right && !node->left) {
                if (node == root)
                    root = nullptr;
                else
                    (getParent(node)->left == node) ? getParent(node)->left = nullptr : getParent(node)->right = nullptr;
                delete node;
            }

            //Contient 1 branche
            //Droite
            else if (node->right && !node->left) {
                if (node == root) {
                    DLNode<T>* temp = root;
                    root = node->right;
                    delete temp;
                }
                else {
                    if (node == getParent(node)->right)
                        getParent(node)->right = node->right ?: node->left;
                    else
                        getParent(node)->left = node->right ?: node->left;

                    delete node;
                }
            }

            //Gauche
            else if (!node->right && node->left) {
                if (node == root) {
                    DLNode<T>* temp = root;
                    root = node->left;
                    delete temp;
                }
                else {
                    if (node == getParent(node)->left)
                        getParent(node)->left = node->left ?: node->right;
                    else
                        getParent(node)->right = node->left ?: node->right;

                    delete node;
                }
            }

            //Contient 2 branches
            else {
                DLNode<T> *temp = node->left;

                while (temp->right) { temp = temp->right; }

                T tempData = temp->data;
                remove(tempData);
                node->data = tempData;
            }
            count--;
        }
    }

    DLNode<T>* searchNode(T data){ return (root)? searchNode(root, data): nullptr; }

    bool search(T data){ return (root) && search(root, data); }

    queue<T>* traversal(Traversal type) {
        queue<T>* traversalQueue = new queue<T>;

        if(root) {
            switch (type) {
                case Traversal::Prefix:
                    prefixTraversal(root, traversalQueue);
                    break;
                case Traversal::Infix:
                    infixTraversal(root, traversalQueue);
                    break;
                case Traversal::Postfix:
                    postfixTraversal(root, traversalQueue);
                    break;
                case Traversal::BreathFirst:
                    queue<DLNode<T>*>* where = new queue<DLNode<T>*>;
                    where->push(root);
                    breathfirstTraversal(where, traversalQueue);
                    delete where;
                    break;
            }
        }
        return traversalQueue;
    }

    size_t size() { return this->count; }

};