#pragma once

#include <stack>
#include <queue>
#include <vector>
#include <string>
#include "BSTree.hpp"
//#include "AVLTRee.hpp"
#include "Folder.hpp"
#include "Note.hpp"
#include "Serialization.hpp"

#include <iostream>

#define pathFileSystem "..//saves//ntfs.xml"

/**
    @author William-Benoit Lambert
    @code 1735971
    @page TP3.hpp
    @note Fonctions principales aux fonctionnements de l'application.
 */

using namespace std;

stack<Folder*>* path;
BSTree<int>* selections;
Serialization* dataFile;
string title;

//AVLTree<int>* selections;

/**
 * Fonction qui ajuste la taille des label appartenant aux fichiers en tout genre.
 * @param thisPath Endroit ou se trouve le label a verifier
 * @param index Emplacement precis ou se trouve le dit label
 * @param Type Quel type de fichier est-ce (FICHIER, DOSSIER, COMPRESSED)
 * @return retourne la longueur optimale en string pour le label
 */
string getVerificationNameSize(int index, FILETYPE Type) {
    if(Type == FILETYPE::DOSSIER)
        return (Window::getStringWidth(path->top()->getFolderNameAt(index)) >= Window::getIconWidth()) ?
               path->top()->getFolderNameAt(index).replace(12,path->top()->getFolderNameAt(index).size() -12,"...") : path->top()->getFolderNameAt(index);
    else
        return (Window::getStringWidth(path->top()->getNoteNameAt(index)) >= Window::getIconWidth()) ?
               path->top()->getNoteNameAt(index).replace(12,path->top()->getNoteNameAt(index).size() -12,"...") : path->top()->getNoteNameAt(index);
}

/**
 * Effectue une vérification de l'existence de doublons et effectue les modifications nécessaires dans le cas de présence de doublons.
 * @param name Nom désire
 * @return Nom de l'élement modifier pour ne pas avoir de doublons
 */
string getNameBasedOnDoublons(string name, int val = 1){
    for(int i =0; i < path->top()->getAllSize(); i++){
        //Vérification existence de folder?
        if(path->top()->getFolderSize())
            if (path->top()->getFolderNameAt(i) == name) {
                if (val > 1)
                    name = Serialization::replace(name, "(" + to_string(val ) + ")", "");

                return getNameBasedOnDoublons(name + "(" + to_string(val) + ")", val + 1);
            }
        //Vérification existence Note?
        if(path->top()->getNoteSize())
            if(path->top()->getNoteNameAt(i  - path->top()->getFolderSize()) == name) {
                if (val != 1)
                    name = Serialization::replace(name, "(" + to_string(val -1) + ")", "");

                return getNameBasedOnDoublons(name + "(" + to_string(val) + ")", val +1);
            }
    }
    return name;
}

/**
 * Donne l'index de l'élément ou se trouve la souris
 * Formule de calcul: (W / iW) / x * (H / iH) / y
 * @param x Position en X du curseur
 * @param y Position en Y du curseur
 * @return index de la position de la souris
 */
int getIndex(const int& x, const int& y) { return (y /Window::getIconHeight()) * (Window::getWidth() / Window::getIconWidth()) + (x / Window::getIconWidth()); }

/**
 * Initialisation de début de programme.
 */
void onInit() {
    selections = new BSTree<int>;
    path = new stack<Folder*>;
    dataFile = new Serialization();

    path->push(dataFile->chargement(pathFileSystem));

    title = path->top()->getName();
}

/**
 * Affichage du contenu du dossier actuel (60x / Sec)
 */
void onRefresh() {
    int posX = 0, posY = 0, i;
    string name;

    //Paramétrage général
    Window::setTitle(title);

    //Fichier de retour en arriere
    if(path->size() > 1) {
        Window::drawIcon(Icon::FOLDER, posX, posY);
        Window::drawString("..", posX + ((Window::getIconWidth() - Window::getStringWidth("..")) / 2),(Window::getIconHeight() - 25) + posY);
        posX += Window::getIconWidth();
    }

    //Définition du système de dossier actuel
    for(i = 0; i < path->top()->getFolderSize(); i++) {
        if(posX + Window::getIconWidth() > Window::getWidth()) {
            posY += Window::getIconHeight();
            posX = 0;
        }

        Window::drawIcon(Icon::FOLDER, posX, posY, selections->search(i));

        //Vérification si le nom dépasse l'icon du dossier
        name = getVerificationNameSize(i, DOSSIER);

        Window::drawString(name, posX + ((Window::getIconWidth() -  Window::getStringWidth(name)) / 2), (Window::getIconHeight() - 25) + posY);
        posX += Window::getIconWidth();
    }

    //Définition du système de fichier actuel
    for(i = 0; i < path->top()->getNoteSize(); i++) {
        if(posX + Window::getIconWidth() > Window::getWidth()) {
            posY += Window::getIconHeight();
            posX = 0;
        }
        name = getVerificationNameSize(i, FICHIER);

        Window::drawIcon(Icon::NOTE, posX, posY, selections->search(i + signed (path->top()->getFolderSize())));
        Window::drawString(name, posX + ((Window::getIconWidth() - Window::getStringWidth(name)) / 2), (Window::getIconHeight() - 25) + posY);
        posX += Window::getIconWidth();
    }
}

/**
 * Événement onclick de la souris
 * @param x Position en x
 * @param y Position en y
 * @param button Si on appuie sur le bouton de droite
 * @param ctrl Si on appuie sur [CTRL] en même temp
 */
void onWindowClick(const int& x, const int& y, const bool& button, const bool& ctrl) {
    int index = getIndex(x, y) - ((path->size() > 1) ? 1 : 0);

    if (button){
        if(ctrl) {
            if (index != -1)
                (selections->search(index)) ? selections->remove(index) : selections->add(index);
        }

        else if(!ctrl && selections->size()) {
            //Déselection totale
            selections->removeAll();
        }

        else{
            //Changement de répertoire
            if (index < path->top()->getFolderSize() + path->top()->getNoteSize()) {

                //Fichiers
                if (index < path->top()->getFolderSize())
                    if (index < 0) {
                        title.erase(title.end() - unsigned(path->top()->getName().size()) - 1, title.end());
                        path->pop();
                    }

                    else {
                        path->push(path->top()->getFolderAt(index));
                        title = title + path->top()->getName() + "/";
                    }

                //Notes
                else {
                    index -= path->top()->getFolderSize();
                    path->top()->setNoteContentAt(Window::showTextField(path->top()->getNoteContentAt(index)), index);
                }
            }
        }

    }

    else {
        if (index != -1 && index < path->top()->getAllSize() && selections->size() != path->top()->getAllSize())
            selections->add(index);

        switch(selections->size()) {
            case 0: Window::showMenu(x, y, SELECT_ALL | NEW_NOTE | NEW_FOLDER); break;
            case 1: Window::showMenu(x, y, SELECT_ALL | RENAME | DELETE); break;
            default: Window::showMenu(x, y,SELECT_ALL | DELETE); break;
        }

        //Si nous somme sur une note
        if(selections->size() == 1 && index > path->top()->getFolderSize() && index < path->top()->getAllSize())
            Window::showMenu(x,y, SELECT_ALL | NEW_NOTE | NEW_FOLDER | RENAME | DELETE /*| ENCODE | DECODAGE*/ ); //TODO Ajouter l'encodage et le décodage

        //TODO Gérer les fichier compressé
    }
}

void onMenuClick(const unsigned int& menuItem) {
    int i = 0;
    string getName;

    switch (menuItem) {
        case Menu::NEW_FOLDER:
            path->top()->createFolder(getNameBasedOnDoublons(Window::showTextField()));
            path->top()->sortFolders(0, path->top()->getFolderSize() -1, FILETYPE::DOSSIER);
            break;

        case Menu::NEW_NOTE:
            path->top()->createFile(getNameBasedOnDoublons( Window::showTextField()));
            path->top()->sortFolders(0, path->top()->getNoteSize() - 1, FILETYPE::FICHIER);
            break;

        case Menu::RENAME:
                //Recherche de l'index sélectionné.
                while (i < path->top()->getAllSize() && !selections->search(i)) { i++; }

                if (i < path->top()->getFolderSize()) {
                    getName = getNameBasedOnDoublons(Window::showTextField(path->top()->getFolderNameAt(i)));

                    if(!getName.empty()) {
                        path->top()->setFolderNameAt(getName, i);
                        path->top()->sortFolders(0, path->top()->getNoteSize() - 1, DOSSIER);
                    }
                }

                else {
                    getName = getNameBasedOnDoublons(Window::showTextField(path->top()->getNoteNameAt(i - path->top()->getFolderSize())));

                    if(!getName.empty()) {
                        path->top()->setNoteNameAt(getName, i);
                        path->top()->sortFolders(0, path->top()->getNoteSize() - 1, FICHIER);
                    }
                }

                break;

        case Menu::DELETE:
                if(selections->size()) {
                    queue<int>* traversal = selections->traversal(Infix);
                    auto* reverseTraversal = new stack<int>;
                    unsigned int sizeTraversal = traversal->size();

                    //Inversion de la queue pour qu'elle devienne décroissante.
                    while(!traversal->empty()){
                        reverseTraversal->push(traversal->front());
                        traversal->pop();
                    }

                    //Suppression des éléments sélectionnés.
                    for(int iterateur = 0; iterateur < sizeTraversal; iterateur++) {
                        (reverseTraversal->top() >= path->top()->getFolderSize()) ?
                        path->top()->removeNoteAt(reverseTraversal->top() - path->top()->getFolderSize()) :
                        path->top()->removeFolderAt(reverseTraversal->top());

                        reverseTraversal->pop();
                    }

                    //Suppression de l'arbre afin de supprimer les index qui ont été supprimé qui sont encore dans l'arbre.
                    selections->removeAll();

                    delete reverseTraversal;
                }

                break;

        case Menu::ENCODE: // TODO : Encoder la note avec la m�thode de Huffman
                path->top()->sortFolders(0, 1 - 1, FILETYPE::COMPRESSED);
                break;

        case Menu::DECODE: // TODO : D�coder la note avec la méthode de Huffman
                break;

        case Menu::SELECT_ALL:
                selections->removeAll();
                for(int j = 0; j < path->top()->getAllSize(); j++)
                    selections->add(j);
                break;
    }
}

void onQuit() {
    //Sauvegarde du systeme de fichier
    const SDL_MessageBoxButtonData buttons[] = {
            {0, 0, "Non"},
            {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "Oui"}};

    const SDL_MessageBoxData msgData = { SDL_MESSAGEBOX_INFORMATION,
                                         nullptr,
                                         "Sauvegarder?",
                                         "Souhaitez-vous sauvegarder les modifications qui ont été apporté dans la dernière execution du programme?",
                                         SDL_arraysize(buttons), buttons};
    int buttonid;

    if (SDL_ShowMessageBox(&msgData, &buttonid) < 0) //Si erreur.
        SDL_Log("Erreur d'affichage du message box.");

    if (buttonid == 1) //Sélection du bouton "Oui"
        dataFile->sauvegarder(pathFileSystem);

    //On retourne à la root.
    while(path->size() > 1){ path->pop(); }

    delete path;
    delete dataFile;
    delete selections;
}