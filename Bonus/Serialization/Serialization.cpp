#include "Serialization.hpp"

// PUBLIC ----------------------------------------

Serialization::Serialization() {
    this->root = nullptr;
    this->path = new stack<Folder*>;
}

Serialization::Serialization(Folder* root) {
    this->root = root;
    this->path = new stack<Folder*>;
}

Serialization::~Serialization() {
    delete root;
    delete path;
}

void Serialization::sauvegarder(const string& filePath){
    ofstream fluxSauvegarde(filePath.c_str());
    if(fluxSauvegarde){
        //Lecture de chaque fichier et de son contenu
        sauvegarder(root, fluxSauvegarde,  "");
        fluxSauvegarde.close();
    }
    else
        throw std::invalid_argument("Erreur dans le flux de sauvegarde. (Verifier le path)");
}

Folder* Serialization::chargement(const string& filePath) {
    ifstream fluxChargement(filePath.c_str());

    if(fluxChargement){
        chargement(fluxChargement);
        fluxChargement.close();
    }

    else {
        ofstream fluxDeCreation(filePath.c_str());

        //Création d'un fichier XML s'il n'existe pas déjà
        (fluxDeCreation)? fluxDeCreation.close() : throw std::invalid_argument("Erreur dans le flux de chargement. (Verifier le path)");
    }

    root = this->root ?: new Folder("/");
    return root;
}

string Serialization::replace(string line, const string &regex, const string &future) {
    size_t startPos = 0;

    while((startPos = line.find(regex, startPos)) != string::npos) {
        line.replace(startPos, regex.length(), future);
        startPos += future.length();
    }

    return line;
}

//PRIVATE ---------------------------------------

/**
 * Sauvegarde du fichier de façon récursif
 * @param fichier Folder dans lequel nous sommes
 * @param stream Flux stream du fichier en cours
 * @param level Niveau d'indentation du fichier
 */
void Serialization::sauvegarder(Folder* fichier, std::ofstream& stream, string level) {
    stream << level << "<F name='" << fichier->getName() <<"'>" << endl;
    string level2 = level + "    ";

    if(fichier->getAllSize()) {
        if(fichier->getFolderSize()) {
            for(int i = 0; i < fichier->getFolderSize(); i++)
                sauvegarder(fichier->getFolderAt(i), stream, level2);
        }
        if(fichier->getNoteSize()) {
            for(int i = 0; i < fichier->getNoteSize(); i++) {
                if(fichier->getNoteCompressionAt(i)) {
                    stream << level2 << "<H name='" << fichier->getNoteNameAt(i) << "'>" << endl;
                    stream << level2 << fichier->getNoteContentAt(i) << endl << level2 << "</H>" << endl << endl;
                }
                else {
                    stream << level2 << "<N name='" << fichier->getNoteNameAt(i) << "'>" << endl;
                    stream << level2 << fichier->getNoteContentAt(i) << endl << level2 << "</N>" << endl << endl;
                }
            }
        }
    }

    stream << level << "</F>" <<endl<<endl;
}

/**
* Fonction de chargement de la fenêtre récursivement
* @param stream
*/
void Serialization::chargement(std::ifstream& stream){
    string line;

    while(getline(stream, line)) {
        line = replace(line); //Suppression de tous les espaces du niveau de l'indentation.

        if(!line.empty()) {
            if (!getEndFile(line)) {
                if (!getEndNote(line)) {
                    switch (getFileType(line)) {
                        case DOSSIER:
                            if (!path->empty()) {
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
                            path->top()->createFile(getAttribute(line, "name"));
                            path->top()->getLastNote()->setContent(getNoteContent(stream));
                            path->top()->getLastNote()->setCompression(true);
                            break;
                    }
                }
            } else //Si on est à la fin d'un dossier alors on le pop du path actuel
                path->pop();
        }
    }
}

string Serialization::getNoteContent(ifstream& stream){
    string line;
    getline(stream, line);
    return replace(line);
}

FILETYPE Serialization::getFileType(string line) {
    switch(line[1]){
        case 'F': return DOSSIER;
        case 'N': return FICHIER;
        case 'H': return COMPRESSED;
    }
    return NONE;
}

bool Serialization::getEndFile(string line) {
    return line[1] == '/' && line[2] == 'F';
}

bool Serialization::getEndNote(string line) {
    return line[1] == '/' && (line[2] == 'N' || line[2] == 'H');
}

string Serialization::getAttribute(string line, const string& attribut) {
    size_t startPos = 0;

    startPos = line.find(attribut + "='", startPos);
    line.erase(0, startPos + attribut.length() + 2);
    line.erase(line.end()- 2, line.end());

    return line;
}