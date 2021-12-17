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

enum FILETYPE { DOSSIER, FICHIER, COMPRESSED, NONE };

class Folder {
private:
	string name;
	vector<Folder*> folders;
	vector<Note*> notes;

public:
    Folder(string name) {
        this->name = move(name);
    }

    ~Folder() {
        while(!folders.empty()) {
            Folder* temp = folders.back();
            folders.pop_back();
            delete temp;
        }

        while(!notes.empty()) {
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

    int getFolderSize()         { return signed(folders.size()); }
    int getNoteSize()           { return signed(notes.size()); }
    int getAllSize()            { return signed(folders.size()) + signed(notes.size()); }

    Folder* getFolderAt(int i)     { return this->folders.at(i); }
    Folder* getLastFolder()        { return this->folders.at(folders.size() -1); }
    Note* getLastNote()            { return this->notes.at(notes.size() -1); }

    //Setter
    void setName(string name)                    { this->name = move(name); }
    void setNoteContentAt(string content, int i) { this->notes.at(i)->setContent(move(content)); }
    void setNoteNameAt(string name, int i)       { this->notes.at(i)->setName(move(name)); }
    void setFolderNameAt(string name, int i)     { this->folders.at(i)->setName(move(name)); }
    void createFolder(string folder)             { folders.push_back(new Folder(move(folder))); }

    void createFile(string file)                 { notes.push_back(new Note(move(file)));}

    //Suppression
    void removeNoteAt(int i)   { delete this->notes.at(i); }
    void removeFolderAt(int i) { delete this->folders.at(i); }

    //Tri par QuickSort
    void sortFolders(long min, long max, FILETYPE type) {
        long g = min, d = max, p = min;

        while (g != d) {
            if (notes.at(g)->getName() > notes.at(d)->getName()) {
                if(type == FILETYPE::DOSSIER)
                    swap(folders.at(g), folders.at(d));
                else if(type == FILETYPE::FICHIER)
                    swap(notes.at(g), notes.at(d));
                else // Fichier compressé
                    swap(notes.at(g), notes.at(d));

                p = g + d - p;
            }

            (p == g) ? d-- : g++;
        }

        if (min < p - 1) sortFolders(min, p - 1, type);
        if (max > p + 1) sortFolders(p + 1, max, type);
    }
};
