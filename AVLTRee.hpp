#ifndef INC_420C35JOTP3_AVLTREE_HPP
#define INC_420C35JOTP3_AVLTREE_HPP

#include <queue>
#include "TLNode.hpp"

using namespace std;

enum Traversal { Prefix, Infix, Postfix };

template <typename T>
class AVLTree {
private:
    TLNode<T>* root;
    size_t count;

    void add(T data, TLNode<T>* node) {
        if(data > node->data)
            (node->right) ? add(data, node->right) : node->right = new TLNode<T>(data, node);
        else if(data < node->data)
            (node->left) ? add(data, node->left) : node->left = new TLNode<T>(data, node);

        //Calcul de l'indice d'equilibre
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

    int Height(TLNode<T>* node) {
        int left = (node->left) ? Height(node->left) : 0;
        int right = (node->right) ? Height(node->right) : 0;
        return (left > right) ? left++ : (left < right) ? right++ : 1;
    }


    int getHeight(DLNode<T> node) {

    }

public:
    BSTree() {
        root = nullptr;
        count = 0;
    }

    ~BSTree(){
        while(root) {
            remove(root);
        }
    }

    void remove(TLNode<T>* root){
        //Le noeud a retirer (search)
        //Verification si le noeud existe
            //Aucun enfant
            //1 Enfant
            //2 Enfant
                //Valeur temporaire

    }

    void add(T data) { (root) ? add(data, root) : root = new TLNode<T>(data); }

    bool search(T data) {
        if(root) {
            TLNode<T>* runner = root;
            while(runner) {
                if(data < runner->data)
                    runner = runner->left;
                else if(data > runner->data)
                    runner = runner->right;
                else
                    return true;
            }
            return false;
        }
    }

    queue<T>* traversal(Traversal type) {
        queue<T>* traversalQueue = new queue<T>();

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
            }
        }
        return traversalQueue;
    }
};

#endif
