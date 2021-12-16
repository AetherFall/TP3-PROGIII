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
string title;
Serialization* dataFile;
//AVLTree<int>* selections;

/**
 * Fonction qui ajuste la taille des label appartenant aux fichiers en tout genre.
 * @param path Endroit ou se trouve le label a verifier
 * @param index Emplacement precis ou se trouve le dit label
 * @param Type Quel type de fichier est-ce (FICHIER, DOSSIER, COMPRESSED)
 * @return retourne la longueur optimale en string pour le label
 */
string getVerificationNameSize(stack<Folder*>* path, int index, FILETYPE Type) {
    if(Type == FILETYPE::DOSSIER)
        return (Window::getStringWidth(path->top()->getFolderNameAt(index)) >= Window::getIconWidth()) ?
               path->top()->getFolderNameAt(index).replace(12,path->top()->getFolderNameAt(index).size() -12,"...") : path->top()->getFolderNameAt(index);
    else
        return (Window::getStringWidth(path->top()->getNoteNameAt(index)) >= Window::getIconWidth()) ?
               path->top()->getNoteNameAt(index).replace(12,path->top()->getNoteNameAt(index).size() -12,"...") : path->top()->getNoteNameAt(index);
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
        name = getVerificationNameSize(path, i, DOSSIER);

        Window::drawString(name, posX + ((Window::getIconWidth() -  Window::getStringWidth(name)) / 2), (Window::getIconHeight() - 25) + posY);
        posX += Window::getIconWidth();
    }

    //Définition du système de fichier actuel
    for(i = 0; i < path->top()->getNoteSize(); i++) {
        if(posX + Window::getIconWidth() > Window::getWidth()) {
            posY += Window::getIconHeight();
            posX = 0;
        }
        name = getVerificationNameSize(path, i, FICHIER);

        Window::drawIcon(Icon::NOTE, posX, posY, selections->search(i + path->top()->getFolderSize()));
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
            if (index < signed(path->top()->getFolderSize() + path->top()->getNoteSize())) {

                //Fichiers
                if (index < signed(path->top()->getFolderSize()))
                    if (index < 0) {
                        title.erase(title.end() - path->top()->getName().size() - 1, title.end());
                        path->pop();
                    } else {
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
        if (index != -1 && index < path->top()->getAllSize() && selections->size() != path->top()->getAllSize()) {
            if(!ctrl)
                selections->removeAll();
            selections->add(index);
        }

        switch(selections->size()) {
            case 0: Window::showMenu(x, y, SELECT_ALL | NEW_NOTE | NEW_FOLDER); break;
            case 1: Window::showMenu(x, y, SELECT_ALL | RENAME | DELETE); break;
            default: Window::showMenu(x, y,SELECT_ALL | DELETE); break;
        }

        //Si nous somme sur une note
        if(selections->size() == 1 && index > path->top()->getFolderSize() && index < path->top()->getAllSize())
            Window::showMenu(x,y, SELECT_ALL | NEW_NOTE | NEW_FOLDER | RENAME | DELETE | ENCODE ); //TODO Ajouter l'encodage

        //TODO Gérer les fichier compressé
    }
}

void onMenuClick(const unsigned int& menuItem) {
    int i = 0;
    switch (menuItem) {
        case Menu::NEW_FOLDER:
            path->top()->createFolder(Window::showTextField());
            path->top()->sortFolders(0, path->top()->getFolderSize() - 1, FILETYPE::DOSSIER);
            break;

        case Menu::NEW_NOTE:
            path->top()->createFile(Window::showTextField());
            path->top()->sortFolders(0, path->top()->getNoteSize() - 1, FILETYPE::FICHIER);
            break;

        case Menu::RENAME:  // TODO : Renommer le dossier ou la note
            //Recherche de l'index sélectionné.
            while (i < path->top()->getAllSize() && !selections->search(i)) { i++; }

            if (i < path->top()->getFolderSize()) {
                path->top()->setFolderNameAt(Window::showTextField(path->top()->getFolderNameAt(i)), i);
                path->top()->sortFolders(0, path->top()->getNoteSize() - 1, FILETYPE::DOSSIER);
            }
            else {
                path->top()->setNoteNameAt(Window::showTextField(path->top()->getFolderNameAt(i)), i);
                path->top()->sortFolders(0, path->top()->getNoteSize() - 1, FILETYPE::FICHIER);
            }
            break;

        case Menu::DELETE: // TODO : Supprimer le ou les dossiers, et tout ce qu'ils contiennent, et la ou les notes sélectionner
            if(selections->size()) {
                queue<int>* traversal = selections->traversal(Traversal::Postfix);
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
        //Dans un soucis de ne pas écrire par dessus mon système de fichier à tout va.
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

    delete path->top(); //Suppression de la root, qui supprimera tout en cascade.
    delete path;
    delete dataFile;
    delete selections;
}