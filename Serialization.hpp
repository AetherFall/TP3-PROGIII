#ifndef INC_420C35JOTP3_SERIALIZATION_HPP
#define INC_420C35JOTP3_SERIALIZATION_HPP

#include <fstream>
#include <string>
#include <stdexcept>
#include <stack>

#include "Folder.hpp"
#include "Note.hpp"

/**
    @author William-Benoit Lambert
    @code 1735971
    @page Serialization.hpp
    @note Fonctions de Serialization du programme dans un fichier XML
 */

class Serialization {
    private:
        Folder* root;
        stack<Folder*>* path;

        /**
         * Sauvegarde du fichier de façon récursif
         * @param fichier Folder dans lequel nous sommes
         * @param stream Flux stream du fichier en cours
         * @param level Niveau d'indentation du fichier
         */
        void sauvegarder(Folder* fichier, std::ofstream& stream, string level) {
            stream << level << "<F name='" << fichier->getName() <<"'>" << endl;
            string level2 = level + "    ";

            if(fichier->getAllSize()) {
                if(fichier->getFolderSize()) {
                    for(int i = 0; i < fichier->getFolderSize(); i++)
                        sauvegarder(fichier->getFolderAt(i), stream, level2);
                }
                if(fichier->getNoteSize()) {
                    for(int i = 0; i < fichier->getNoteSize(); i++) {
                        stream << level2 << "<N name='" << fichier->getNoteNameAt(i) << "'>" <<endl;
                        stream << level2 << fichier->getNoteContentAt(i) << endl << level2 << "</N>" <<endl<<endl;
                    }
                }
            }

            stream << level << "</F>" <<endl<<endl;
        }

        /**
         * Fonction de chargement de la fenêtre récursivement
         * @param stream
         */
        void chargement(std::ifstream& stream){
            string line;

            while(getline(stream, line)) {
                line = replace(line); //Suppression de tous les espaces du niveau de l'indentation.

                if(!line.empty()) {
                    if (!getEndFile(line)) {
                        if (!getEndNote(line)) {
                            switch (getFileType(line)) {
                                case DOSSIER:
                                    if (path->size()) {
                                        path->top()->createFolder(getAttribute(line, "name"));
                                        path->push(path->top()->getLastFolder()); //On entre dans le dernier dossier ajouté
                                    }

                                    else { //Aucune root, alors on l'ajoute
                                        root = new Folder(getAttribute(line, "name"));
                                        path->push(root);
                                    }

                                    //On verifie s'il y a des sous-dossier/ fichier et on les ajoutes.
                                    chargement(stream);
                                    break;

                                case FICHIER:
                                    path->top()->createFile(getAttribute(line, "name"));
                                    path->top()->getLastNote()->setContent(getNoteContent(stream));
                                    break;
                                case COMPRESSED:
                                    break;
                            }
                        }
                    } else //Si on est à la fin d'un dossier alors on le pop du path actuel
                        path->pop();
                }
            }
        }

        string getNoteContent(ifstream& stream){
            string line;
            getline(stream, line);
            return replace(line);
        }

       string replace(string line, string regex = "  ", string future = "") {
           size_t startPos = 0;

           while((startPos = line.find(regex, startPos)) != string::npos) {
               line.replace(startPos, regex.length(), future);
               startPos += future.length();
           }

           return line;
       }

       FILETYPE getFileType(string line) {
            switch(line[1]){
                case 'F': return DOSSIER;
                case 'N': return FICHIER;
                case 'H': return COMPRESSED;
            }
        }

        bool getEndFile(string line) {
            return line[1] == '/' && line[2] != 'N';
        }

        bool getEndNote(string line) {
            return line[1] == '/' && line[2] == 'N';
        }

        string getAttribute(string line, string attribut) {
            size_t startPos = 0;

           startPos = line.find(attribut + "='", startPos);
           line.erase(0, startPos + attribut.length() + 2);
           line.erase(line.end()- 2, line.end());

            return line;
       }

    public:
        //Surcharge
        Serialization() {
            this->root = nullptr;
            this->path = new stack<Folder*>;
        }

        Serialization(Folder* root) {
            this->root = root;
            this->path = new stack<Folder*>;
        }

        ~Serialization() {
            delete path->top();
            delete root;
        }

        void addRoot(Folder* root) { this->root = root; }

        void sauvegarder(string path){
            ofstream fluxSauvegarde(path.c_str());
            if(fluxSauvegarde){
                //Lecture de chaque fichier et de son contenu
                sauvegarder(root, fluxSauvegarde, "");
                fluxSauvegarde.close();
            }
            else
                throw std::invalid_argument("Erreur dans le flux de sauvegarde. (Verifier le path)");
        }

        Folder* chargement(string path) {
            ifstream fluxChargement(path.c_str());

            if(fluxChargement){
                chargement(fluxChargement);

                fluxChargement.close();
                return this->root;
            }
            else
                throw std::invalid_argument("Erreur dans le flux de chargement. (Verifier le path)");
        }
};


#endif
