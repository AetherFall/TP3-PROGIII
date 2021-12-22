#ifndef INC_420C35JOTP3_SERIALIZATION_HPP
#define INC_420C35JOTP3_SERIALIZATION_HPP
#pragma once

#include <fstream>
#include <string>
#include <stdexcept>
#include <stack>

#include "../../Folder.hpp"
#include "../../Note.hpp"

/**
    @author William-Benoit Lambert
    @code 1735971
    @page Serialization.hpp
    @note Fonctions de Serialization du programme dans un fichier XML
    @note Contient aussi toutes les fonctions de gestions de chaînes de caractères
 */

class Serialization {
public:
    //Constructeurs
    Serialization();
    Serialization(Folder *root);

    ~Serialization();

    //Méthodes récursive
    void sauvegarder(const string &filePath);
    Folder *chargement(const string &filePath);
    static string replace(string line, const string &regex = "  ", const string &future = "");

private:
    //Variables
    Folder *root;
    stack<Folder *> *path;

    //Récursivité
    void sauvegarder(Folder *fichier, std::ofstream &stream, string level);
    void chargement(std::ifstream &stream);

    //Méthodes
    static string getNoteContent(ifstream &stream);
    static FILETYPE getFileType(string line);
    static bool getEndFile(string line);
    static bool getEndNote(string line);
    static string getAttribute(string line, const string &attribut);

};
#endif
