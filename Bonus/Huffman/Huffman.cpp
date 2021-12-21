#include "Huffman.hpp"

#define dictionnairePath "..//Bonus//Huffman//dictionnaire.txt"

//      PUBLIC   -------------------------------------------------
Huffman::Huffman(){ generationArbre(); generationDictionnaire(); }

Huffman::~Huffman() { delete huffmanTree; }

std::string Huffman::encoding(char text) {
    return "";
}

std::string Huffman::decoding(std::string cypher) { return ""; }

//      PRIVATE   ------------------------------------------------

void Huffman::generationArbre() {
    //Source du modèle statistique: https://fr.wikipedia.org/wiki/Fr%C3%A9quence_d%27apparition_des_lettres_en_fran%C3%A7ais
    string statsParOccurence = "eaisnrtoludcmpégbvhfqyxjèàkwz";
    auto* keyTree = new PriorityQueue<TLNode<char>*>;

    //Génération de l'étage le plus bas selon l'ordre des occurences.
    for(char i = 0; i < unsigned (statsParOccurence.length()); i++)
        keyTree->push(new TLNode<char>(statsParOccurence.at(i)), int(i) + 1);

    //Génération de l'arbre de Huffman.
    while(keyTree->size() != 1){
        char actualKeySize = keyTree->size(), i;
        PriorityQueue<TLNode<char>*> tempQueue;

        for(i = 0; i < actualKeySize && i+1 < actualKeySize; i+= 2){
            //Node 1
            TLNode<char>* temp1 = keyTree->front();
            char priority1 = unsigned(keyTree->getPriority());
            keyTree->pop();

            //Node 2
            TLNode<char>* temp2 = keyTree->front();
            char priority2 = unsigned(keyTree->getPriority());
            keyTree->pop();

            //Création de la nouvelle node parent.
            auto* newNode = new TLNode<char>(NULL, nullptr, temp1, temp2);
            tempQueue.push(newNode, int(i)+1);

            //Mise à jour des parents dans les feuilles.
            temp1->up = newNode;
            temp2->up = newNode;
        }

        //S'il reste une node solitaire
        if(keyTree->size()) {
            tempQueue.push(keyTree->front(), statsParOccurence.length() * 2); //Rien ne peux être plus grand que le maximum *2
            keyTree->pop();
        }

        //Transvidage dans KeyTree.
        actualKeySize = tempQueue.size();

        for(i = 0; i < actualKeySize; i++) {
            keyTree->push(tempQueue.front(), int(i)+1);
            tempQueue.pop();
        }
    }

    keyTree->front()->up = nullptr;
    this->huffmanTree = keyTree->front();
    delete keyTree;
}

void Huffman::generationDictionnaire() {
    ofstream fluxConstruction(dictionnairePath);
    if(fluxConstruction){
        charToHuffmanBin(huffmanTree, char(NULL), char(NULL), fluxConstruction);
        fluxConstruction.close();
    }

    else
        throw std::invalid_argument("Erreur dans la création du dictionnaire de Huffman. (Verifier le path)");
}

void Huffman::charToHuffmanBin(TLNode<char>* node, char byteChain, char from, ofstream& stream) {
    if(from != NULL)
        byteChain = (byteChain << 1) + from - '0';

        if (node->left)
            charToHuffmanBin(node->left, byteChain, '0', stream);

        if (node->right)
            charToHuffmanBin(node->right, byteChain, '1', stream);

    if(node->data > 32)
        stream << to_string(node->data) << " : " << to_string(int(byteChain)) << endl;

}
