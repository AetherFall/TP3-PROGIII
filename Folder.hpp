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

    ~Folder() {/*
        while(!folders.empty()) {
            Folder* temp = folders.back();
            folders.pop_back();
            delete temp;
        }

        while(!notes.empty()) {
            Note* temp = notes.back();
            notes.pop_back();
            delete temp;
        }*/

        int i;
        for(i =0; i < folders.size(); i++)
            delete folders.at(i);
        for(i = 0; i < notes.size(); i++)
            delete notes.at(i);
    }

    //Getter
    string getName()               { return this->name; }
    string getFolderNameAt(int i)  { return this->folders.at(i)->getName(); }
    string getNoteNameAt(int i)    { return this->notes.at(i)->getName(); }
    string getNoteContentAt(int i) { return this->notes.at(i)->getContent(); }
    bool getNoteCompressionAt(int i)     { return this->notes.at(i)->isCompressed(); }

    int getFolderSize()         { return signed(folders.size()); }
    int getNoteSize()           { return signed(notes.size()); }
    int getAllSize()            { return signed(folders.size()) + signed(notes.size()); }

    Folder* getFolderAt(int i)     { return this->folders.at(i); }
    Folder* getLastFolder()        { return this->folders.at(folders.size() -1); }
    Note* getLastNote()            { return this->notes.at(notes.size() -1); }

    //Setter
    void setName(string name)                       { this->name = move(name); }
    void setNoteContentAt(string content, int i)    { this->notes.at(i)->setContent(move(content)); }
    void setNoteNameAt(string name, int i)          { this->notes.at(i)->setName(move(name)); }
    void setNoteCompressAt(bool compression, int i) { this->notes.at(i)->setCompression(compression); }
    void setFolderNameAt(string name, int i)        { this->folders.at(i)->setName(move(name)); }
    void createFolder(string folder)                { folders.push_back(new Folder(move(folder))); }
    void createFile(string file)                    { notes.push_back(new Note(move(file)));}

    //Suppression
    void removeNoteAt(int i)   {
        Note* temp = this->notes.at(i);
        this->notes.erase(this->notes.begin() + i);
        delete temp;
    }

    void removeFolderAt(int i) {
        Folder* temp = this->folders.at(i);
        this->folders.erase(this->folders.begin() + i);
        delete temp;
    }

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
            else {// Fichier compress??
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
