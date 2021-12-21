#pragma once

/**
    @author William-Benoit Lambert
    @code 1735971
    @page Note.hpp
    @note object note
 */

using namespace std;

class Note{
private:
    string name;
    string content;
    bool compression;

public:
	Note(string name, string content = "", bool isCompress = false){
        this->name = move(name);
        this->content = move(content);
        this->compression = isCompress;
    }

    string getName(){ return this->name; }
    string getContent() { return this->content; }
    bool isCompressed() { return compression; }

    void setName(string name) { this->name = move(name); }
    void setContent(string content) { this->content = move(content); }
    void setCompression(bool comp) { this->compression = comp; }

};
