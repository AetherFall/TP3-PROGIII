#pragma once

#include <vector>
#include "Note.hpp"

/**
    @author William-Benoit Lambert
    @code 1735971
    @page Folder.hpp
    @note object dossier
 */

using namespace std;

enum FILETYPE { DOSSIER, FICHIER, COMPRESSED };

class Folder {
private:
	string name;
	vector<Folder*> folders;
	vector<Note*> notes;

public:
    Folder(string name) {
        this->name = name;
    }

    ~Folder() {
        while(this->folders.size()) {
            Folder* temp = folders.back();
            folders.pop_back();
            delete temp;
        }

        while(this->notes.size()) {
            Note* temp = notes.back();
            notes.pop_back();
            delete temp;
        }
    }

    //Getter
    string getName()               { return this->name; }
    string getFolderNameAt(int i)  { return this->folders.at(i)->getName(); }
    string getNoteNameAt(int i)    { return this->notes.at(i)->getName(); }
    string getNoteContentAt(int i) { return this->notes.at(i)->getContent(); }

    size_t getFolderSize()         { return folders.size(); }
    size_t getNoteSize()           { return notes.size(); }
    size_t getAllSize()            { return folders.size() + notes.size(); }

    Folder* getFolderAt(int i)     { return this->folders.at(i); }
    Folder* getLastFolder()        { return this->folders.at(folders.size() -1); }
    Note* getLastNote()            { return this->notes.at(notes.size() -1); }

    //Setter
    void setName(string name)                    { this->name = name; }
    void setNoteContentAt(string content, int i) { this->notes.at(i)->setContent(content); }
    void setNoteNameAt(string name, int i)       { this->notes.at(i)->setName(name); }
    void setFolderNameAt(string name, int i)     { this->folders.at(i)->setName(name); }
    void createFolder(string folder)             { folders.push_back(new Folder(folder)); }

    void createFile(string file)                 { notes.push_back(new Note(file));}

    //Suppression
    void removeNoteAt(int i)   { delete this->notes.at(i); }
    void removeFolderAt(int i) { delete this->folders.at(i); }

    //Tri par QuickSort
    void sortFolders(long min, long max, FILETYPE type) {
        long g = min, d = max, p = min;

        while (g != d) {
            if(type == FILETYPE::DOSSIER) {
                if (folders.at(g)->getName() > folders.at(d)->getName()) {
                    swap(folders.at(g), folders.at(d));
                    p = g + d - p;
                }
            }
            else if(type == FILETYPE::FICHIER) {
                if (notes.at(g)->getName() > notes.at(d)->getName()) {
                    swap(notes.at(g), notes.at(d));
                    p = g + d - p;
                }
            }
            else {// Fichier compressé
                if (notes.at(g)->getName() > notes.at(d)->getName()) {
                    swap(notes.at(g), notes.at(d));
                    p = g + d - p;
                }
            }

            (p == g) ? d-- : g++;
        }

        if (min < p - 1) sortFolders(min, p - 1, type);
        if (max > p + 1) sortFolders(p + 1, max, type);
    }
};
