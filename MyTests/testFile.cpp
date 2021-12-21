#include "../BSTree/BSTree.hpp"
#include "../Bonus/Huffman/Huffman.cpp"
#include <string>
#include <queue>

#include <iostream>

using namespace std;

/**
    @author William-Benoit Lambert
    @code 1735971
    @page testFile.cpp
    @note Cette page sert à tester les différentes structures de données mise en application dans ce projet
 */

/**
 * Traduit un parcours de l'arbre en String
 * @param traversal Les données de l'arbre selon le parcours désiré
 * @return le traversal en String, avec ajout de virgule de séparation.
 */
string printTraversal(queue<int>* traversal) {
    string traverse;
    int trCount = traversal->size();

    for(int i = 0; i < trCount; i++) {
        if(traverse.empty())
            traverse = to_string(traversal->front());
        else
            traverse += ", " + to_string(traversal->front());

        traversal->pop();
    }

    delete traversal;
    return traverse;
}

int main(int argc, char** argv) {
    BSTree<int> tree;
    Huffman compresser;

    switch(stoi(argv[1])) {
        case 0: tree.add(42);
                break;

        case 1: tree.add(42);
                tree.add(32);
                break;

        case 2: tree.add(42);
                tree.add(404);
                break;

        case 3:
        case 4:
        case 5: tree.add(42);
                tree.add(32);
                tree.add(23);
                tree.add(29);
                break;

        case 6:
        case 7:
        case 8: tree.add(42);
                tree.add(404);
                tree.add(666);
                tree.add(405);
                break;

        case 9:
        case 10:
        case 14:
        case 16:
        case 17:
        case 18:
        case 11: tree.add(42);
                 tree.add(404);
                 tree.add(666);
                 tree.add(32);
                 tree.add(69);
                 tree.add(23);
                 tree.add(11);
                 tree.add(12);
                 tree.add(29);
                 break;

        case 15: tree.add(42);
                 tree.add(404);
                 tree.add(666);
                 tree.add(32);
                 tree.add(69);
                 tree.add(23);
                 tree.add(11);
                 tree.add(12);
                 tree.add(29);
                 tree.add(32);
                 tree.add(69);
                 tree.add(23);
                 tree.add(11);
                 tree.add(12);
                 tree.add(29);
                 break;

        case 12:
        case 13: tree.add(7);
                 tree.add(0);
                 tree.add(3);
                 tree.add(6);
                 tree.add(5);
                 tree.add(1);
                 tree.add(2);
                 break;

        case 19: tree.add(7);
                 tree.add(0);
                 tree.add(1);
                 tree.add(2);
                 tree.add(3);
    }


    /*          42
     *         / \
     *       32  404
     *      /    / \
     *    23   69  666
     *   / \
     * 11  29
     *  \
     *   12
     */

    //TEST SEGMENT
    switch(stoi(argv[1])) {
        // Feuille Unique
        case 0: tree.remove(42);
                return (printTraversal(tree.traversal(Traversal::Postfix)).empty()) ? EXIT_SUCCESS: EXIT_FAILURE;

        case 1: tree.remove(32);
                return (printTraversal(tree.traversal(Traversal::Postfix)) == "42") ? EXIT_SUCCESS: EXIT_FAILURE;

        case 2: tree.remove(404);
                return (printTraversal(tree.traversal(Traversal::Postfix)) == "42") ? EXIT_SUCCESS: EXIT_FAILURE;

        // 1 Branche a gauche
        case 3: tree.remove(42);
                return (printTraversal(tree.traversal(Traversal::Postfix)) == "29, 23, 32") ? EXIT_SUCCESS: EXIT_FAILURE;

        case 4: tree.remove(32);
                return (printTraversal(tree.traversal(Traversal::Postfix)) == "29, 23, 42") ? EXIT_SUCCESS: EXIT_FAILURE;

        case 5: tree.remove(23);
                return (printTraversal(tree.traversal(Traversal::Postfix)) == "29, 32, 42") ? EXIT_SUCCESS: EXIT_FAILURE;

        // 1 Branche a droite
        case 6: tree.remove(42);
                return (printTraversal(tree.traversal(Traversal::Postfix)) == "405, 666, 404") ? EXIT_SUCCESS: EXIT_FAILURE;

        case 7: tree.remove(404);
                return (printTraversal(tree.traversal(Traversal::Postfix)) == "405, 666, 42") ? EXIT_SUCCESS: EXIT_FAILURE;

        case 8: tree.remove(666);
                return (printTraversal(tree.traversal(Traversal::Postfix)) == "405, 404, 42") ? EXIT_SUCCESS: EXIT_FAILURE;

        // Contient 2 branches
        case 9: tree.remove(42);
                return (printTraversal(tree.traversal(Traversal::Postfix)) == "12, 11, 29, 23, 69, 666, 404, 32") ? EXIT_SUCCESS: EXIT_FAILURE;

        case 10: tree.remove(23);
                return (printTraversal(tree.traversal(Traversal::Postfix)) == "11, 29, 12, 32, 69, 666, 404, 42") ? EXIT_SUCCESS: EXIT_FAILURE;

        case 11: tree.remove(404);
                 return (printTraversal(tree.traversal(Traversal::Postfix)) == "12, 11, 29, 23, 32, 666, 69, 42") ? EXIT_SUCCESS: EXIT_FAILURE;

        case 12: tree.remove(0);  //------------------------------------------------------------------------- CAS PARTICULIER?
                 return (printTraversal(tree.traversal(Traversal::Postfix)) == "2, 1, 5, 6, 3, 7") ? EXIT_SUCCESS: EXIT_FAILURE;

        case 13: tree.remove(7);  //------------------------------------------------------------------------- CAS PARTICULIER?
                 return (printTraversal(tree.traversal(Traversal::Postfix)) == "2, 1, 5, 6, 3, 0") ? EXIT_SUCCESS: EXIT_FAILURE;

        case 16: return (tree.search(42)) ? EXIT_SUCCESS: EXIT_FAILURE;

        case 17: tree.remove(666);
                 return (tree.search(666)) ? EXIT_FAILURE : EXIT_SUCCESS;

        case 18: tree.remove(32);
                  return (tree.search(23)) ? EXIT_SUCCESS: EXIT_FAILURE;

        case 19: tree.remove(7);
                 return (tree.search(2)) ? EXIT_SUCCESS: EXIT_FAILURE;

    }
}