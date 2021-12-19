#ifndef TP3_HUFFMAN_HPP
#define TP3_HUFFMAN_HPP

/**
    @author William-Benoit Lambert
    @code 1735971
    @page Huffman.hpp
    @note Initialisation d'un objet Huffman servant à crypter et décrypter des chaines de caractères.
 */

#include <string>

class Huffman {
    public:
        Huffman();
        std::string encoding(std::string text);
        std::string decoding(std::string cypher);

    private:
        string cleCryptage;
        void generationArbre();
};

#endif
