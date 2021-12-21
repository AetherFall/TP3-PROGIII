#ifndef TP3_HUFFMAN_HPP
#define TP3_HUFFMAN_HPP

/**
    @author William-Benoit Lambert
    @code 1735971
    @page Huffman.hpp
    @note Initialisation d'un objet Huffman servant à crypter et décrypter des chaines de caractères.
 */

#include <string>
#include "../PriorityQueue/priorityQueue.hpp"
#include "../AVLTree/TLNode.hpp"
#include <cstdlib> //For binary conversion
#include <fstream>

class Huffman {
    public:
        Huffman();
        ~Huffman();
        std::string encoding(char text);
        std::string decoding(std::string cypher);

    private:
        TLNode<char>* huffmanTree;
        void charToHuffmanBin(TLNode<char>* node, char byteChain, char from, ofstream& stream);
        void generationDictionnaire();
        void generationArbre();
};

#endif
