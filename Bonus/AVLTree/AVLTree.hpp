#ifndef INC_420C35JOTP3_AVLTREE_HPP
#define INC_420C35JOTP3_AVLTREE_HPP

#include <queue>
#include "TLNode.hpp"

/**
    @author William-Benoit Lambert
    @code 1735971
    @page AVLTree.hpp
    @note Structure de données de l'arbre AVL
 */

/**
 * @attention Cette page est encore en développement et n'est pas terminé, merci de ne pas en tenir rigueur.
 */

using namespace std;

enum Traversal { Prefix, Infix, Postfix, BreathFirst };

template <typename T>
class AVLTree {
private:
    TLNode<T>* root;
    size_t count;

    void rotationDroite(TLNode<T> *node){
        TLNode<T>* temp1 = node->left;
        TLNode<T>* temp2 = temp1->right;

        temp1->right = node;
        node->left = temp2;
    }

    void rotationGauche(TLNode<T> *node){
        TLNode<T>* temp1 = node->right;
        TLNode<T>* temp2 = temp1->left;

        temp1->left = node;
        node-right = temp2;
    }

    void add(T data, TLNode<T>* node) {
        if(data > node->data)
            (node->right) ? add(data, node->right) : node->right = new TLNode<T>(data, node);
        else if(data < node->data)
            (node->left) ? add(data, node->left) : node->left = new TLNode<T>(data, node);

        //Calcul de l'indice d'equilibre
        if(Height(node->right) - Height(node->left) > 1 && data < node->left->data)
            rotationGauche(node);

        if(Height(node->right) - Height(node->left) < -1 && data > node->right->data)
            rotationDroite(node);

        //Cas d'exception
        if(Height(node->right) - Height(node->left) > 1 && data > node->left->data){
            rotationGauche(node->left);
            rotationDroit(node);
        }

        if(Height(node->right) - Height(node->left) < -1 && data < node->right->data) {
            rotationDroite(node->right);
            rotationGauche(node);
        }

    }

    void prefixTraversal(TLNode<T>* actual, queue<T>* traversalQueue) {
        traversalQueue->push(actual->data);
        if(actual->left)  prefixTraversal(actual->left, traversalQueue);
        if(actual->right) prefixTraversal(actual->right, traversalQueue);
    }

    void infixTraversal(TLNode<T>* actual, queue<T>* traversalQueue) {
        if(actual->left) infixTraversal(actual->left, traversalQueue);
        traversalQueue->push(actual->data);
        if(actual->right) infixTraversal(actual->right, traversalQueue);
    }

    void postfixTraversal(TLNode<T>* actual, queue<T>* traversalQueue) {
        if(actual->left) postfixTraversal(actual->left, traversalQueue);
        if(actual->right) postfixTraversal(actual->right, traversalQueue);
        traversalQueue->push(actual->data);
    }

    void breathfirstTraversal(queue<TLNode<T>*>* where, queue<T>* traversalQueue) {
        if(where->front()->left) where->push(where->front()->left);
        if(where->front()->right) where->push(where->front()->right);

        traversalQueue->push(where->front()->data);
        where->pop();

        if(where->size()) breathfirstTraversal(where, traversalQueue);
    }

    int Height(TLNode<T>* node) {
        int left = (node->left) ? Height(node->left) : 0;
        int right = (node->right) ? Height(node->right) : 0;
        return (left > right) ? left +1 : right +1;
    }

    TLNode<T>* searchNode(TLNode<T>* node, T data) {
        if(data < node->data)
            return searchNode(node->left, data);
        else if(data > node->data)
            return searchNode(node->right,data);
        else if(data == node->data)
            return node;
        else
            return nullptr;
    }

    bool search(TLNode<T>* node, T data) {
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
    AVLTree() {
        root = nullptr;
        count = 0;
    }

    ~AVLTree() { removeAll(); }

    void removeAll() {
        while(root) { remove(root->data); }
    }

    void remove(T data) {
        if(root && search(data)) {
            TLNode<T>* node = searchNode(data);

            //Feuille unique
            if (!node->right && !node->left) {
                if (node == root)
                    root = nullptr;
                else
                    (node->->left == node) ? node->->left = nullptr : node->->right = nullptr;
                delete node;
            }

                //Contient 1 branche
                //Droite
            else if (node->right && !node->left) {
                if (node == root) {
                    TLNode<T>* temp = root;
                    root = node->right;
                    delete temp;
                }
                else {
                    if (node == node->->right)
                        node->->right = node->right ?: node->left;
                    else
                        node->->left = node->right ?: node->left;

                    delete node;
                }
            }

                //Gauche
            else if (!node->right && node->left) {
                if (node == root) {
                    TLNode<T>* temp = root;
                    root = node->left;
                    delete temp;
                }
                else {
                    if (node == node->->left)
                        node->->left = node->left ?: node->right;
                    else
                        node->->right = node->left ?: node->right;

                    delete node;
                }
            }

                //Contient 2 branches
            else {
                TLNode<T> *temp = node->left;

                while (temp->right) { temp = temp->right; }

                T tempData = temp->data;
                remove(tempData);
                node->data = tempData;
            }
            count--;
        }
    }

    void add(T data) { (root) ? add(data, root) : root = new TLNode<T>(data); }

    TLNode<T>* searchNode(T data){ return (root)? searchNode(root, data): nullptr; }

    bool search(T data){ return (root) && search(root, data); }

    queue<T>* traversal(Traversal type) {
        auto* traversalQueue = new queue<T>;

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
                    auto* where = new queue<TLNode<T>*>;
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

#endif
