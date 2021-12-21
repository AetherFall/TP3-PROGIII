#include "Huffman.hpp"

#define dictionnairePath "..//Bonus//Huffman//dictionnaire.txt"

//PUBLIC                 -------------------------------------------------
Huffman::Huffman(){ generationArbre(); generationDictionnaire(); }

Huffman::~Huffman() { delete huffmanTree; }

std::string Huffman::encoding(std::string text) {
    string crypted;

    for(char &letter : text) {
        if(!crypted.empty())
            crypted += " ";

        crypted += to_string(findLetterCode(tolower(letter)));
    }

    return crypted;
}

std::string Huffman::decoding(std::string cypher) {
    string ligne;

    for(char &code : cypher){
        if(code != ' ')
            ligne += to_string(findDecrypt(code));
    }

    return ligne;
}

//PRIVATE              ------------------------------------------------

int Huffman::Height(TLNode<char>* node) {
    int left = (node->left) ? Height(node->left) : 0;
    int right = (node->right) ? Height(node->right) : 0;
    return (left > right) ? left+1 : right+1;
}

void Huffman::generationArbre() {
    //Source du modèle statistique: https://fr.wikipedia.org/wiki/Fr%C3%A9quence_d%27apparition_des_lettres_en_fran%C3%A7ais
    string statsParOccurence = " eaisnrtoludcmpgbvhfqyxjkwz";
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
            auto* newNode = new TLNode<char>('\000', nullptr, temp1, temp2);
            tempQueue.push(newNode, priority1 + priority2);

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
        charToHuffmanBin(huffmanTree, char('\000'), char('\000'), fluxConstruction, 0);
        fluxConstruction.close();
    }

    else
        throw std::invalid_argument("Erreur dans la création du dictionnaire de Huffman. (Verifier le path)");
}

void Huffman::charToHuffmanBin(TLNode<char>* node, char byteChain, char from, ofstream& stream, int iteration) {
    if(from != '\000') {
        byteChain = (byteChain << 1) + from - '0';
        iteration++;
    }

    if (node->left)
        charToHuffmanBin(node->left, byteChain, '0', stream, iteration);

    if (node->right)
        charToHuffmanBin(node->right, byteChain, '1', stream, iteration);

    //Ajout de bit de remplissage car l'abre génère des doublons de lettres chiffré
    while(iteration < Height(huffmanTree) -1){
        byteChain <<= 1;
        iteration++;
    }

    if(node->data > 31)
        stream << to_string(node->data) << ":" << int(byteChain) << endl;

}

char Huffman::findLetterCode(char letter){
    ifstream fluxDictionnaire(dictionnairePath);
    string line;
    char code;

    if(fluxDictionnaire){
        while(getline(fluxDictionnaire, line)){
            if(getLetter(line) == letter){
                code = getCode(line);
                break;
            }
        }

        fluxDictionnaire.close();
    }

    else
        generationDictionnaire();

    return code;
}

char Huffman::findDecrypt(char cypher){
    ifstream fluxDictionnaire(dictionnairePath);
    string line;
    char letter;

    if(fluxDictionnaire){
        while(getline(fluxDictionnaire, line)){
            if(getCode(line) == cypher){
                letter = getLetter(line);
                break;
            }
        }

        fluxDictionnaire.close();
    }

    else
        generationDictionnaire();

    return letter;
}

char Huffman::getLetter(string line) {
    int i = 0;
    string temp = "";

    while(i < line.length() && line.at(i) != ':') {
        temp += line.at(i);
        i++;
    }

    return (!temp.empty())? char(std::stoi(temp)): '\000';
}

char Huffman::getCode(string line) {
    int i = 0;
    size_t pos = 0;

    while(i < line.length() && line.at(i) != ':') { i++; }

    line.erase(line.begin(), line.begin() + (i +1));

    return char(std::stoi(line));
}